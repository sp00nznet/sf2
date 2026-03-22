/* Auto-generated recompiled code for Street Fighter II */
/* Source range: $000400 - $000B62 */
/* Functions: 50 */

#include <cps1recomp/cps1recomp.h>
#include "recomp_funcs.h"
#include "../task_fiber.h"
#include <stdio.h>

/* $000400-$000402  (1 instructions, 2 bytes) */
void vec_address_error(void) {
    g_m68k.a[7] += 0x8;
    /* WARNING: function did not end with RTS */
}

/* $000402-$00040C  (2 instructions, 10 bytes) */
void vec_trace(void) {
    g_m68k.a[7] = (g_m68k.a[5] + 0x0);
    { func_table_call(0x00040E); return; }
    /* WARNING: function did not end with RTS */
}

/* $00040C-$00040E  (1 instructions, 2 bytes) */
void vec_irq7(void) {
    return; /* RTE */
}

/* $00040E-$00041E  (2 instructions, 16 bytes) */
void entry_point(void) {
    { uint8_t _mv = (uint8_t)(0x80); bus_write8(0x800030, _mv); M68K_TST8(_mv); }
    { uint8_t _mv = (uint8_t)(0x0); bus_write8(0x800030, _mv); M68K_TST8(_mv); }
    /* WARNING: function did not end with RTS */
}

/* $00041C-$000426  (3 instructions, 10 bytes) */
void jt_00041C(void) {
    { uint32_t _ea = (g_m68k.a[0] + 0x6 + (int16_t)(uint16_t)g_m68k.d[0]); uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0xfa); bus_write8(_ea, _tmp); }
    g_m68k.a[4] = 0x426;
    func_table_call(0x00073C); /* LEA+BRA call to loc_00073C */
    /* WARNING: function did not end with RTS */
}

/* $000426-$00044E  (6 instructions, 40 bytes) */
void sub_000426(void) {
    g_m68k.a[4] = 0x42e;
    func_table_call(0x0007CC); /* LEA+BRA call to loc_0007CC */
    { uint8_t _mv = (uint8_t)(0xf0); bus_write8(0x800181, _mv); M68K_TST8(_mv); }
    { uint16_t _mv = (uint16_t)(0xffc0); bus_write16(0x80010C, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x0); bus_write16(0x80010E, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9100); bus_write16(0x800100, _mv); M68K_TST16(_mv); }
    /* WARNING: function did not end with RTS */
}

/* $000448-$00047A  (18 instructions, 50 bytes) */
void jt_000448(void) {
    M68K_SUBX8(g_m68k.d[0], g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0x10033fc);
    { uint16_t _mv = (uint16_t)(0x90c0); bus_write16(0x800102, _mv); M68K_TST16(_mv); }
    g_m68k.a[0] -= (int16_t)(uint16_t)(g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0x10233fc);
    { uint16_t _mv = (uint16_t)(0x9040); bus_write16(0x800104, _mv); M68K_TST16(_mv); }
    M68K_SUB16(g_m68k.d[0], g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0x10433fc);
    { uint16_t _mv = (uint16_t)(0x9080); bus_write16(0x800106, _mv); M68K_TST16(_mv); }
    M68K_SUB32(g_m68k.d[0], g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0x10633fc);
    { uint16_t _mv = (uint16_t)(0x9200); bus_write16(0x800108, _mv); M68K_TST16(_mv); }
    M68K_SUB8(g_m68k.d[1], g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0x10849fa);
    g_m68k.a[4] = 0x476;
    M68K_OR8(g_m68k.d[6], 0x0);
    { func_table_call(0x0007B0); return; }
    M68K_BTST(bus_read8(/* UNHANDLED_ADDR: #$49fa0006 */ 0), g_m68k.d[1]);
    /* WARNING: function did not end with RTS */
}

/* $000476-$00047E  (2 instructions, 8 bytes) */
void jt_000476(void) {
    g_m68k.a[4] = 0x47e;
    func_table_call(0x000768); /* LEA+BRA call to loc_000768 */
    /* WARNING: function did not end with RTS */
}

/* $00047E-$000486  (2 instructions, 8 bytes) */
void sub_00047E(void) {
    g_m68k.a[4] = 0x486;
    func_table_call(0x00077E); /* LEA+BRA call to loc_00077E */
    /* WARNING: function did not end with RTS */
}

/* $000486-$000502  (26 instructions, 124 bytes) */
void sub_000486(void) {
    g_m68k.a[4] = 0x48e;
    func_table_call(0x000794); /* LEA+BRA call to loc_000794 */
    { uint16_t _mv = (uint16_t)(0x12c8); bus_write16(0x800154, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x3e); bus_write16(0x800122, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x3f); bus_write16(0x80014A, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9000); bus_write16(0x80010A, _mv); M68K_TST16(_mv); }
    g_m68k.a[2] = 0x828;
    g_m68k.a[5] = 0x4ba;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x4000)); M68K_TST16((uint16_t)g_m68k.d[0]);
