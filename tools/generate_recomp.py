#!/usr/bin/env python3
"""
CPS1 SF2 Code Generator — translates M68K instructions to recompiled C code.

Reads an assembled CPS1 68K binary, disassembles all discovered functions,
and generates C source files that use the cps1recomp API.

Usage:
    python tools/generate_recomp.py build/sf2_68k.bin [--output-dir src/recomp/]
"""

import struct
import sys
import os
import re
from collections import defaultdict
from capstone import Cs, CS_ARCH_M68K, CS_MODE_M68K_000

# Import the analyzer
sys.path.insert(0, os.path.dirname(__file__))
from analyze_rom import CPS1ROM, M68KAnalyzer


# ============================================================
# M68K → C instruction translator
# (Adapted from pigskin — identical M68K translation logic,
#  just targets cps1recomp instead of genrecomp)
# ============================================================

class M68KTranslator:
    """Translates individual M68K instructions to C statements using cps1recomp macros."""

    CC_MAP = {
        'hi': 'M68K_CC_HI', 'ls': 'M68K_CC_LS',
        'cc': 'M68K_CC_CC', 'cs': 'M68K_CC_CS',
        'ne': 'M68K_CC_NE', 'eq': 'M68K_CC_EQ',
        'vc': 'M68K_CC_VC', 'vs': 'M68K_CC_VS',
        'pl': 'M68K_CC_PL', 'mi': 'M68K_CC_MI',
        'ge': 'M68K_CC_GE', 'lt': 'M68K_CC_LT',
        'gt': 'M68K_CC_GT', 'le': 'M68K_CC_LE',
    }

    SIZE_SUFFIX = {'.b': 8, '.w': 16, '.l': 32}

    def __init__(self, rom, labels):
        self.rom = rom
        self.labels = labels
        self.func_start = 0
        self.func_end = 0

    def translate_instruction(self, addr, mnemonic, op_str, raw_bytes, func_start, func_end=0):
        self.func_start = func_start
        self.func_end = func_end
        lines = []

        if addr in self.labels and addr != func_start:
            label = self.labels[addr]
            if label.startswith('loc_'):
                lines.append(f"{label}:")

        size = 16
        base_mnem = mnemonic
        for suffix, sz in self.SIZE_SUFFIX.items():
            if mnemonic.endswith(suffix):
                size = sz
                base_mnem = mnemonic[:-2]
                break

        c_code = self._translate(addr, base_mnem, mnemonic, op_str, size, raw_bytes)
        if c_code:
            lines.append(f"    {c_code}")
        else:
            hex_str = ' '.join(f'{b:02X}' for b in raw_bytes)
            lines.append(f"    /* TODO ${addr:06X}: {mnemonic} {op_str}  [{hex_str}] */")

        return lines

    def _translate(self, addr, base, mnemonic, op_str, size, raw_bytes):
        ops = self._split_operands(op_str) if op_str else []

        # SR/CCR moves
        if base == 'move' and any(o.strip().lower() in ('sr', 'ccr') for o in ops):
            return self._gen_move_sr(ops, mnemonic)
        if base == 'move': return self._gen_move(ops, size)
        if base == 'movea': return self._gen_movea(ops, size)
        if base == 'moveq': return self._gen_moveq(ops)
        if mnemonic in ('movem.l', 'movem.w'): return self._gen_movem(ops, mnemonic, size)
        if base == 'clr': return self._gen_clr(ops, size)
        if base == 'lea': return self._gen_lea(ops)
        if base == 'pea': return self._gen_pea(ops)
        if base == 'exg' or mnemonic == 'exg': return self._gen_exg(ops)
        if base == 'ext': return self._gen_ext(ops, size)
        if mnemonic == 'swap': return self._gen_swap(ops)
        if mnemonic == 'link': return self._gen_link(ops)
        if mnemonic == 'unlk': return self._gen_unlk(ops)

        # Arithmetic
        if base == 'add': return self._gen_arith('ADD', ops, size)
        if base == 'adda': return self._gen_adda(ops, size)
        if base == 'addi': return self._gen_arith('ADD', ops, size)
        if base == 'addq': return self._gen_addq(ops, size)
        if base == 'addx': return self._gen_arith('ADDX', ops, size)
        if base == 'sub': return self._gen_arith('SUB', ops, size)
        if base == 'suba': return self._gen_suba(ops, size)
        if base == 'subi': return self._gen_arith('SUB', ops, size)
        if base == 'subq': return self._gen_subq(ops, size)
        if base == 'subx': return self._gen_arith('SUBX', ops, size)
        if base == 'cmp': return self._gen_cmp(ops, size)
        if base == 'cmpa': return self._gen_cmpa(ops, size)
        if base == 'cmpi': return self._gen_cmp(ops, size)
        if base == 'neg': return self._gen_neg('NEG', ops, size)
        if base == 'negx': return self._gen_neg('NEGX', ops, size)
        if base == 'mulu': return self._gen_mul('MULU', ops)
        if base == 'muls': return self._gen_mul('MULS', ops)
        if base == 'divu': return self._gen_div('DIVU', ops)
        if base == 'divs': return self._gen_div('DIVS', ops)

        # Logic (check ANDI/ORI/EORI to SR/CCR first)
        if base in ('andi', 'ori', 'eori') and len(ops) == 2:
            dst_lower = ops[1].strip().lower()
            if dst_lower in ('sr', 'ccr'):
                imm = self._imm(ops[0])
                fn = 'sr' if dst_lower == 'sr' else 'ccr'
                get_fn = f'm68k_get_{fn}()'
                set_fn = f'm68k_set_{fn}'
                op_char = {'andi': '&', 'ori': '|', 'eori': '^'}[base]
                return f'{set_fn}({get_fn} {op_char} {imm});'
        if base in ('and', 'andi'): return self._gen_logic('AND', ops, size)
        if base in ('or', 'ori'): return self._gen_logic('OR', ops, size)
        if base in ('eor', 'eori'): return self._gen_logic('EOR', ops, size)
        if base == 'not': return self._gen_unary_logic('NOT', ops, size)
        if base == 'tst': return self._gen_tst(ops, size)

        # Bit operations
        if base == 'btst': return self._gen_bit('BTST', ops)
        if base == 'bset': return self._gen_bit('BSET', ops)
        if base == 'bclr': return self._gen_bit('BCLR', ops)
        if base == 'bchg': return self._gen_bit('BCHG', ops)

        # Shifts
        for shift_op in ('lsl', 'lsr', 'asr', 'rol', 'ror'):
            if base == shift_op:
                macro = shift_op.upper()
                if macro == 'ASL': macro = 'LSL'  # ASL == LSL
                return self._gen_shift(macro, ops, size)
        if base == 'asl': return self._gen_shift('LSL', ops, size)

        # Branch/control
        if base == 'bra': return self._gen_bra(ops, addr)
        if base == 'bsr': return self._gen_bsr(ops, addr)
        if base == 'jmp': return self._gen_jmp(ops, addr)
        if base == 'jsr': return self._gen_jsr(ops, addr)
        if mnemonic == 'rts': return 'return;'
        if mnemonic == 'rte': return 'return; /* RTE */'
        if mnemonic == 'rtr': return '{ uint16_t _sr = bus_read16(g_m68k.a[7]); g_m68k.a[7] += 2; m68k_set_ccr((uint8_t)_sr); return; }'

        for cc_suffix, cc_macro in self.CC_MAP.items():
            if base == f'b{cc_suffix}': return self._gen_bcc(cc_macro, ops, addr)
            if base == f's{cc_suffix}': return self._gen_scc(cc_macro, ops)
            if base == f'db{cc_suffix}': return self._gen_dbcc(cc_macro, ops, addr)

        if mnemonic == 'dbra': return self._gen_dbcc('0', ops, addr)
        if base == 'st' or mnemonic == 'st': return self._gen_scc('M68K_CC_T', ops)
        if base == 'sf' or mnemonic == 'sf': return self._gen_scc('M68K_CC_F', ops)
        if mnemonic == 'nop': return '/* nop */'
        if base == 'trap':
            return f'/* TRAP #{self._imm(ops[0])} */'

        return None

    # ================================================================
    # Operand helpers (identical to pigskin)
    # ================================================================

    @staticmethod
    def _split_operands(op_str):
        parts = []
        depth = 0
        current = []
        for ch in op_str:
            if ch in ('(', '['): depth += 1; current.append(ch)
            elif ch in (')', ']'): depth -= 1; current.append(ch)
            elif ch == ',' and depth == 0: parts.append(''.join(current).strip()); current = []
            else: current.append(ch)
        if current: parts.append(''.join(current).strip())
        return parts

    @staticmethod
    def _parse_reglist(reglist_str):
        regs = []
        for part in reglist_str.replace(' ', '').lower().split('/'):
            m = re.match(r'^([da])(\d)-([da])(\d)$', part)
            if m:
                for i in range(int(m.group(2)), int(m.group(4)) + 1):
                    regs.append(f'{m.group(1)}{i}')
            else:
                m2 = re.match(r'^([da])(\d)$', part)
                if m2: regs.append(f'{m2.group(1)}{m2.group(2)}')
                elif part == 'sp': regs.append('a7')
        return regs

    def _reg(self, op):
        op = op.strip().lower()
        if re.match(r'^d[0-7]$', op): return f'g_m68k.d[{op[1]}]'
        if re.match(r'^a[0-7]$', op): return f'g_m68k.a[{op[1]}]'
        if op == 'sp': return 'g_m68k.a[7]'
        if op == 'sr': return 'm68k_get_sr()'
        if op == 'ccr': return 'm68k_get_ccr()'
        if op == 'usp': return 'g_m68k.usp'
        return None

    def _imm(self, op):
        op = op.strip()
        if op.startswith('#$'): return f'0x{op[2:]}'
        if op.startswith('#0x'): return op[1:]
        if op.startswith('#-$'): return f'(-0x{op[3:]})'
        if op.startswith('#-'): return op[1:]
        if op.startswith('#'): return op[1:]
        return None

    def _parse_disp(self, s):
        s = s.strip()
        if not s or s == '0': return '0'
        neg = s.startswith('-')
        if neg: s = s[1:]
        if s.startswith('$'): val = f'0x{s[1:]}'
        elif s.startswith('0x'): val = s
        else: val = s
        return f'(-{val})' if neg else val

    def _ea_read(self, op, size):
        op = op.strip()
        r = self._reg(op)
        if r: return r
        imm = self._imm(op)
        if imm: return imm

        m = re.match(r'^\(a([0-7])\)$', op, re.I)
        if m: return f'bus_read{size}(g_m68k.a[{m.group(1)}])'

        m = re.match(r'^\(a([0-7])\)\+$', op, re.I)
        if m: return f'_postinc{size}({m.group(1)})'

        m = re.match(r'^-\(a([0-7])\)$', op, re.I)
        if m: return f'_predec{size}({m.group(1)})'

        m = re.match(r'^(-?\$?[\dA-Fa-f]+)\(a([0-7])\)$', op, re.I)
        if m: return f'bus_read{size}(g_m68k.a[{m.group(2)}] + {self._parse_disp(m.group(1))})'

        m = re.match(r'^(-?\$?[\dA-Fa-f]*)\(a([0-7]),\s*([da])([0-7])(?:\.(w|l))?(?:\s*\*\s*(\d+))?\)$', op, re.I)
        if m:
            disp = self._parse_disp(m.group(1)) if m.group(1) else '0'
            idx = f'g_m68k.{"d" if m.group(3).lower()=="d" else "a"}[{m.group(4)}]'
            if (m.group(5) or 'w').lower() == 'w': idx = f'(int16_t)(uint16_t){idx}'
            else: idx = f'(int32_t){idx}'
            if m.group(6) and m.group(6) != '1': idx = f'{idx} * {m.group(6)}'
            return f'bus_read{size}(g_m68k.a[{m.group(2)}] + {disp} + {idx})'

        m = re.match(r'^(-?\$?[\dA-Fa-f]+)\(pc\)$', op, re.I)
        if m: return f'bus_read{size}({self._parse_disp(m.group(1))})'

        m = re.match(r'^(-?\$?[\dA-Fa-f]*)\(pc,\s*([da])([0-7])(?:\.(w|l))?(?:\s*\*\s*(\d+))?\)$', op, re.I)
        if m:
            disp = self._parse_disp(m.group(1)) if m.group(1) else '0'
            idx = f'g_m68k.{"d" if m.group(2).lower()=="d" else "a"}[{m.group(3)}]'
            if (m.group(4) or 'w').lower() == 'w': idx = f'(int16_t)(uint16_t){idx}'
            else: idx = f'(int32_t){idx}'
            if m.group(5) and m.group(5) != '1': idx = f'{idx} * {m.group(5)}'
            return f'bus_read{size}({disp} + {idx})'

        m = re.match(r'^\(\$([0-9A-Fa-f]+)\)\.(w|l)$', op, re.I)
        if m: return f'bus_read{size}(0x{int(m.group(1), 16):06X})'

        m = re.match(r'^\$([0-9A-Fa-f]+)(?:\.(w|l))?$', op, re.I)
        if m: return f'bus_read{size}(0x{int(m.group(1), 16):06X})'

        return f'/* UNHANDLED_READ: {op} */ 0'

    def _ea_write(self, op, size, value_expr):
        op = op.strip()
        r = self._reg(op)
        if r:
            if size == 8: return f'{r} = ({r} & 0xFFFFFF00u) | ((uint8_t)({value_expr}));'
            elif size == 16: return f'{r} = ({r} & 0xFFFF0000u) | ((uint16_t)({value_expr}));'
            else: return f'{r} = {value_expr};'

        m = re.match(r'^\(a([0-7])\)$', op, re.I)
        if m: return f'bus_write{size}(g_m68k.a[{m.group(1)}], {value_expr});'

        m = re.match(r'^\(a([0-7])\)\+$', op, re.I)
        if m:
            inc = size // 8
            return f'bus_write{size}(g_m68k.a[{m.group(1)}], {value_expr}); g_m68k.a[{m.group(1)}] += {inc};'

        m = re.match(r'^-\(a([0-7])\)$', op, re.I)
        if m:
            dec = size // 8
            return f'g_m68k.a[{m.group(1)}] -= {dec}; bus_write{size}(g_m68k.a[{m.group(1)}], {value_expr});'

        m = re.match(r'^(-?\$?[\dA-Fa-f]+)\(a([0-7])\)$', op, re.I)
        if m: return f'bus_write{size}(g_m68k.a[{m.group(2)}] + {self._parse_disp(m.group(1))}, {value_expr});'

        m = re.match(r'^(-?\$?[\dA-Fa-f]*)\(a([0-7]),\s*([da])([0-7])(?:\.(w|l))?(?:\s*\*\s*(\d+))?\)$', op, re.I)
        if m:
            disp = self._parse_disp(m.group(1)) if m.group(1) else '0'
            idx = f'g_m68k.{"d" if m.group(3).lower()=="d" else "a"}[{m.group(4)}]'
            if (m.group(5) or 'w').lower() == 'w': idx = f'(int16_t)(uint16_t){idx}'
            else: idx = f'(int32_t){idx}'
            if m.group(6) and m.group(6) != '1': idx = f'{idx} * {m.group(6)}'
            return f'bus_write{size}(g_m68k.a[{m.group(2)}] + {disp} + {idx}, {value_expr});'

        m = re.match(r'^\(\$([0-9A-Fa-f]+)\)\.(w|l)$', op, re.I)
        if m: return f'bus_write{size}(0x{int(m.group(1), 16):06X}, {value_expr});'

        m = re.match(r'^\$([0-9A-Fa-f]+)(?:\.(w|l))?$', op, re.I)
        if m: return f'bus_write{size}(0x{int(m.group(1), 16):06X}, {value_expr});'

        return f'/* UNHANDLED_WRITE: {op} = {value_expr} */'

    def _ea_addr(self, op):
        op = op.strip()
        for pat, fmt in [
            (r'^\(a([0-7])\)\+$', 'g_m68k.a[{0}]'),
            (r'^-\(a([0-7])\)$', 'g_m68k.a[{0}]'),
            (r'^\(a([0-7])\)$', 'g_m68k.a[{0}]'),
        ]:
            m = re.match(pat, op, re.I)
            if m: return fmt.format(m.group(1))

        m = re.match(r'^(-?\$?[\dA-Fa-f]+)\(a([0-7])\)$', op, re.I)
        if m: return f'(g_m68k.a[{m.group(2)}] + {self._parse_disp(m.group(1))})'

        m = re.match(r'^(-?\$?[\dA-Fa-f]*)\(a([0-7]),\s*([da])([0-7])(?:\.(w|l))?(?:\s*\*\s*(\d+))?\)$', op, re.I)
        if m:
            disp = self._parse_disp(m.group(1)) if m.group(1) else '0'
            idx = f'g_m68k.{"d" if m.group(3).lower()=="d" else "a"}[{m.group(4)}]'
            if (m.group(5) or 'w').lower() == 'w': idx = f'(int16_t)(uint16_t){idx}'
            else: idx = f'(int32_t){idx}'
            if m.group(6) and m.group(6) != '1': idx = f'{idx} * {m.group(6)}'
            return f'(g_m68k.a[{m.group(2)}] + {disp} + {idx})'

        m = re.match(r'^(-?\$?[\dA-Fa-f]+)\(pc\)$', op, re.I)
        if m: return self._parse_disp(m.group(1))

        m = re.match(r'^(-?\$?[\dA-Fa-f]*)\(pc,\s*([da])([0-7])(?:\.(w|l))?(?:\s*\*\s*(\d+))?\)$', op, re.I)
        if m:
            disp = self._parse_disp(m.group(1)) if m.group(1) else '0'
            idx = f'g_m68k.{"d" if m.group(2).lower()=="d" else "a"}[{m.group(3)}]'
            if (m.group(4) or 'w').lower() == 'w': idx = f'(int16_t)(uint16_t){idx}'
            else: idx = f'(int32_t){idx}'
            if m.group(5) and m.group(5) != '1': idx = f'{idx} * {m.group(5)}'
            return f'({disp} + {idx})'

        m = re.match(r'^\(\$([0-9A-Fa-f]+)\)\.(w|l)$', op, re.I)
        if m: return f'0x{int(m.group(1), 16):06X}'

        m = re.match(r'^\$([0-9A-Fa-f]+)(?:\.(w|l))?$', op, re.I)
        if m: return f'0x{int(m.group(1), 16):06X}'

        return f'/* UNHANDLED_ADDR: {op} */ 0'

    # ================================================================
    # Instruction generators
    # ================================================================

    def _gen_move(self, ops, size):
        if len(ops) != 2: return None
        src = self._ea_read(ops[0], size)
        r = self._reg(ops[1])
        if r:
            stmt = self._ea_write(ops[1], size, src)
            return f'{stmt} M68K_TST{size}((uint{size}_t){r});'
        return f'{{ uint{size}_t _mv = (uint{size}_t)({src}); {self._ea_write(ops[1], size, "_mv")} M68K_TST{size}(_mv); }}'

    def _gen_movea(self, ops, size):
        if len(ops) != 2: return None
        src = self._ea_read(ops[0], size)
        r = self._reg(ops[1])
        if not r: return None
        if size == 16: return f'{r} = (uint32_t)(int32_t)(int16_t)(uint16_t)({src});'
        return f'{r} = {src};'

    def _gen_moveq(self, ops):
        if len(ops) != 2: return None
        imm = self._imm(ops[0])
        r = self._reg(ops[1])
        if imm and r: return f'{r} = (uint32_t)(int32_t)(int8_t)({imm}); M68K_TST32({r});'
        return None

    def _gen_movem(self, ops, mnemonic, size):
        byte_size = size // 8
        read_fn = f'bus_read{size}'
        write_fn = f'bus_write{size}'

        def _is_reglist(s):
            s = s.strip().lower()
            return bool(re.match(r'^[da]\d', s)) and ('/' in s or '-' in s or re.match(r'^[da]\d$', s))

        if _is_reglist(ops[0]):
            reglist = self._parse_reglist(ops[0])
            ea = ops[1].strip()
            m = re.match(r'^-\(a([0-7])\)$', ea, re.I)
            if m:
                n = m.group(1)
                stmts = []
                all_regs_rev = [f'a{i}' for i in range(7, -1, -1)] + [f'd{i}' for i in range(7, -1, -1)]
                for r in all_regs_rev:
                    if r in reglist:
                        stmts.append(f'g_m68k.a[{n}] -= {byte_size}; {write_fn}(g_m68k.a[{n}], {self._reg(r)});')
                return ' '.join(stmts)
            ea_addr = self._ea_addr(ea)
            stmts = [f'{{ uint32_t _addr = {ea_addr};']
            for r in reglist:
                stmts.append(f'{write_fn}(_addr, {self._reg(r)}); _addr += {byte_size};')
            stmts.append('}')
            return ' '.join(stmts)
        else:
            reglist = self._parse_reglist(ops[1])
            ea = ops[0].strip()
            m = re.match(r'^\(a([0-7])\)\+$', ea, re.I)
            if m:
                n = m.group(1)
                stmts = []
                for r in reglist:
                    rc = self._reg(r)
                    if size == 16:
                        stmts.append(f'{rc} = (uint32_t)(int32_t)(int16_t){read_fn}(g_m68k.a[{n}]); g_m68k.a[{n}] += {byte_size};')
                    else:
                        stmts.append(f'{rc} = {read_fn}(g_m68k.a[{n}]); g_m68k.a[{n}] += {byte_size};')
                return ' '.join(stmts)
            ea_addr = self._ea_addr(ea)
            stmts = [f'{{ uint32_t _addr = {ea_addr};']
            for r in reglist:
                rc = self._reg(r)
                if size == 16:
                    stmts.append(f'{rc} = (uint32_t)(int32_t)(int16_t){read_fn}(_addr); _addr += {byte_size};')
                else:
                    stmts.append(f'{rc} = {read_fn}(_addr); _addr += {byte_size};')
            stmts.append('}')
            return ' '.join(stmts)

    def _gen_clr(self, ops, size):
        return f'{self._ea_write(ops[0], size, "0")} g_m68k.flag_n = false; g_m68k.flag_z = true; g_m68k.flag_v = false; g_m68k.flag_c = false;'

    def _gen_lea(self, ops):
        r = self._reg(ops[1])
        return f'{r} = {self._ea_addr(ops[0])};' if r else None

    def _gen_pea(self, ops):
        return f'g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], {self._ea_addr(ops[0])});'

    def _gen_exg(self, ops):
        r1, r2 = self._reg(ops[0]), self._reg(ops[1])
        return f'{{ uint32_t _t = {r1}; {r1} = {r2}; {r2} = _t; }}' if r1 and r2 else None

    def _gen_ext(self, ops, size):
        r = self._reg(ops[0])
        return f'M68K_EXT{size}({r});' if r else None

    def _gen_swap(self, ops):
        r = self._reg(ops[0])
        return f'M68K_SWAP({r});' if r else None

    def _gen_link(self, ops):
        r, imm = self._reg(ops[0]), self._imm(ops[1])
        return f'g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], {r}); {r} = g_m68k.a[7]; g_m68k.a[7] += (int16_t)({imm});' if r and imm else None

    def _gen_unlk(self, ops):
        r = self._reg(ops[0])
        return f'g_m68k.a[7] = {r}; {r} = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4;' if r else None

    def _gen_arith(self, op, ops, size):
        src = self._ea_read(ops[0], size)
        dst_r = self._reg(ops[1])
        if dst_r: return f'M68K_{op}{size}({dst_r}, {src});'
        ea = self._ea_addr(ops[1])
        return f'{{ uint32_t _ea = {ea}; uint{size}_t _tmp = bus_read{size}(_ea); M68K_{op}{size}(_tmp, {src}); bus_write{size}(_ea, _tmp); }}'

    def _gen_adda(self, ops, size):
        src = self._ea_read(ops[0], size)
        r = self._reg(ops[1])
        if not r: return None
        return f'{r} += (int16_t)(uint16_t)({src});' if size == 16 else f'{r} += {src};'

    def _gen_suba(self, ops, size):
        src = self._ea_read(ops[0], size)
        r = self._reg(ops[1])
        if not r: return None
        return f'{r} -= (int16_t)(uint16_t)({src});' if size == 16 else f'{r} -= {src};'

    def _gen_addq(self, ops, size):
        imm = self._imm(ops[0])
        if not imm: return None
        dst_r = self._reg(ops[1])
        if dst_r:
            if ops[1].strip().lower().startswith('a'): return f'{dst_r} += {imm};'
            return f'M68K_ADD{size}({dst_r}, {imm});'
        ea = self._ea_addr(ops[1])
        return f'{{ uint32_t _ea = {ea}; uint{size}_t _tmp = bus_read{size}(_ea); M68K_ADD{size}(_tmp, {imm}); bus_write{size}(_ea, _tmp); }}'

    def _gen_subq(self, ops, size):
        imm = self._imm(ops[0])
        if not imm: return None
        dst_r = self._reg(ops[1])
        if dst_r:
            if ops[1].strip().lower().startswith('a'): return f'{dst_r} -= {imm};'
            return f'M68K_SUB{size}({dst_r}, {imm});'
        ea = self._ea_addr(ops[1])
        return f'{{ uint32_t _ea = {ea}; uint{size}_t _tmp = bus_read{size}(_ea); M68K_SUB{size}(_tmp, {imm}); bus_write{size}(_ea, _tmp); }}'

    def _gen_cmp(self, ops, size):
        return f'M68K_CMP{size}({self._ea_read(ops[1], size)}, {self._ea_read(ops[0], size)});'

    def _gen_cmpa(self, ops, size):
        src = self._ea_read(ops[0], size)
        r = self._reg(ops[1])
        if not r: return None
        if size == 16: return f'M68K_CMP32({r}, (uint32_t)(int32_t)(int16_t)(uint16_t)({src}));'
        return f'M68K_CMP32({r}, {src});'

    def _gen_neg(self, op, ops, size):
        r = self._reg(ops[0])
        if r: return f'M68K_{op}{size}({r});'
        ea = self._ea_addr(ops[0])
        return f'{{ uint32_t _ea = {ea}; uint{size}_t _tmp = bus_read{size}(_ea); M68K_{op}{size}(_tmp); bus_write{size}(_ea, _tmp); }}'

    def _gen_mul(self, op, ops):
        r = self._reg(ops[1])
        return f'M68K_{op}({r}, {self._ea_read(ops[0], 16)});' if r else None

    def _gen_div(self, op, ops):
        r = self._reg(ops[1])
        return f'M68K_{op}({r}, {self._ea_read(ops[0], 16)});' if r else None

    def _gen_logic(self, op, ops, size):
        src = self._ea_read(ops[0], size)
        dst_r = self._reg(ops[1])
        if dst_r: return f'M68K_{op}{size}({dst_r}, {src});'
        ea = self._ea_addr(ops[1])
        return f'{{ uint32_t _ea = {ea}; uint{size}_t _tmp = bus_read{size}(_ea); M68K_{op}{size}(_tmp, {src}); bus_write{size}(_ea, _tmp); }}'

    def _gen_unary_logic(self, op, ops, size):
        r = self._reg(ops[0])
        if r: return f'M68K_{op}{size}({r});'
        ea = self._ea_addr(ops[0])
        return f'{{ uint32_t _ea = {ea}; uint{size}_t _tmp = bus_read{size}(_ea); M68K_{op}{size}(_tmp); bus_write{size}(_ea, _tmp); }}'

    def _gen_tst(self, ops, size):
        return f'M68K_TST{size}({self._ea_read(ops[0], size)});'

    def _gen_bit(self, op, ops):
        bit = self._ea_read(ops[0], 32)
        r = self._reg(ops[1])
        if r: return f'M68K_{op}({r}, {bit});'
        ea = self._ea_addr(ops[1])
        if op == 'BTST':
            return f'M68K_BTST(bus_read8({ea}), {bit});'
        bop = {'BSET': '|', 'BCLR': '& ~', 'BCHG': '^'}[op]
        return f'{{ uint32_t _ea = {ea}; uint8_t _b = 1u << (({bit}) & 7); uint8_t _v = bus_read8(_ea); g_m68k.flag_z = !(_v & _b); bus_write8(_ea, _v {bop} _b); }}'

    def _gen_shift(self, op, ops, size):
        if len(ops) == 2:
            cnt = self._ea_read(ops[0], 8)
            r = self._reg(ops[1])
            if r: return f'M68K_{op}{size}({r}, {cnt});'
            ea = self._ea_addr(ops[1])
            return f'{{ uint32_t _ea = {ea}; uint{size}_t _tmp = bus_read{size}(_ea); M68K_{op}{size}(_tmp, {cnt}); bus_write{size}(_ea, _tmp); }}'
        elif len(ops) == 1:
            ea = self._ea_addr(ops[0])
            return f'{{ uint32_t _ea = {ea}; uint16_t _tmp = bus_read16(_ea); M68K_{op}16(_tmp, 1); bus_write16(_ea, _tmp); }}'
        return None

    def _is_local_target(self, target):
        return self.func_start <= target < self.func_end

    def _parse_branch_target(self, op):
        op = op.strip()
        m = re.match(r'^\$([0-9A-Fa-f]+)(?:\.(w|l))?$', op)
        if m:
            try: return int(m.group(1), 16)
            except ValueError: return None
        if op.startswith('0x'):
            try: return int(op.split('.')[0], 16)
            except ValueError: return None
        m = re.match(r'^\(\$([0-9A-Fa-f]+)\)\.(w|l)$', op)
        if m: return int(m.group(1), 16)
        return None

    def _gen_bra(self, ops, addr):
        target = self._parse_branch_target(ops[0])
        if target is not None and self._is_local_target(target):
            return f'goto {self.labels.get(target, f"loc_{target:06X}")};'
        if target is not None:
            return f'{{ func_table_call(0x{target:06X}); return; }}'
        return '/* BRA unresolved */'

    def _gen_bsr(self, ops, addr):
        target = self._parse_branch_target(ops[0])
        label = self.labels.get(target, f'sub_{target:06X}')
        return f'func_table_call(0x{target:06X}); /* {label} */'

    def _gen_jmp(self, ops, addr):
        target = self._parse_branch_target(ops[0])
        if target is not None:
            label = self.labels.get(target, f'sub_{target:06X}')
            if label.startswith('loc_'): return f'goto {label};'
            return f'{{ func_table_call(0x{target:06X}); return; }}'
        ea = self._ea_addr(ops[0])
        if 'UNHANDLED' not in ea:
            return f'{{ func_table_call({ea}); return; }}'
        return f'/* JMP indirect: {ops[0]} */'

    def _gen_jsr(self, ops, addr):
        target = self._parse_branch_target(ops[0])
        if target is not None:
            return f'func_table_call(0x{target:06X});'
        ea = self._ea_addr(ops[0])
        if 'UNHANDLED' not in ea:
            return f'func_table_call({ea});'
        return f'/* JSR indirect: {ops[0]} */'

    def _gen_bcc(self, cc, ops, addr):
        target = self._parse_branch_target(ops[0])
        if target is not None and self._is_local_target(target):
            return f'if ({cc}) goto {self.labels.get(target, f"loc_{target:06X}")};'
        if target is not None:
            return f'if ({cc}) {{ func_table_call(0x{target:06X}); return; }}'
        return '/* Bcc unresolved */'

    def _gen_scc(self, cc, ops):
        r = self._reg(ops[0])
        if r: return f'{r} = ({r} & 0xFFFFFF00u) | ({cc} ? 0xFFu : 0x00u);'
        return f'bus_write8({self._ea_addr(ops[0])}, {cc} ? 0xFF : 0x00);'

    def _gen_dbcc(self, cc, ops, addr):
        r = self._reg(ops[0])
        target = self._parse_branch_target(ops[1]) if len(ops) > 1 else None
        if not (r and target is not None): return None
        label = self.labels.get(target, f'loc_{target:06X}')
        local = self._is_local_target(target)
        goto_stmt = f'goto {label}' if local else f'func_table_call(0x{target:06X}); return'
        cond = '' if cc == '0' else f'if (!({cc})) '
        return f'{cond}{{ int16_t _cnt = (int16_t)(uint16_t){r}; _cnt--; {r} = ({r} & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) {{ {goto_stmt}; }} }}'

    def _gen_move_sr(self, ops, mnemonic):
        if 'sr' in ops[0].lower():
            return self._ea_write(ops[1], 16, 'm68k_get_sr()')
        else:
            return f'm68k_set_sr({self._ea_read(ops[0], 16)});'


