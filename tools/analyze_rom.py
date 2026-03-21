#!/usr/bin/env python3
"""
CPS1 SF2 ROM Analyzer — M68K disassembly + function discovery.

Reads an assembled CPS1 68K program binary (from extract_roms.py),
performs recursive-descent disassembly from known entry points,
discovers function boundaries via call graph + jump table scanning,
and exports a JSON function map for the code generator.

Usage:
    python tools/analyze_rom.py build/sf2_68k.bin [--output functions.json] [--stats]
"""

import struct
import json
import sys
import os
from collections import defaultdict
from capstone import Cs, CS_ARCH_M68K, CS_MODE_M68K_000


# ============================================================
# CPS1 ROM class (raw assembled binary, no header)
# ============================================================

class CPS1ROM:
    """Represents an assembled CPS1 68K program binary."""

    def __init__(self, path):
        with open(path, 'rb') as f:
            self.data = f.read()
        self.size = len(self.data)
        self._parse_vectors()

    def _parse_vectors(self):
        """Parse M68K exception vector table from the start of ROM.

        The 68000 vector table occupies the first 1024 bytes ($000-$3FF):
          $000: Initial SSP (Supervisor Stack Pointer)
          $004: Initial PC (entry point)
          $008: Bus Error
          ...
          $060: Spurious interrupt
          $064: IRQ1 (autovector)
          $068: IRQ2 (autovector) — CPS1 VBlank interrupt
          $06C: IRQ3
          ...
        """
        self.initial_sp = struct.unpack('>I', self.data[0x00:0x04])[0]
        self.initial_pc = struct.unpack('>I', self.data[0x04:0x08])[0]

        self.vectors = {}
        vector_names = {
            2: 'bus_error', 3: 'address_error', 4: 'illegal_insn',
            5: 'div_zero', 6: 'chk', 7: 'trapv',
            8: 'privilege_violation', 9: 'trace',
            # CPS1 uses IRQ2 for VBlank
            25: 'irq1', 26: 'irq2_vblank', 27: 'irq3',
            28: 'irq4', 29: 'irq5', 30: 'irq6', 31: 'irq7',
        }
        for idx, name in vector_names.items():
            addr = struct.unpack('>I', self.data[idx*4:(idx+1)*4])[0]
            if addr != 0 and addr < self.size and (addr & 1) == 0:
                self.vectors[name] = addr

    def read8(self, addr):
        if addr < self.size:
            return self.data[addr]
        return 0

    def read16(self, addr):
        if addr + 1 < self.size:
            return struct.unpack('>H', self.data[addr:addr+2])[0]
        return 0

    def read32(self, addr):
        if addr + 3 < self.size:
            return struct.unpack('>I', self.data[addr:addr+4])[0]
        return 0

    def print_info(self):
        print(f"ROM size:    {self.size} bytes ({self.size//1024} KB)")
        print(f"Entry PC:    ${self.initial_pc:06X}")
        print(f"Initial SSP: ${self.initial_sp:08X}")
        print(f"VBlank IRQ:  ${self.vectors.get('irq2_vblank', 0):06X}")
        print(f"Vectors:")
        for name, addr in sorted(self.vectors.items(), key=lambda x: x[1]):
            print(f"  {name:<24s} ${addr:06X}")


# ============================================================
# Recursive descent disassembler + function finder
# (Adapted from pigskin/tools/analyze_rom.py — identical M68K analysis)
# ============================================================