loc_0004BE:
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--; g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_0004BE; } }
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(bus_read16(0x800148))); M68K_TST16((uint16_t)g_m68k.d[0]);
    M68K_AND16(g_m68k.d[0], 0xfc3f);
    M68K_CMP16(g_m68k.d[0], 0x407);
    if (M68K_CC_NE) { func_table_call(0x0006F0); return; }
    g_m68k.a[2] = 0x843;
    g_m68k.a[5] = 0x4e0;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[2] = 0x86d;
    g_m68k.a[5] = 0x4ec;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[0] = 0x90c000;
    g_m68k.a[1] = g_m68k.a[0];
    g_m68k.a[1] += 0x3ffe;
    g_m68k.a[2] = 0x877;
    g_m68k.a[6] = 0x506;
    /* WARNING: function did not end with RTS */
}

/* $000500-$000512  (6 instructions, 18 bytes) */
void jt_000500(void) {
    M68K_OR8(g_m68k.d[6], 0x0);
    { func_table_call(0x0006CA); return; }
    M68K_BSET(g_m68k.d[6], g_m68k.d[0]);
    g_m68k.a[2] = 0x882;
    g_m68k.a[5] = 0x512;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    /* WARNING: function did not end with RTS */
}

/* $000512-$00051E  (3 instructions, 12 bytes) */
void jt_000512(void) {
    g_m68k.a[2] = 0x88d;
    g_m68k.a[5] = 0x51e;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    /* WARNING: function did not end with RTS */
}

/* $00051E-$000538  (6 instructions, 26 bytes) */
void sub_00051E(void) {
    g_m68k.a[0] = 0x904000;
    g_m68k.a[1] = g_m68k.a[0];
    g_m68k.a[1] += 0x3ffe;
    g_m68k.a[2] = 0x898;
    g_m68k.a[6] = 0x538;
    func_table_call(0x0006CA); /* LEA+BRA call to loc_0006CA */
    /* WARNING: function did not end with RTS */
}

/* $000538-$000544  (3 instructions, 12 bytes) */
void sub_000538(void) {
    g_m68k.a[2] = 0x8a3;
    g_m68k.a[5] = 0x544;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    /* WARNING: function did not end with RTS */
}

/* $000544-$00054C  (2 instructions, 8 bytes) */
void sub_000544(void) {
    g_m68k.a[4] = 0x54c;
    func_table_call(0x00077E); /* LEA+BRA call to loc_00077E */
    /* WARNING: function did not end with RTS */
}

/* $00054C-$0006A6  (75 instructions, 346 bytes) */
void sub_00054C(void) {
    g_m68k.a[2] = 0x8ae;
    g_m68k.a[5] = 0x558;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[0] = 0x908000;
    g_m68k.a[1] = g_m68k.a[0];
    g_m68k.a[1] += 0x3ffe;
    g_m68k.a[2] = 0x8b8;
    g_m68k.a[6] = 0x572;
    func_table_call(0x0006CA); /* LEA+BRA call to loc_0006CA */
    g_m68k.a[2] = 0x8c3;
    g_m68k.a[5] = 0x57e;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[4] = 0x586;
    func_table_call(0x000794); /* LEA+BRA call to loc_000794 */
    g_m68k.a[2] = 0x8ce;
    g_m68k.a[5] = 0x592;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[0] = 0x910000;
    g_m68k.a[1] = g_m68k.a[0];
    g_m68k.a[1] += 0x7ffe;
    g_m68k.a[2] = 0x8d9;
    g_m68k.a[6] = 0x5ac;
    func_table_call(0x0006CA); /* LEA+BRA call to loc_0006CA */
    g_m68k.a[2] = 0x8e4;
    g_m68k.a[5] = 0x5b8;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[2] = 0x84e;
    g_m68k.a[5] = 0x5c4;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[0] = 0xff0000;
    g_m68k.a[1] = g_m68k.a[0];
    g_m68k.a[1] += 0xfffe;
    g_m68k.a[2] = 0x857;
    g_m68k.a[6] = 0x5de;
    func_table_call(0x0006CA); /* LEA+BRA call to loc_0006CA */
    g_m68k.a[2] = 0x862;
    g_m68k.a[5] = 0x5ea;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
    g_m68k.a[4] = 0x5f2;
    func_table_call(0x0007B0); /* LEA+BRA call to loc_0007B0 */
    g_m68k.a[4] = 0x5fa;
    func_table_call(0x000752); /* LEA+BRA call to loc_000752 */
    g_m68k.a[5] = 0xFF8000;
    { uint16_t _mv = (uint16_t)(0x3f); bus_write16(0x800122, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x3f); bus_write16(g_m68k.a[5] + 0x4c, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x12da); bus_write16(g_m68k.a[5] + 0x52, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x3f); bus_write16(g_m68k.a[5] + 0x5c, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9000); bus_write16(g_m68k.a[5] + 0x34, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9100); bus_write16(0x800100, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9100); bus_write16(g_m68k.a[5] + 0x2a, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x90c0); bus_write16(0x800102, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x90c0); bus_write16(g_m68k.a[5] + 0x2c, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9040); bus_write16(0x800104, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9040); bus_write16(g_m68k.a[5] + 0x2e, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9080); bus_write16(0x800106, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9080); bus_write16(g_m68k.a[5] + 0x30, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9200); bus_write16(0x800108, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(0x9200); bus_write16(g_m68k.a[5] + 0x32, _mv); M68K_TST16(_mv); }
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x0)); M68K_TST16((uint16_t)g_m68k.d[0]);
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x36, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x60, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x3a, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x64, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x3e, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x68, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x42, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x6c, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x46, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x70, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x4a, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x74, _mv); M68K_TST16(_mv); }
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x100)); M68K_TST16((uint16_t)g_m68k.d[0]);
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x38, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x62, _mv); M68K_TST16(_mv); }
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $0006A6-$0006CA  (9 instructions, 36 bytes) */
void jt_0006A6(void) {
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x3c, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x66, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x40, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x6a, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x44, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x6e, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x48, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x72, _mv); M68K_TST16(_mv); }
    { func_table_call(0x000910); return; }
    /* WARNING: function did not end with RTS */
}