# ============================================================
# Code generator
# ============================================================

class CodeGenerator:
    MAX_FUNCS_PER_FILE = 50

    def __init__(self, rom, analyzer, translator, output_dir):
        self.rom = rom
        self.analyzer = analyzer
        self.translator = translator
        self.output_dir = output_dir
        if os.path.exists(output_dir):
            for f in os.listdir(output_dir):
                if f.startswith('recomp_') and f.endswith('.c'):
                    os.remove(os.path.join(output_dir, f))
        os.makedirs(output_dir, exist_ok=True)

    def generate_all(self):
        functions = sorted(self.analyzer.functions.values(), key=lambda f: f['start'])
        chunks = []
        for i in range(0, len(functions), self.MAX_FUNCS_PER_FILE):
            chunk = functions[i:i + self.MAX_FUNCS_PER_FILE]
            chunk_name = f"recomp_{chunk[0]['start']:06X}_{chunk[-1]['end']:06X}"
            chunks.append((chunk_name, chunk))

        for chunk_name, chunk_funcs in chunks:
            self._generate_chunk(chunk_name, chunk_funcs)

        self._generate_registration(functions)

        print(f"\nGenerated {len(chunks)} source files with {len(functions)} functions")
        print(f"Output directory: {self.output_dir}")

    def _generate_chunk(self, chunk_name, funcs):
        lines = [
            f'/* Auto-generated recompiled code for Street Fighter II */',
            f'/* Source range: ${funcs[0]["start"]:06X} - ${funcs[-1]["end"]:06X} */',
            f'/* Functions: {len(funcs)} */',
            f'',
            f'#include <cps1recomp/cps1recomp.h>',
            f'#include "recomp_funcs.h"',
            f'',
        ]
        for func in funcs:
            lines.extend(self._generate_function(func))
            lines.append('')

        path = os.path.join(self.output_dir, f'{chunk_name}.c')
        with open(path, 'w') as f:
            f.write('\n'.join(lines))

    def _generate_function(self, func):
        lines = []
        name = func['name']
        start, end = func['start'], func['end']
        insn_addrs = func.get('insn_addrs', [])

        lines.append(f'/* ${start:06X}-${end:06X}  ({func["insn_count"]} instructions, {func["size"]} bytes) */')
        lines.append(f'void {name}(void) {{')

        if not insn_addrs:
            lines.append('    /* empty function */')
            lines.append('}')
            return lines

        for addr in insn_addrs:
            if addr not in self.analyzer.instructions:
                continue
            mnemonic, op_str, size, raw = self.analyzer.instructions[addr]
            c_lines = self.translator.translate_instruction(addr, mnemonic, op_str, raw, start, end)
            lines.extend(c_lines)

        if not func['has_return']:
            lines.append('    /* WARNING: function did not end with RTS */')

        lines.append('}')

        # Fix cross-function gotos
        defined_labels = set()
        for line in lines:
            stripped = line.strip()
            if stripped.endswith(':') and not stripped.startswith('/*'):
                defined_labels.add(stripped[:-1])

        fixed = []
        for line in lines:
            m = re.search(r'goto ((?:loc|sub|jt|vec)_([0-9A-Fa-f]+));', line)
            if m and m.group(1) not in defined_labels:
                addr_val = int(m.group(2), 16)
                line = line.replace(f'goto {m.group(1)};', f'{{ func_table_call(0x{addr_val:06X}); return; }}')
            fixed.append(line)

        # Fix labels at end of function: C requires a statement after a label
        # If a label line is followed by a comment or closing brace, add a no-op
        final = []
        for i, line in enumerate(fixed):
            final.append(line)
            stripped = line.strip()
            if stripped.endswith(':') and not stripped.startswith('/*') and not stripped.startswith('//'):
                # Check if next non-empty line is } or comment
                next_real = ''
                for j in range(i + 1, len(fixed)):
                    ns = fixed[j].strip()
                    if ns:
                        next_real = ns
                        break
                if next_real.startswith('/*') or next_real.startswith('}') or next_real == '':
                    final.append('    ; /* empty statement after label */')

        return final

    def _generate_registration(self, functions):
        lines = [
            '/* Auto-generated — forward declarations + registration */',
            '#ifndef RECOMP_FUNCS_H',
            '#define RECOMP_FUNCS_H',
            '',
            '#include <cps1recomp/cps1recomp.h>',
            '',
        ]
        for sz in [8, 16, 32]:
            inc = sz // 8
            lines.append(f'static inline uint{sz}_t _postinc{sz}(int n) {{ uint{sz}_t v = bus_read{sz}(g_m68k.a[n]); g_m68k.a[n] += {inc}; return v; }}')
        for sz in [8, 16, 32]:
            dec = sz // 8
            lines.append(f'static inline uint{sz}_t _predec{sz}(int n) {{ g_m68k.a[n] -= {dec}; return bus_read{sz}(g_m68k.a[n]); }}')
        lines.append('')

        for func in functions:
            lines.append(f'void {func["name"]}(void);')
        lines.append('')

        lines.append('static inline void recomp_register_all(void) {')
        for func in functions:
            lines.append(f'    func_table_register(0x{func["start"]:06X}, {func["name"]});')
        lines.append('}')
        lines.append('')
        lines.append('#endif /* RECOMP_FUNCS_H */')

        path = os.path.join(self.output_dir, 'recomp_funcs.h')
        with open(path, 'w') as f:
            f.write('\n'.join(lines))


def main():
    import argparse
    parser = argparse.ArgumentParser(description='CPS1 SF2 Code Generator')
    parser.add_argument('rom', help='Path to assembled 68K binary (sf2_68k.bin)')
    parser.add_argument('--output-dir', '-o', default='src/recomp', help='Output directory')
    args = parser.parse_args()

    print("=== CPS1 SF2 Code Generator ===\n")

    rom = CPS1ROM(args.rom)
    rom.print_info()

    print("\n--- Phase 1: Analysis ---")
    analyzer = M68KAnalyzer(rom)
    analyzer.analyze()

    print("\n--- Phase 2: Code Generation ---")
    translator = M68KTranslator(rom, analyzer.labels)
    generator = CodeGenerator(rom, analyzer, translator, args.output_dir)
    generator.generate_all()

    print("\nDone!")


if __name__ == '__main__':
    main()