class M68KAnalyzer:
    CALL_MNEMONICS = {'bsr', 'jsr'}
    UNCONDITIONAL_ENDS = {'bra', 'jmp', 'rts', 'rte', 'rtr'}
    BIT_MNEMONICS = {'btst', 'bset', 'bclr', 'bchg'}

    def __init__(self, rom):
        self.rom = rom
        self.cs = Cs(CS_ARCH_M68K, CS_MODE_M68K_000)
        self.cs.detail = True

        self.visited = set()
        self.instructions = {}
        self.functions = {}
        self.call_graph = defaultdict(set)
        self.xrefs_to = defaultdict(set)
        self.labels = {}
        self.jump_tables = {}

    def analyze(self, extra_entries=None):
        """Run full analysis."""
        entry_points = set()
        entry_points.add(self.rom.initial_pc)
        for name, addr in self.rom.vectors.items():
            entry_points.add(addr)
            self.labels[addr] = f"vec_{name}"
        self.labels[self.rom.initial_pc] = "entry_point"

        # Add any extra entry points from file
        if extra_entries:
            entry_points.update(extra_entries)
            print(f"Added {len(extra_entries)} extra entry points")

        # Scan for jump tables
        jt_targets = self._scan_jump_tables()
        entry_points.update(jt_targets)

        print(f"\nStarting analysis from {len(entry_points)} entry points "
              f"(vectors + {len(jt_targets)} jump table targets)...")

        # Multi-pass disassembly
        all_func_entries = set(entry_points)
        work = list(entry_points)

        while work:
            new_work = []
            for addr in work:
                if addr in self.visited or addr >= self.rom.size or addr < 0x200:
                    continue
                if addr & 1:
                    continue
                new_targets = self._disassemble_block(addr)
                for target, is_call in new_targets:
                    if target not in self.visited and 0x200 <= target < self.rom.size and not (target & 1):
                        new_work.append(target)
                        if is_call:
                            all_func_entries.add(target)
            work = new_work

        # Scan for address-loading instructions
        more_targets = self._scan_address_loads()
        if more_targets:
            print(f"  Found {len(more_targets)} additional targets from address loads")
            work = list(more_targets - self.visited)
            all_func_entries.update(more_targets)
            while work:
                new_work = []
                for addr in work:
                    if addr in self.visited or addr >= self.rom.size or addr < 0x200 or (addr & 1):
                        continue
                    new_targets = self._disassemble_block(addr)
                    for target, is_call in new_targets:
                        if target not in self.visited and 0x200 <= target < self.rom.size and not (target & 1):
                            new_work.append(target)
                            if is_call:
                                all_func_entries.add(target)
                work = new_work

        # Pass 3: Scan for functions after RTS/RTE and LINK/MOVEM prologues
        post_rts = self._scan_post_rts_entries()
        prologue_entries = self._scan_prologues()
        extra = (post_rts | prologue_entries) - self.visited
        if extra:
            print(f"  Found {len(extra)} entries from post-RTS + prologue scan")
            all_func_entries.update(extra)
            work = list(extra)
            while work:
                new_work = []
                for addr in work:
                    if addr in self.visited or addr >= self.rom.size or addr < 0x200 or (addr & 1):
                        continue
                    new_targets = self._disassemble_block(addr)
                    for target, is_call in new_targets:
                        if target not in self.visited and 0x200 <= target < self.rom.size and not (target & 1):
                            new_work.append(target)
                            if is_call:
                                all_func_entries.add(target)
                work = new_work

        # Pass 4: Look for call targets that weren't in any discovered function
        call_targets = set()
        for func_entry in list(all_func_entries):
            for call_addr in self.call_graph.get(func_entry, set()):
                if call_addr not in self.visited and 0x200 <= call_addr < self.rom.size and not (call_addr & 1):
                    call_targets.add(call_addr)
        if call_targets:
            print(f"  Found {len(call_targets)} unreached call targets")
            all_func_entries.update(call_targets)
            work = list(call_targets)
            while work:
                new_work = []
                for addr in work:
                    if addr in self.visited or addr >= self.rom.size or addr < 0x200 or (addr & 1):
                        continue
                    new_targets = self._disassemble_block(addr)
                    for target, is_call in new_targets:
                        if target not in self.visited and 0x200 <= target < self.rom.size and not (target & 1):
                            new_work.append(target)
                            if is_call:
                                all_func_entries.add(target)
                work = new_work

        self._build_functions(all_func_entries)

        print(f"Disassembled {len(self.instructions)} instructions")
        print(f"Found {len(self.functions)} functions")

    def _scan_post_rts_entries(self):
        """Scan for functions that start immediately after RTS/RTE instructions."""
        entries = set()
        for addr in sorted(self.instructions.keys()):
            mnemonic = self.instructions[addr][0]
            size = self.instructions[addr][2]
            if mnemonic in ('rts', 'rte', 'rtr'):
                next_addr = addr + size
                # Align to word boundary
                if next_addr & 1:
                    next_addr += 1
                if next_addr not in self.visited and next_addr < self.rom.size:
                    # Validate: try to disassemble a few instructions
                    code = bytes(self.rom.data[next_addr:min(next_addr + 20, self.rom.size)])
                    insns = list(self.cs.disasm(code, next_addr, count=3))
                    if len(insns) >= 2:
                        entries.add(next_addr)
                        if next_addr not in self.labels:
                            self.labels[next_addr] = f"sub_{next_addr:06X}"
        return entries

    def _scan_prologues(self):
        """Scan uncovered ROM regions for common M68K function prologues."""
        entries = set()
        i = 0x200
        while i < self.rom.size - 4:
            if i in self.visited:
                i += 2
                continue
            word = struct.unpack('>H', self.rom.data[i:i+2])[0]
            # LINK A6, #imm16 = $4E56
            # LINK A5, #imm16 = $4E55
            # MOVEM.L reglist, -(A7) = $48E7
            if word in (0x4E56, 0x4E55, 0x48E7):
                # Validate with disassembly
                code = bytes(self.rom.data[i:min(i + 20, self.rom.size)])
                insns = list(self.cs.disasm(code, i, count=3))
                if len(insns) >= 2:
                    entries.add(i)
                    if i not in self.labels:
                        self.labels[i] = f"sub_{i:06X}"
            i += 2
        return entries

    def _scan_jump_tables(self):
        """Scan ROM for potential jump/address tables."""
        targets = set()
        rom = self.rom
        i = 0x200
        while i < rom.size - 12:
            addrs = []
            j = i
            while j < rom.size - 3:
                val = struct.unpack('>I', rom.data[j:j+4])[0]
                if 0x200 <= val < rom.size and (val & 1) == 0:
                    addrs.append(val)
                    j += 4
                else:
                    break
            if len(addrs) >= 3:
                self.jump_tables[i] = addrs
                for a in addrs:
                    targets.add(a)
                    if a not in self.labels:
                        self.labels[a] = f"jt_{a:06X}"
                i = j
            else:
                i += 2

        print(f"Found {len(self.jump_tables)} jump tables with {len(targets)} unique targets")
        return targets

    def _scan_address_loads(self):
        """Scan disassembled code for LEA/MOVE.L #addr patterns."""
        targets = set()
        for addr, (mnemonic, op_str, size, raw) in self.instructions.items():
            if mnemonic in ('lea', 'pea'):
                target = self._parse_absolute_addr(op_str)
                if target and 0x200 <= target < self.rom.size and not (target & 1):
                    targets.add(target)
            elif mnemonic == 'move.l' and '#$' in op_str:
                try:
                    imm_str = op_str.split('#$')[1].split(',')[0].strip()
                    val = int(imm_str, 16)
                    if 0x200 <= val < self.rom.size and not (val & 1):
                        targets.add(val)
                except (ValueError, IndexError):
                    pass
        return targets - self.visited

    def _parse_absolute_addr(self, op_str):
        for part in op_str.replace('(', '').replace(')', '').replace('.l', '').replace('.w', '').split(','):
            part = part.strip()
            if part.startswith('$'):
                try:
                    return int(part[1:], 16)
                except ValueError:
                    pass
        return None

    def _disassemble_block(self, start_addr):
        """Disassemble a basic block. Returns list of (target, is_call)."""
        targets = []
        addr = start_addr

        for _ in range(10000):
            if addr >= self.rom.size or addr in self.visited:
                break
            if addr & 1:
                break

            self.visited.add(addr)
            code = bytes(self.rom.data[addr:min(addr+10, self.rom.size)])
            insns = list(self.cs.disasm(code, addr, count=1))

            if not insns:
                break

            insn = insns[0]
            mnemonic = insn.mnemonic.lower()
            op_str = insn.op_str

            self.instructions[addr] = (mnemonic, op_str, insn.size, code[:insn.size])

            if mnemonic in self.CALL_MNEMONICS:
                target = self._extract_branch_target(insn, addr)
                if target is not None:
                    self.xrefs_to[target].add(addr)
                    self.call_graph[start_addr].add(target)
                    targets.append((target, True))
                    if target not in self.labels:
                        self.labels[target] = f"sub_{target:06X}"
                addr += insn.size
                continue

            elif mnemonic in ('bra', 'jmp'):
                target = self._extract_branch_target(insn, addr)
                if target is not None:
                    self.xrefs_to[target].add(addr)
                    targets.append((target, False))
                break

            elif mnemonic in ('rts', 'rte', 'rtr'):
                break

            elif mnemonic.startswith('b') and mnemonic not in self.BIT_MNEMONICS:
                target = self._extract_branch_target(insn, addr)
                if target is not None:
                    self.xrefs_to[target].add(addr)
                    targets.append((target, False))
                    if target not in self.labels:
                        self.labels[target] = f"loc_{target:06X}"
                addr += insn.size
                continue

            elif mnemonic.startswith('db'):
                target = self._extract_branch_target(insn, addr)
                if target is not None:
                    self.xrefs_to[target].add(addr)
                    targets.append((target, False))
                    if target not in self.labels:
                        self.labels[target] = f"loc_{target:06X}"
                addr += insn.size
                continue

            else:
                addr += insn.size

        return targets

    def _extract_branch_target(self, insn, addr):
        op_str = insn.op_str.strip()

        if ',' in op_str and insn.mnemonic.lower().startswith('db'):
            target_part = op_str.split(',', 1)[1].strip()
            if target_part.startswith('$'):
                try:
                    return int(target_part[1:], 16) & 0xFFFFFF
                except ValueError:
                    pass
            if target_part.startswith('0x'):
                try:
                    return int(target_part, 16) & 0xFFFFFF
                except ValueError:
                    pass

        if op_str.startswith('$'):
            try:
                return int(op_str[1:], 16) & 0xFFFFFF
            except ValueError:
                pass
        if op_str.startswith('0x'):
            try:
                return int(op_str, 16) & 0xFFFFFF
            except ValueError:
                pass

        if '(' in op_str and '$' in op_str:
            inner = op_str.replace('(', '').replace(')', '').replace('.l', '').replace('.w', '').strip()
            if inner.startswith('$'):
                try:
                    return int(inner[1:], 16) & 0xFFFFFF
                except ValueError:
                    pass

        if insn.operands:
            op = insn.operands[0]
            if hasattr(op, 'imm'):
                return op.imm & 0xFFFFFF
            if hasattr(op, 'mem') and hasattr(op.mem, 'disp'):
                if op.mem.base == 0:
                    return op.mem.disp & 0xFFFFFF

        return None

    def _build_functions(self, func_entries):
        all_addrs = sorted(self.instructions.keys())
        if not all_addrs:
            return

        sorted_entries = sorted(func_entries & set(all_addrs))

        for i, entry in enumerate(sorted_entries):
            next_entry = sorted_entries[i + 1] if i + 1 < len(sorted_entries) else self.rom.size

            insn_addrs = []
            for addr in all_addrs:
                if addr < entry:
                    continue
                if addr >= next_entry:
                    break
                insn_addrs.append(addr)

            if not insn_addrs:
                continue

            end_addr = insn_addrs[-1]
            last_mnem = self.instructions[end_addr][0]
            last_size = self.instructions[end_addr][2]

            name = self.labels.get(entry, f"sub_{entry:06X}")

            local_labels = set()
            for ia in insn_addrs:
                for ref_from in self.xrefs_to.get(ia, set()):
                    if entry <= ref_from < next_entry:
                        local_labels.add(ia)

            self.functions[entry] = {
                'name': name,
                'start': entry,
                'end': end_addr + last_size,
                'size': (end_addr + last_size) - entry,
                'insn_count': len(insn_addrs),
                'insn_addrs': insn_addrs,
                'calls': sorted(self.call_graph.get(entry, set())),
                'has_return': last_mnem in ('rts', 'rte', 'rtr'),
                'local_labels': sorted(local_labels),
            }

    def get_disassembly(self, start=None, end=None):
        lines = []
        addrs = sorted(self.instructions.keys())
        if start is not None:
            addrs = [a for a in addrs if a >= start]
        if end is not None:
            addrs = [a for a in addrs if a <= end]

        for addr in addrs:
            mnemonic, op_str, size, raw = self.instructions[addr]
            hex_bytes = ' '.join(f'{b:02X}' for b in raw)

            label = ""
            if addr in self.labels:
                label = f"\n{self.labels[addr]}:\n"

            xref = ""
            if addr in self.xrefs_to and len(self.xrefs_to[addr]) > 0:
                refs = ', '.join(f'${r:06X}' for r in sorted(self.xrefs_to[addr])[:5])
                xref = f"  ; xref: {refs}"

            lines.append(f"{label}  {addr:06X}:  {hex_bytes:<24s}  {mnemonic:<8s} {op_str}{xref}")

        return '\n'.join(lines)

    def export_json(self, path):
        output = {
            'rom': {
                'size': self.rom.size,
                'entry_pc': f"0x{self.rom.initial_pc:06X}",
                'initial_sp': f"0x{self.rom.initial_sp:08X}",
            },
            'vectors': {name: f"0x{addr:06X}" for name, addr in self.rom.vectors.items()},
            'stats': {
                'total_instructions': len(self.instructions),
                'total_functions': len(self.functions),
                'total_xrefs': len(self.xrefs_to),
                'code_coverage_bytes': sum(self.instructions[a][2] for a in self.instructions),
            },
            'functions': {},
        }

        for addr in sorted(self.functions.keys()):
            func = self.functions[addr]
            output['functions'][f"0x{addr:06X}"] = {
                'name': func['name'],
                'start': f"0x{func['start']:06X}",
                'end': f"0x{func['end']:06X}",
                'size': func['size'],
                'insn_count': func['insn_count'],
                'calls': [f"0x{c:06X}" for c in func['calls']],
                'has_return': func['has_return'],
            }

        with open(path, 'w') as f:
            json.dump(output, f, indent=2)
        print(f"\nExported {len(self.functions)} functions to {path}")