/* $0006CA-$000702  (20 instructions, 56 bytes) */
void loc_0006CA(void) {
    g_m68k.a[3] = g_m68k.a[0];
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | ((uint16_t)(0x3)); M68K_TST16((uint16_t)g_m68k.d[1]);
    g_m68k.d[3] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[3]);
loc_0006D2:
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(bus_read16(0x6fe + (int16_t)(uint16_t)g_m68k.d[3]))); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.a[0] = g_m68k.a[3];
loc_0006D8:
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(bus_read16(g_m68k.a[0]))); M68K_TST16((uint16_t)g_m68k.d[2]);
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[0], _mv); M68K_TST16(_mv); }
    M68K_CMP16(g_m68k.d[0], bus_read16(g_m68k.a[0]));
    if (M68K_CC_NE) goto loc_0006F4;
    { uint16_t _mv = (uint16_t)(g_m68k.d[2]); bus_write16(g_m68k.a[0], _mv); g_m68k.a[0] += 2; M68K_TST16(_mv); }
    M68K_CMP32(g_m68k.a[0], g_m68k.a[1]);
    if (M68K_CC_LS) goto loc_0006D8;
    M68K_ADD16(g_m68k.d[3], 0x2);
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[1]; _cnt--; g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_0006D2; } }
    return; /* JMP (a6) = return from LEA+BRA call */
loc_0006F0:
    g_m68k.a[2] = 0x830;
loc_0006F4:
    g_m68k.a[5] = 0x6fc;
    func_table_call(0x000706); /* LEA+BRA call to loc_000706 */
loc_0006FC:
    goto loc_0006FC;
    M68K_OR8(g_m68k.d[0], 0x55);
    /* WARNING: function did not end with RTS */
}

/* $000706-$00073C  (21 instructions, 54 bytes) */
void loc_000706(void) {
    g_m68k.a[0] = g_m68k.a[2];
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(_postinc8(0))); M68K_TST8((uint8_t)g_m68k.d[0]);
    M68K_LSL16(g_m68k.d[0], 0x7);
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFFFF00u) | ((uint8_t)(_postinc8(0))); M68K_TST8((uint8_t)g_m68k.d[1]);
    M68K_LSL16(g_m68k.d[1], 0x2);
    M68K_ADD16(g_m68k.d[0], g_m68k.d[1]);
    g_m68k.a[1] = 0x90c000;
    g_m68k.a[1] += g_m68k.d[0];
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(_postinc8(0))); M68K_TST8((uint8_t)g_m68k.d[0]);
loc_000722:
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFFFF00u) | ((uint8_t)(_postinc8(0))); M68K_TST8((uint8_t)g_m68k.d[1]);
    if (M68K_CC_EQ) goto loc_00073A;
    M68K_ADD16(g_m68k.d[1], 0x4000);
    { uint16_t _mv = (uint16_t)(g_m68k.d[1]); bus_write16(g_m68k.a[1] + 0x0, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[1] + 0x2, _mv); M68K_TST16(_mv); }
    g_m68k.a[1] = (g_m68k.a[1] + 0x80);
    goto loc_000722;
loc_00073A:
    return; /* JMP (a5) = return from LEA+BRA call */
    /* WARNING: function did not end with RTS */
}

/* $00073C-$000768  (13 instructions, 44 bytes) */
void loc_00073C(void) {
    g_m68k.a[0] = 0x900000;
    g_m68k.a[1] = 0x903FFF;
loc_000748:
    { uint16_t _mv = (uint16_t)(0x0); bus_write16(g_m68k.a[0], _mv); g_m68k.a[0] += 2; M68K_TST16(_mv); }
    M68K_CMP32(g_m68k.a[0], g_m68k.a[1]);
    if (M68K_CC_LS) goto loc_000748;
    return; /* JMP (a4) = return from LEA+BRA call */
loc_000752:
    g_m68k.a[0] = 0xFF0000;
    g_m68k.d[4] = (g_m68k.d[4] & 0xFFFF0000u) | ((uint16_t)(0x1fff)); M68K_TST16((uint16_t)g_m68k.d[4]);
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
loc_00075E:
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[4]; _cnt--; g_m68k.d[4] = (g_m68k.d[4] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_00075E; } }
    return; /* JMP (a4) = return from LEA+BRA call */
    /* WARNING: function did not end with RTS */
}

/* $000768-$00077E  (5 instructions, 22 bytes) */
void loc_000768(void) {
    g_m68k.a[0] = 0x90C000;
    g_m68k.a[1] = 0x90FFFF;
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x4020)); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    { func_table_call(0x0007A6); return; }
    /* WARNING: function did not end with RTS */
}

/* $00077E-$000794  (5 instructions, 22 bytes) */
void loc_00077E(void) {
    g_m68k.a[0] = 0x904000;
    g_m68k.a[1] = 0x907FFF;
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x2800)); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    { func_table_call(0x0007A6); return; }
    /* WARNING: function did not end with RTS */
}

/* $000794-$0007A6  (4 instructions, 18 bytes) */
void loc_000794(void) {
    g_m68k.a[0] = 0x908000;
    g_m68k.a[1] = 0x90BFFF;
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x400)); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    /* WARNING: function did not end with RTS */
}

/* $0007A6-$0007B0  (5 instructions, 10 bytes) */
void loc_0007A6(void) {
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[0], _mv); g_m68k.a[0] += 2; M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[1]); bus_write16(g_m68k.a[0], _mv); g_m68k.a[0] += 2; M68K_TST16(_mv); }
    M68K_CMP32(g_m68k.a[0], g_m68k.a[1]);
    if (M68K_CC_LS) { func_table_call(0x0007A6); return; }
    return; /* JMP (a4) = return from LEA+BRA call */
    /* WARNING: function did not end with RTS */
}

/* $0007B0-$0007C2  (5 instructions, 18 bytes) */
void loc_0007B0(void) {
    g_m68k.a[0] = 0x910000;
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x0)); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    g_m68k.d[4] = (g_m68k.d[4] & 0xFFFF0000u) | ((uint16_t)(0xff)); M68K_TST16((uint16_t)g_m68k.d[4]);
loc_0007C0:
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[0], _mv); g_m68k.a[0] += 2; M68K_TST16(_mv); }
    /* WARNING: function did not end with RTS */
}

/* $0007C2-$0007CC  (4 instructions, 10 bytes) */
void jt_0007C2(void) {
    { uint16_t _mv = (uint16_t)(g_m68k.d[1]); bus_write16(g_m68k.a[0], _mv); g_m68k.a[0] += 2; M68K_TST16(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[1]); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[4]; _cnt--; g_m68k.d[4] = (g_m68k.d[4] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { func_table_call(0x0007C0); return; } }
    return; /* JMP (a4) = return from LEA+BRA call */
    /* WARNING: function did not end with RTS */
}

/* $0007CC-$000800  (15 instructions, 52 bytes) */
void loc_0007CC(void) {
    g_m68k.a[0] = 0x8f0;
    g_m68k.a[1] = 0x900400;
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $000800-$000806  (1 instructions, 6 bytes) */
void jt_000800(void) {
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    /* WARNING: function did not end with RTS */
}

/* $000806-$000828  (11 instructions, 34 bytes) */
void jt_000806(void) {
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.d[0] = _postinc32(0); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_OR32(g_m68k.d[0], 0xf000f000);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    return; /* JMP (a4) = return from LEA+BRA call */
    /* WARNING: function did not end with RTS */
}

/* $0008E0-$0008E4  (2 instructions, 4 bytes) */
void jt_0008E0(void) {
    /* TRAP #0x7 */
    g_m68k.d[0] = g_m68k.d[0]; M68K_TST32((uint32_t)g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $000910-$000A68  (87 instructions, 344 bytes) */
void sub_000910(void) {
    g_m68k.a[7] = 0x000000;
    g_m68k.a[5] = 0xFF8000;
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x24, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x22, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x20, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[5] + 0x1e, _mv); M68K_TST16(_mv); }
    g_m68k.a[0] = (g_m68k.a[5] + 0xa2);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x1f)); M68K_TST16((uint16_t)g_m68k.d[0]);
loc_000932:
    { uint32_t _mv = (uint32_t)(0xffffffff); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(0xffffffff); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--; g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_000932; } }
    g_m68k.a[0] = (g_m68k.a[5] + 0x1a2);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x1f)); M68K_TST16((uint16_t)g_m68k.d[0]);