def main():
    import argparse
    parser = argparse.ArgumentParser(description='CPS1 SF2 ROM Analyzer')
    parser.add_argument('rom', help='Path to assembled 68K binary (sf2_68k.bin)')
    parser.add_argument('--output', '-o', default='functions.json', help='Output JSON path')
    parser.add_argument('--disasm', '-d', action='store_true', help='Print full disassembly')
    parser.add_argument('--disasm-func', type=str, help='Disassemble specific function (hex addr)')
    parser.add_argument('--stats', action='store_true', help='Print statistics')
    parser.add_argument('--extra-entries', type=str, help='File with extra entry points (hex, one per line)')
    args = parser.parse_args()

    print("=== CPS1 SF2 ROM Analyzer ===\n")

    rom = CPS1ROM(args.rom)
    rom.print_info()

    extra = set()
    if args.extra_entries and os.path.exists(args.extra_entries):
        with open(args.extra_entries) as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith('#'):
                    extra.add(int(line, 16))

    analyzer = M68KAnalyzer(rom)
    analyzer.analyze(extra_entries=extra if extra else None)

    if args.stats:
        print("\n=== Top 20 Largest Functions ===")
        funcs_by_size = sorted(analyzer.functions.values(), key=lambda f: f['insn_count'], reverse=True)
        for f in funcs_by_size[:20]:
            print(f"  {f['name']:<30s}  ${f['start']:06X}  {f['insn_count']:5d} insns  {f['size']:5d} bytes")

        total_bytes = sum(analyzer.instructions[a][2] for a in analyzer.instructions)
        print(f"\n=== Coverage ===")
        print(f"  Code bytes discovered: {total_bytes} ({total_bytes/1024:.1f} KB)")
        print(f"  ROM utilization:       {total_bytes*100/rom.size:.1f}%")

    if args.disasm:
        print("\n=== Full Disassembly ===")
        print(analyzer.get_disassembly())

    if args.disasm_func:
        addr = int(args.disasm_func, 16)
        if addr in analyzer.functions:
            func = analyzer.functions[addr]
            print(f"\n=== {func['name']} (${func['start']:06X} - ${func['end']:06X}) ===")
            print(analyzer.get_disassembly(func['start'], func['end']))
        else:
            print(f"No function at ${addr:06X}")

    analyzer.export_json(args.output)


if __name__ == '__main__':
    main()