loc_00094A:
    { uint32_t _mv = (uint32_t)(0x0); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(0x0); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--; g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_00094A; } }
    { uint16_t _mv = (uint16_t)(0x2); bus_write16(g_m68k.a[5] + 0x2c2, _mv); M68K_TST16(_mv); }
    func_table_call(0x0015D4);
    func_table_call(0x00162C);
    func_table_call(0x001C4E); /* loc_001C4E */
    func_table_call(0x001D8E); /* loc_001D8E */
    bus_write8(g_m68k.a[5] + 0x322, 0); g_m68k.flag_n = false; g_m68k.flag_z = true; g_m68k.flag_v = false; g_m68k.flag_c = false;
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(0x800000))); M68K_TST8((uint8_t)g_m68k.d[0]);
    M68K_NOT8(g_m68k.d[0]);
    M68K_CMP8(g_m68k.d[0], 0x50);
    if (M68K_CC_NE) goto loc_000988;
    { uint8_t _mv = (uint8_t)(0x1); bus_write8(g_m68k.a[5] + 0x322, _mv); M68K_TST8(_mv); }
loc_000988:
    g_m68k.a[0] = 0xd44;
    g_m68k.a[6] = (g_m68k.a[5] + (-0x8000));
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0xf)); M68K_TST16((uint16_t)g_m68k.d[7]);
loc_000994:
    { uint32_t _mv = (uint32_t)(_postinc32(0)); bus_write32(g_m68k.a[6] + 0xc, _mv); M68K_TST32(_mv); }
    g_m68k.a[6] = (g_m68k.a[6] + 0x20);
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[7]; _cnt--; g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_000994; } }
    g_m68k.a[0] = (g_m68k.a[5] + 0x528a);
    g_m68k.a[1] = (g_m68k.a[5] + 0x52da);
    g_m68k.d[6] = (uint32_t)(int32_t)(int8_t)(0x5); M68K_TST32(g_m68k.d[6]);
loc_0009AA:
    { uint32_t _mv = (uint32_t)(g_m68k.a[0]); bus_write32(g_m68k.a[1], _mv); g_m68k.a[1] += 4; M68K_TST32(_mv); }
    g_m68k.a[0] = (g_m68k.a[0] + 0x8);
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[6]; _cnt--; g_m68k.d[6] = (g_m68k.d[6] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_0009AA; } }
    g_m68k.a[0] = (g_m68k.a[5] + 0x528a);
    g_m68k.a[1] = 0xda4;
    { uint32_t _mv = (uint32_t)(bus_read32(g_m68k.a[1])); bus_write32(g_m68k.a[5] + 0x5302, _mv); M68K_TST32(_mv); }
    g_m68k.d[6] = (uint32_t)(int32_t)(int8_t)(0x5); M68K_TST32(g_m68k.d[6]);
loc_0009C2:
    { uint32_t _mv = (uint32_t)(_postinc32(1)); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(_postinc32(1)); bus_write32(g_m68k.a[0], _mv); g_m68k.a[0] += 4; M68K_TST32(_mv); }
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[6]; _cnt--; g_m68k.d[6] = (g_m68k.d[6] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_0009C2; } }
    g_m68k.a[0] = 0xd84;
    g_m68k.a[6] = (g_m68k.a[5] + (-0x75d0));
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x7)); M68K_TST16((uint16_t)g_m68k.d[7]);
loc_0009D6:
    { uint32_t _mv = (uint32_t)(_postinc32(0)); g_m68k.a[6] -= 4; bus_write32(g_m68k.a[6], _mv); M68K_TST32(_mv); }
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[7]; _cnt--; g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_0009D6; } }
    { uint32_t _mv = (uint32_t)(g_m68k.a[6]); bus_write32(g_m68k.a[5] + (-0x7df8), _mv); M68K_TST32(_mv); }
    { uint16_t _mv = (uint16_t)(0x8); bus_write16(g_m68k.a[5] + (-0x7df4), _mv); M68K_TST16(_mv); }
    { uint8_t _mv = (uint8_t)(0x0); bus_write8(g_m68k.a[5] + 0x307, _mv); M68K_TST8(_mv); }
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x0)); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.a[0] = 0x639e;
    func_table_call(0x000B20);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0xc0)); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.a[0] = 0x14f2;
    func_table_call(0x000B20);
    m68k_set_sr(0x2000);
loc_000A0C:
    /* --- Frame boundary ---
       Original code polls the vblank flag to detect async interrupts.
       In recompiled code, reading the flag triggers the vblank hook
       synchronously (one frame render+present).  We fire it once at the
       top, then scan all 16 task slots before the next frame. */
    bus_write8(g_m68k.a[5] + (-0x7df2), 0);  /* clear vblank flag */
    g_m68k.flag_n = false; g_m68k.flag_z = true;
    g_m68k.flag_v = false; g_m68k.flag_c = false;

    /* Trigger vblank: reading the flag fires the hook (renders one frame).
       The ISR (vec_irq2_vblank) updates task sleep counters. */
    m68k_set_sr(0x2600);
    (void)bus_read8(g_m68k.a[5] + (-0x7df2));
    /* The vblank hook renders a frame but does NOT call the 68k VBlank ISR.
       Call it explicitly — the ISR updates CPS registers, scans task slots
       to decrement sleep counters (status 1→4), and sets the vblank flag. */
    vec_irq2_vblank();

    /* Set up task scan */
    g_m68k.a[0] = (g_m68k.a[5] + (-0x8000));
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0xf));
    M68K_TST16((uint16_t)g_m68k.d[0]);

loc_000A18:
    /* Scan task slots (no vblank check mid-scan) */
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[0] + 0x0)));
    M68K_TST8((uint8_t)g_m68k.d[1]);
    M68K_CMP8(g_m68k.d[1], 0x4);
    if (M68K_CC_CC) goto loc_000A3A;
    m68k_set_sr(0x2000);
    g_m68k.a[0] = (g_m68k.a[0] + 0x20);
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--; g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_000A18; } }
    goto loc_000A0C;
loc_000A3A:
    /* Save main loop context on 68k stack */
    g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], 0xa2c);
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); g_m68k.a[7] -= 2; bus_write16(g_m68k.a[7], _mv); M68K_TST16(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.a[0]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.a[7]); bus_write32(g_m68k.a[5] + (-0x7e00), _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.a[0]); bus_write32(g_m68k.a[5] + (-0x7dfc), _mv); M68K_TST32(_mv); }

    /* --- Fiber-based task dispatch --- */
    {
        uint32_t task_base = g_m68k.a[5] - 0x8000;
        int slot = (int)(g_m68k.a[0] - task_base) / TASK_SLOT_SIZE;
        uint8_t status = (uint8_t)g_m68k.d[1];

        if (status >= 0x0C) {
            /* New task (status 0x0C): create fiber, run from code address */
            uint32_t code_addr = bus_read32(g_m68k.a[0] + 4);
            task_fiber_create(slot, code_addr);
            bus_write8(g_m68k.a[0] + 0x0, 0x08);  /* mark as running */
            task_fiber_switch_to(slot);
        } else if (task_fiber_exists(slot)) {
            /* Resume existing fiber (status 0x04/0x08) */
            bus_write8(g_m68k.a[0] + 0x0, 0x08);  /* mark as running */
            task_fiber_switch_to(slot);
        }
        /* else: no fiber — skip (shouldn't normally happen) */
    }

    /* loc_000A5E: Restore main loop context after fiber yields/terminates */
    g_m68k.a[7] = bus_read32(g_m68k.a[5] + (-0x7e00));
    g_m68k.a[0] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4;
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(bus_read16(g_m68k.a[7]))); g_m68k.a[7] += 2;
    g_m68k.a[7] += 4;  /* pop pseudo return address (0xA2C) */

    /* Continue scanning remaining task slots (replaces the `return;`) */
    m68k_set_sr(0x2000);
    g_m68k.a[0] = (g_m68k.a[0] + 0x20);
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--; g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_000A18; } }
    goto loc_000A0C;
}

/* $000A68-$000A6E  (1 instructions, 6 bytes) */
void jt_000A68(void) {
    { uint8_t _mv = (uint8_t)(0x8); bus_write8(g_m68k.a[0] + 0x0, _mv); M68K_TST8(_mv); }
    /* WARNING: function did not end with RTS */
}

/* $000A6E-$000A7E  (5 instructions, 16 bytes) */
void jt_000A6E(void) {
    { uint32_t _mv = (uint32_t)(bus_read32(g_m68k.a[0] + 0x4)); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], _mv); M68K_TST32(_mv); }
    { uint16_t _mv = (uint16_t)(bus_read16(g_m68k.a[0] + 0x2)); g_m68k.a[7] -= 2; bus_write16(g_m68k.a[7], _mv); M68K_TST16(_mv); }
    g_m68k.a[1] = bus_read32(g_m68k.a[0] + 0x8);
    g_m68k.usp = (g_m68k.usp & 0xFFFF0000u) | ((uint16_t)(g_m68k.a[1])); M68K_TST16((uint16_t)g_m68k.usp);
    return; /* RTE */
}

/* $000A7E-$000A94  (6 instructions, 22 bytes) */
void jt_000A7E(void) {
    { uint8_t _mv = (uint8_t)(0x8); bus_write8(g_m68k.a[0] + 0x0, _mv); M68K_TST8(_mv); }
    g_m68k.a[1] = bus_read32(g_m68k.a[0] + 0x4);
    g_m68k.a[4] = bus_read32(g_m68k.a[0] + 0xc);
    g_m68k.usp = (g_m68k.usp & 0xFFFF0000u) | ((uint16_t)(g_m68k.a[4])); M68K_TST16((uint16_t)g_m68k.usp);
    m68k_set_sr(0x0);
    { func_table_call(g_m68k.a[1]); return; }
    /* WARNING: function did not end with RTS */
}

/* $000A94-$000B06  (26 instructions, 114 bytes) */
void vec_irq2_vblank(void) {
    g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[6]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[5]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[4]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[3]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[2]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[1]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[0]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[7]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[6]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[5]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[4]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[3]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[2]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[1]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[0]);
    g_m68k.a[5] = 0xFF8000;
    { uint16_t _mv = (uint16_t)(bus_read16(g_m68k.a[5] + 0x2a)); bus_write16(0x800100, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(bus_read16(g_m68k.a[5] + 0x32)); bus_write16(0x800108, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(bus_read16(0x800148)); bus_write16(g_m68k.a[5] + 0x5e, _mv); M68K_TST16(_mv); }
    func_table_call(0x001BAA); /* loc_001BAA */
    func_table_call(0x000B06); /* jt_000B06 */
    func_table_call(0x001B14);
    func_table_call(0x00626C);
    func_table_call(0x001ED0); /* loc_001ED0 */
    func_table_call(0x001FE2); /* loc_001FE2 */
    func_table_call(0x001D72); /* loc_001D72 */
    { uint32_t _ea = (g_m68k.a[5] + 0x1c); uint8_t _tmp = bus_read8(_ea); M68K_ADD8(_tmp, 0x1); bus_write8(_ea, _tmp); }
    bus_write8((g_m68k.a[5] + (-0x7df2)), M68K_CC_T ? 0xFF : 0x00);
    bus_write8(g_m68k.a[5] + 0x2e2, 0); g_m68k.flag_n = false; g_m68k.flag_z = true; g_m68k.flag_v = false; g_m68k.flag_c = false;
    g_m68k.a[0] = (g_m68k.a[5] + (-0x8000));
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0xf)); M68K_TST16((uint16_t)g_m68k.d[0]);
loc_000AE4:
    M68K_CMP8(bus_read8(g_m68k.a[0] + 0x0), 0x1);
    if (M68K_CC_NE) goto loc_000AF8;
    { uint32_t _ea = (g_m68k.a[0] + 0x1); uint8_t _tmp = bus_read8(_ea); M68K_SUB8(_tmp, 0x1); bus_write8(_ea, _tmp); }
    if (M68K_CC_NE) goto loc_000AF8;
    { uint8_t _mv = (uint8_t)(0x4); bus_write8(g_m68k.a[0] + 0x0, _mv); M68K_TST8(_mv); }
loc_000AF8:
    g_m68k.a[0] = (g_m68k.a[0] + 0x20);
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--; g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_000AE4; } }
    g_m68k.d[0] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[1] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[2] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[3] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[4] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[5] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[6] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[7] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[0] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[1] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[2] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[3] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[4] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[5] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[6] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4;
    return; /* RTE */
}

/* $000B06-$000B0E  (1 instructions, 8 bytes) */
void jt_000B06(void) {
    { uint16_t _mv = (uint16_t)(bus_read16(g_m68k.a[5] + 0x5c)); bus_write16(0x80014A, _mv); M68K_TST16(_mv); }
    /* WARNING: function did not end with RTS */
}

/* $000B08-$000B0C  (1 instructions, 4 bytes) */
void jt_000B08(void) {
    { uint32_t _ea = g_m68k.a[4]; uint16_t _tmp = bus_read16(_ea); M68K_OR16(_tmp, 0x80); bus_write16(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $000B0A-$000B10  (1 instructions, 6 bytes) */
void jt_000B0A(void) {
    M68K_OR32(g_m68k.d[0], 0x14a33ed);
    /* WARNING: function did not end with RTS */
}

/* $000B0C-$000B18  (3 instructions, 12 bytes) */
void jt_000B0C(void) {
    /* TODO $000B0C: movep.l $33ed(a2), d0  [01 4A 33 ED] */
    { uint16_t _mv = (uint16_t)(bus_read16(g_m68k.a[5] + 0x34)); bus_write16(0x80010A, _mv); M68K_TST16(_mv); }
    { uint32_t _ea = /* UNHANDLED_ADDR: ([a4, d0.w], $303c) */ 0; uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0x80); bus_write8(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $000B16-$000B1A  (1 instructions, 4 bytes) */
void jt_000B16(void) {
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0x50)); M68K_TST16((uint16_t)g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $000B18-$000B1C  (1 instructions, 4 bytes) */
void jt_000B18(void) {
    { uint32_t _ea = g_m68k.a[0]; uint16_t _tmp = bus_read16(_ea); M68K_OR16(_tmp, 0x51c8); bus_write16(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $000B1A-$000B20  (2 instructions, 6 bytes) */
void jt_000B1A(void) {
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--; g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { { func_table_call(0x000B1A); return; } } }
    return;
}

/* $000B20-$000B24  (2 instructions, 4 bytes) */
/* TRAP #0: Install task at slot offset D0.
   Handler at $B24-$B40: LEA -$8000(A5),A1; TST.B (A1,D0); BNE skip;
   MOVE.W #$C00,(A1,D0); MOVE.L A0,4(A1,D0); MOVE.W D1,$10(A1,D0);
   MOVE.W D2,$12(A1,D0); RTE */
void sub_000B20(void) {
    uint32_t task_base = g_m68k.a[5] - 0x8000;
    int16_t slot_off = (int16_t)(uint16_t)g_m68k.d[0];
    /* Check if slot is free */
    M68K_TST8(bus_read8(task_base + slot_off));
    if (M68K_CC_NE) return;  /* Slot occupied */
    /* Install task */
    bus_write16(task_base + slot_off + 0x00, 0x0C00);       /* status = new task */
    bus_write32(task_base + slot_off + 0x04, g_m68k.a[0]);  /* code address */
    bus_write16(task_base + slot_off + 0x10, (uint16_t)g_m68k.d[1]); /* param */
    bus_write16(task_base + slot_off + 0x12, (uint16_t)g_m68k.d[2]); /* param */
    printf("[task] TRAP#0: installed task $%06X at slot offset 0x%X\n",
           g_m68k.a[0], (unsigned)slot_off);
    fflush(stdout);
}

/* $000B24-$000B28  (1 instructions, 4 bytes) */
void sub_000B24(void) {
    g_m68k.a[1] = (g_m68k.a[5] + (-0x8000));
    /* WARNING: function did not end with RTS */
}

/* $000B26-$000B28  (1 instructions, 2 bytes) */
void jt_000B26(void) {
    M68K_OR8(g_m68k.d[0], g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $000B28-$000B2C  (1 instructions, 4 bytes) */
void jt_000B28(void) {
    M68K_TST8(bus_read8(g_m68k.a[1] + 0 + (int16_t)(uint16_t)g_m68k.d[0]));
    /* WARNING: function did not end with RTS */
}

/* $000B2A-$000B2E  (1 instructions, 4 bytes) */
void jt_000B2A(void) {
    M68K_OR8(g_m68k.d[0], 0x12);
    /* WARNING: function did not end with RTS */
}

/* $000B2C-$000B42  (6 instructions, 22 bytes) */
void jt_000B2C(void) {
    if (M68K_CC_NE) goto loc_000B40;
    { uint16_t _mv = (uint16_t)(0xc00); bus_write16(g_m68k.a[1] + 0 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST16(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.a[0]); bus_write32(g_m68k.a[1] + 0x4 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST32(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[1]); bus_write16(g_m68k.a[1] + 0x10 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[2]); bus_write16(g_m68k.a[1] + 0x12 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST16(_mv); }
loc_000B40:
    return; /* RTE */
}

/* $000B42-$000B46  (2 instructions, 4 bytes) */
void sub_000B42(void) {
    /* TRAP #0xa */
    return;
}

/* $000B5E-$000B62  (2 instructions, 4 bytes) */
/* TRAP #7: Install task via free list (handler at $000B62 = sub_000B62) */
void sub_000B5E(void) {
    sub_000B62();
}
