/* Auto-generated recompiled code for Street Fighter II */
/* Source range: $000B62 - $00118C */
/* Functions: 50 */

#include <cps1recomp/cps1recomp.h>
#include "recomp_funcs.h"
#include "../task_fiber.h"

/* $000B62-$000BE4  (37 instructions, 130 bytes) */
void sub_000B62(void) {
    M68K_TST16(bus_read16(g_m68k.a[5] + (-0x7df4)));
    if (M68K_CC_EQ) goto loc_000B88;
    g_m68k.a[3] = bus_read32(g_m68k.a[5] + (-0x7df8));
    g_m68k.a[1] = _postinc32(3);
    { uint32_t _mv = (uint32_t)(g_m68k.a[3]); bus_write32(g_m68k.a[5] + (-0x7df8), _mv); M68K_TST32(_mv); }
    { uint32_t _ea = (g_m68k.a[5] + (-0x7df4)); uint16_t _tmp = bus_read16(_ea); M68K_SUB16(_tmp, 0x1); bus_write16(_ea, _tmp); }
    { uint16_t _mv = (uint16_t)(0xc00); bus_write16(g_m68k.a[1] + 0x0, _mv); M68K_TST16(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.a[0]); bus_write32(g_m68k.a[1] + 0x4, _mv); M68K_TST32(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[1] + 0x10, _mv); M68K_TST16(_mv); }
    { uint16_t _mv = (uint16_t)(g_m68k.d[1]); bus_write16(g_m68k.a[1] + 0x12, _mv); M68K_TST16(_mv); }
loc_000B88:
    return; /* RTE */
    /* TRAP #0x1 */
    g_m68k.a[7] += 0x6;
    g_m68k.a[0] = bus_read32(g_m68k.a[5] + (-0x7dfc));
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x10, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x14, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x18, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x1c, _mv); M68K_TST32(_mv); }
    { uint8_t _mv = (uint8_t)(0x0); bus_write8(g_m68k.a[0] + 0x0, _mv); M68K_TST8(_mv); }
    { func_table_call(0x000A5E); return; }
    /* TRAP #0x9 */
    g_m68k.a[7] += 0x6;
    g_m68k.a[0] = bus_read32(g_m68k.a[5] + (-0x7dfc));
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x10, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x14, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x18, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[0] + 0x1c, _mv); M68K_TST32(_mv); }
    { uint8_t _mv = (uint8_t)(0x0); bus_write8(g_m68k.a[0] + 0x0, _mv); M68K_TST8(_mv); }
    g_m68k.a[4] = bus_read32(g_m68k.a[5] + (-0x7df8));
    { uint32_t _mv = (uint32_t)(g_m68k.a[0]); g_m68k.a[4] -= 4; bus_write32(g_m68k.a[4], _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.a[4]); bus_write32(g_m68k.a[5] + (-0x7df8), _mv); M68K_TST32(_mv); }
    { uint32_t _ea = (g_m68k.a[5] + (-0x7df4)); uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x1); bus_write16(_ea, _tmp); }
    { func_table_call(0x000A5E); return; }
    /* TRAP #0x2 */
    return;
}

/* $000BE4-$000C00  (8 instructions, 28 bytes) */
void sub_000BE4(void) {
    g_m68k.a[4] = (g_m68k.a[5] + (-0x8000));
    M68K_TST8(bus_read8(g_m68k.a[4] + 0 + (int16_t)(uint16_t)g_m68k.d[0]));
    if (M68K_CC_EQ) { func_table_call(0x000C06); return; }
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    { uint32_t _mv = (uint32_t)(g_m68k.d[1]); bus_write32(g_m68k.a[4] + 0x10 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[1]); bus_write32(g_m68k.a[4] + 0x14 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[1]); bus_write32(g_m68k.a[4] + 0x18 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[1]); bus_write32(g_m68k.a[4] + 0x1c + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST32(_mv); }
    /* WARNING: function did not end with RTS */
}

/* $000C00-$000C08  (2 instructions, 8 bytes) */
void jt_000C00(void) {
    { uint8_t _mv = (uint8_t)(0x0); bus_write8(g_m68k.a[4] + 0 + (int16_t)(uint16_t)g_m68k.d[0], _mv); M68K_TST8(_mv); }
loc_000C06:
    return; /* RTE */
}

/* $000C08-$000C12  (4 instructions, 10 bytes) */
void sub_000C08(void) {
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[5] + 0x2dc))); M68K_TST8((uint8_t)g_m68k.d[1]);
    if (M68K_CC_EQ) goto loc_000C10;
    /* TRAP #0x8 */
loc_000C10:
    return;
}

/* $000C5C-$000C60  (2 instructions, 4 bytes) */
/* TRAP #0xB: Clear secondary task slots and their fibers.
   Handler at $C60 (sub_000C60) clears 8 task slots at A5-$7F00. */
void sub_000C5C(void) {
    sub_000C60();
    /* Also delete fibers for the secondary task slots (8-15) */
    for (int i = 8; i < TASK_SLOT_COUNT; i++) {
        if (task_fiber_exists(i)) task_fiber_delete(i);
    }
}

/* $000C60-$000C9E  (17 instructions, 62 bytes) */
void sub_000C60(void) {
    g_m68k.a[4] = (g_m68k.a[5] + (-0x7f00));
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x7)); M68K_TST16((uint16_t)g_m68k.d[7]);
loc_000C68:
    M68K_TST8(bus_read8(g_m68k.a[4] + 0x0));
    if (M68K_CC_EQ) goto loc_000C94;
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[4] + 0x10, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[4] + 0x14, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[4] + 0x18, _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.d[0]); bus_write32(g_m68k.a[4] + 0x1c, _mv); M68K_TST32(_mv); }
    { uint8_t _mv = (uint8_t)(0x0); bus_write8(g_m68k.a[4] + 0x0, _mv); M68K_TST8(_mv); }
    g_m68k.a[3] = bus_read32(g_m68k.a[5] + (-0x7df8));
    { uint32_t _mv = (uint32_t)(g_m68k.a[4]); g_m68k.a[3] -= 4; bus_write32(g_m68k.a[3], _mv); M68K_TST32(_mv); }
    { uint32_t _mv = (uint32_t)(g_m68k.a[3]); bus_write32(g_m68k.a[5] + (-0x7df8), _mv); M68K_TST32(_mv); }
    { uint32_t _ea = (g_m68k.a[5] + (-0x7df4)); uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x1); bus_write16(_ea, _tmp); }
loc_000C94:
    g_m68k.a[4] = (g_m68k.a[4] + 0x20);
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[7]; _cnt--; g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { goto loc_000C68; } }
    return; /* RTE */
}

/* $000CB8-$000CC4  (4 instructions, 12 bytes) */
/* TRAP #3: Sleep for D0.w frames.  Yields the current task fiber.
   Original: MOVEM save, TRAP #3, MOVEM restore, RTS.
   TRAP #3 handler at $CC4 sets status to sleeping with frame count. */
void sub_000CB8(void) {
    uint32_t task_addr = bus_read32(g_m68k.a[5] + (-0x7dfc));
    /* Build status word: high byte = status type, low byte = frame count.
       D0 == 0 → status 0x0200 (special 2-frame sleep)
       D0 != 0 → status 0x0100 | D0 (sleep D0 frames, status byte = 1) */
    uint16_t status_word;
    if (((uint8_t)g_m68k.d[0]) == 0) {
        status_word = 0x0200;
    } else {
        status_word = 0x0100 | ((uint8_t)g_m68k.d[0]);
    }
    bus_write16(task_addr + 0x0, status_word);
    task_fiber_yield_to_main();
    /* Fiber resumes here when sleep expires and task is re-dispatched */
}

/* $000CC4-$000CEC  (12 instructions, 40 bytes) */
void sub_000CC4(void) {
    g_m68k.a[0] = bus_read32(g_m68k.a[5] + (-0x7dfc));
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | ((uint16_t)(0x100)); M68K_TST16((uint16_t)g_m68k.d[1]);
    M68K_TST8(g_m68k.d[0]);
    if (M68K_CC_NE) goto loc_000CD4;
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | ((uint16_t)(0x200)); M68K_TST16((uint16_t)g_m68k.d[1]);
loc_000CD4:
    M68K_OR16(g_m68k.d[0], g_m68k.d[1]);
    { uint16_t _mv = (uint16_t)(g_m68k.d[0]); bus_write16(g_m68k.a[0] + 0x0, _mv); M68K_TST16(_mv); }
    g_m68k.a[1] = (g_m68k.a[1] & 0xFFFF0000u) | ((uint16_t)(g_m68k.usp)); M68K_TST16((uint16_t)g_m68k.a[1]);
    { uint32_t _mv = (uint32_t)(g_m68k.a[1]); bus_write32(g_m68k.a[0] + 0x8, _mv); M68K_TST32(_mv); }
    { uint16_t _mv = (uint16_t)(_postinc16(7)); bus_write16(g_m68k.a[0] + 0x2, _mv); M68K_TST16(_mv); }
    { uint32_t _mv = (uint32_t)(_postinc32(7)); bus_write32(g_m68k.a[0] + 0x4, _mv); M68K_TST32(_mv); }
    { func_table_call(0x000A5E); return; }
    /* WARNING: function did not end with RTS */
}

/* $000CEC-$000CF0  (2 instructions, 4 bytes) */
/* TRAP #4: Yield — set task status to "ready" (4) and return to main loop.
   The task will be re-dispatched on the next main loop scan. */
void sub_000CEC(void) {
    uint32_t task_addr = bus_read32(g_m68k.a[5] + (-0x7dfc));
    bus_write8(task_addr + 0x0, 0x04);
    task_fiber_yield_to_main();
    /* Fiber resumes here when re-dispatched */
}

/* $000CF0-$000D0C  (7 instructions, 28 bytes) */
void sub_000CF0(void) {
    g_m68k.a[0] = bus_read32(g_m68k.a[5] + (-0x7dfc));
    { uint8_t _mv = (uint8_t)(0x4); bus_write8(g_m68k.a[0] + 0x0, _mv); M68K_TST8(_mv); }
    g_m68k.a[1] = (g_m68k.a[1] & 0xFFFF0000u) | ((uint16_t)(g_m68k.usp)); M68K_TST16((uint16_t)g_m68k.a[1]);
    { uint32_t _mv = (uint32_t)(g_m68k.a[1]); bus_write32(g_m68k.a[0] + 0x8, _mv); M68K_TST32(_mv); }
    { uint16_t _mv = (uint16_t)(_postinc16(7)); bus_write16(g_m68k.a[0] + 0x2, _mv); M68K_TST16(_mv); }
    { uint32_t _mv = (uint32_t)(_postinc32(7)); bus_write32(g_m68k.a[0] + 0x4, _mv); M68K_TST32(_mv); }
    { func_table_call(0x000A5E); return; }
    /* WARNING: function did not end with RTS */
}

/* $000D0C-$000D18  (4 instructions, 12 bytes) */
void sub_000D0C(void) {
    g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[6]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[5]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[4]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[3]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[2]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[1]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[0]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[7]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[6]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[5]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[4]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[3]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[2]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[1]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[0]);
    /* TRAP #0x5 */
    g_m68k.d[0] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[1] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[2] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[3] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[4] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[5] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[6] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[7] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[0] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[1] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[2] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[3] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[4] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[5] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[6] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4;
    return;
}

/* $000D28-$000D34  (4 instructions, 12 bytes) */
void sub_000D28(void) {
    g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[6]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[5]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[4]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[3]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[2]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[1]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.a[0]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[7]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[6]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[5]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[4]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[3]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[2]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[1]); g_m68k.a[7] -= 4; bus_write32(g_m68k.a[7], g_m68k.d[0]);
    /* TRAP #0x6 */
    g_m68k.d[0] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[1] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[2] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[3] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[4] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[5] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[6] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.d[7] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[0] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[1] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[2] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[3] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[4] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[5] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4; g_m68k.a[6] = bus_read32(g_m68k.a[7]); g_m68k.a[7] += 4;
    return;
}

/* $000DD4-$000DEE  (9 instructions, 26 bytes) */
void sub_000DD4(void) {
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(bus_read16(g_m68k.a[5] + 0x2c4))); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | ((uint16_t)(g_m68k.d[0])); M68K_TST16((uint16_t)g_m68k.d[1]);
    M68K_ADD16(g_m68k.d[0], g_m68k.d[0]);
    M68K_ADD16(g_m68k.d[0], g_m68k.d[1]);
    M68K_LSR16(g_m68k.d[0], 0x8);
    { uint32_t _ea = (g_m68k.a[5] + 0x2c5); uint8_t _tmp = bus_read8(_ea); M68K_ADD8(_tmp, g_m68k.d[0]); bus_write8(_ea, _tmp); }
    { uint8_t _mv = (uint8_t)(g_m68k.d[0]); bus_write8(g_m68k.a[5] + 0x2c4, _mv); M68K_TST8(_mv); }
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[5] + 0x2c5))); M68K_TST8((uint8_t)g_m68k.d[0]);
    return;
}

/* $000DEE-$000DFC  (4 instructions, 14 bytes) */
void sub_000DEE(void) {
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[5] + 0x2db))); M68K_TST8((uint8_t)g_m68k.d[0]);
loc_000DF2:
    M68K_AND8(g_m68k.d[0], 0xf3);
    { uint8_t _mv = (uint8_t)(g_m68k.d[0]); bus_write8(g_m68k.a[5] + 0x2db, _mv); M68K_TST8(_mv); }
    return;
}

/* $000DFC-$000E00  (1 instructions, 4 bytes) */
void sub_000DFC(void) {
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[5] + 0x2db))); M68K_TST8((uint8_t)g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $000E00-$000E12  (5 instructions, 18 bytes) */
void jt_000E00(void) {
    M68K_CMP8(bus_read8(g_m68k.a[5] + 0x2dd), 0x9);
    if (M68K_CC_CC) { func_table_call(0x000DF2); return; }
    M68K_OR8(g_m68k.d[0], 0xc);
    { uint8_t _mv = (uint8_t)(g_m68k.d[0]); bus_write8(g_m68k.a[5] + 0x2db, _mv); M68K_TST8(_mv); }
    return;
}

/* $000E12-$000E22  (4 instructions, 16 bytes) */
void jt_000E12(void) {
    g_m68k.a[6] = bus_read32(g_m68k.a[5] + (-0x7dfc));
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(bus_read16(g_m68k.a[6] + 0x10))); M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | ((uint16_t)(bus_read16(0xe22 + (int16_t)(uint16_t)g_m68k.d[0]))); M68K_TST16((uint16_t)g_m68k.d[1]);
    { func_table_call((0xe22 + (int16_t)(uint16_t)g_m68k.d[1])); return; }
    /* WARNING: function did not end with RTS */
}

/* $000E28-$000E4A  (7 instructions, 34 bytes) */
void jt_000E28(void) {
    { uint32_t _ea = (g_m68k.a[4] + 0x74 + (int16_t)(uint16_t)g_m68k.d[0]); uint16_t _tmp = bus_read16(_ea); M68K_AND16(_tmp, 0x3fe); bus_write16(_ea, _tmp); }
    { uint32_t _ea = g_m68k.a[6]; uint32_t _tmp = bus_read32(_ea); M68K_SUB32(_tmp, 0x4c804f2); bus_write32(_ea, _tmp); }
    M68K_BTST(bus_read8(g_m68k.a[6]), g_m68k.d[2]);
    M68K_BCHG(g_m68k.d[6], g_m68k.d[2]);
    { uint32_t _ea = /* UNHANDLED_ADDR: ([$d2, d0.w * 4], $126) */ 0; uint8_t _b = 1u << ((g_m68k.d[2]) & 7); uint8_t _v = bus_read8(_ea); g_m68k.flag_z = !(_v & _b); bus_write8(_ea, _v ^ _b); }
    { uint32_t _ea = /* UNHANDLED_ADDR: ([], $5d0) */ 0; uint8_t _b = 1u << ((g_m68k.d[0]) & 7); uint8_t _v = bus_read8(_ea); g_m68k.flag_z = !(_v & _b); bus_write8(_ea, _v ^ _b); }
    { uint32_t _ea = g_m68k.a[6]; uint8_t _tmp = bus_read8(_ea); M68K_ADD8(_tmp, 0x52); bus_write8(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $000F00-$000FAC  (16 instructions, 172 bytes) */
void jt_000F00(void) {
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0xf000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    func_table_call(0x001152); /* jt_001152 */
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[6] + 0x13))); M68K_TST8((uint8_t)g_m68k.d[0]);
    func_table_call(0x000CB8);
    M68K_CMP32(bus_read32(g_m68k.a[5] + 0x5d3a), 0x1fa);
    if (M68K_CC_NE) { func_table_call(0x000F00); return; }
    bus_write8(g_m68k.a[5] + 0x5d58, 0); g_m68k.flag_n = false; g_m68k.flag_z = true; g_m68k.flag_v = false; g_m68k.flag_c = false;
    { uint16_t _mv = (uint16_t)(0x1); bus_write16(g_m68k.a[5] + 0x5d56, _mv); M68K_TST16(_mv); }
    { uint32_t _ea = (g_m68k.a[5] + 0x52); uint16_t _tmp = bus_read16(_ea); M68K_AND16(_tmp, 0xffc0); bus_write16(_ea, _tmp); }
    { func_table_call(0x000BAE); return; }
loc_000F94:
    M68K_CMP32(bus_read32(g_m68k.a[5] + 0x5d4a), 0xffffffff);
    if (M68K_CC_EQ) { func_table_call(0x000FE0); return; }
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0xf000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    g_m68k.a[0] = 0x900400;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d42);
    /* WARNING: function did not end with RTS */
}

/* $000FAC-$001000  (21 instructions, 84 bytes) */
void jt_000FAC(void) {
    func_table_call(0x001288); /* loc_001288 */
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0xf000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    g_m68k.a[0] = 0x900800;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d46);
    func_table_call(0x001288); /* loc_001288 */
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0xf000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    g_m68k.a[0] = 0x900C00;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d4a);
    func_table_call(0x001288); /* loc_001288 */
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[6] + 0x13))); M68K_TST8((uint8_t)g_m68k.d[0]);
    func_table_call(0x000CB8);
    { func_table_call(0x000F94); return; }
loc_000FE0:
    { func_table_call(0x000BAE); return; }
loc_000FE4:
    M68K_TST32(bus_read32(g_m68k.a[5] + 0x5d4a));
    if (M68K_CC_EQ) { func_table_call(0x00102C); return; }
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0x1000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    g_m68k.a[0] = 0x900400;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d42);
    func_table_call(0x001288); /* loc_001288 */
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0x1000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    /* WARNING: function did not end with RTS */
}

/* $001000-$00100E  (3 instructions, 14 bytes) */
void jt_001000(void) {
    g_m68k.a[0] = 0x900800;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d46);
    func_table_call(0x001288); /* loc_001288 */
    /* WARNING: function did not end with RTS */
}

/* $00100C-$001018  (4 instructions, 12 bytes) */
void jt_00100C(void) {
    m68k_set_sr(m68k_get_sr() & 0x343c);
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0x1000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(g_m68k.d[0])); M68K_TST8((uint8_t)g_m68k.d[0]);
    g_m68k.a[0] = 0x900C00;
    /* WARNING: function did not end with RTS */
}

/* $001018-$001026  (4 instructions, 14 bytes) */
void jt_001018(void) {
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d4a);
    func_table_call(0x001288); /* loc_001288 */
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[6] + 0x13))); M68K_TST8((uint8_t)g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $001024-$001078  (28 instructions, 84 bytes) */
void jt_001024(void) {
    { uint32_t _ea = g_m68k.a[3]; uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0xb8); bus_write8(_ea, _tmp); }
    func_table_call(0x000CB8);
    M68K_CMP32(bus_read32(0x000BAE), 0x60b84ef8);
    { func_table_call(0x000FE4); return; }
loc_00102C:
    { func_table_call(0x000BAE); return; }
loc_001030:
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[6] + 0x12))); M68K_TST8((uint8_t)g_m68k.d[0]);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(g_m68k.d[0])); M68K_TST16((uint16_t)g_m68k.d[7]);
    M68K_AND16(g_m68k.d[0], 0xe0);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | ((uint16_t)(g_m68k.d[0])); M68K_TST16((uint16_t)g_m68k.d[1]);
    M68K_LSL16(g_m68k.d[0], 0x5);
    g_m68k.a[0] = 0x900000;
    g_m68k.a[0] += g_m68k.d[0];
    M68K_AND16(g_m68k.d[7], 0x1f);
    { uint16_t _mv = (uint16_t)(g_m68k.d[7]); bus_write16(g_m68k.a[6] + 0x14, _mv); M68K_TST16(_mv); }
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u) | ((uint16_t)(0xf000)); M68K_TST16((uint16_t)g_m68k.d[2]);
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d3e);
    M68K_LSR16(g_m68k.d[1], 0x3);
    g_m68k.a[1] = (g_m68k.a[1] + 0 + (int16_t)(uint16_t)g_m68k.d[1]);
    g_m68k.d[0] = bus_read32(g_m68k.a[1]); M68K_TST32((uint32_t)g_m68k.d[0]);
    M68K_BTST(g_m68k.d[0], g_m68k.d[7]);
    if (M68K_CC_NE) goto loc_001074;
    func_table_call(0x0011E2); /* loc_0011E2 */
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | ((uint8_t)(bus_read8(g_m68k.a[6] + 0x13))); M68K_TST8((uint8_t)g_m68k.d[0]);
    func_table_call(0x000CB8);
    goto loc_001030;
loc_001074:
    { func_table_call(0x000BAE); return; }
    /* WARNING: function did not end with RTS */
}

/* $001100-$001104  (1 instructions, 4 bytes) */
void jt_001100(void) {
    { uint32_t _ea = g_m68k.a[7]; uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0x3e); bus_write8(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $001104-$001142  (15 instructions, 62 bytes) */
void jt_001104(void) {
    g_m68k.a[0] = 0x900800;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d46);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x1f)); M68K_TST16((uint16_t)g_m68k.d[7]);
    func_table_call(0x001142); /* jt_001142 */
    g_m68k.a[0] = 0x900C00;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d4a);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x1f)); M68K_TST16((uint16_t)g_m68k.d[7]);
    func_table_call(0x001142); /* jt_001142 */
    g_m68k.a[0] = 0x901000;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d4e);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x7)); M68K_TST16((uint16_t)g_m68k.d[7]);
    func_table_call(0x001142); /* jt_001142 */
    g_m68k.a[0] = 0x901400;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d52);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x7)); M68K_TST16((uint16_t)g_m68k.d[7]);
    /* WARNING: function did not end with RTS */
}

/* $001142-$001146  (1 instructions, 4 bytes) */
void jt_001142(void) {
    func_table_call(0x0011EA); /* loc_0011EA */
    /* WARNING: function did not end with RTS */
}

/* $001144-$00114A  (2 instructions, 6 bytes) */
void jt_001144(void) {
    { uint32_t _ea = g_m68k.a[6]; uint32_t _tmp = bus_read32(_ea); M68K_OR32(_tmp, 0x51cffffa); bus_write32(_ea, _tmp); }
    { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[7]; _cnt--; g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | (uint16_t)_cnt; if (_cnt != -1) { func_table_call(0x001142); return; } }
    /* WARNING: function did not end with RTS */
}

/* $00114A-$00114C  (1 instructions, 2 bytes) */
void jt_00114A(void) {
    g_m68k.d[0] = bus_read32(g_m68k.a[1]); M68K_TST32((uint32_t)g_m68k.d[0]);
    /* WARNING: function did not end with RTS */
}

/* $00114C-$001150  (1 instructions, 4 bytes) */
void jt_00114C(void) {
    { uint32_t _ea = (g_m68k.a[5] + 0x5d3a); uint32_t _tmp = bus_read32(_ea); M68K_ADD32(_tmp, g_m68k.d[0]); bus_write32(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $001150-$001152  (1 instructions, 2 bytes) */
void jt_001150(void) {
    return;
}

/* $001152-$001156  (1 instructions, 4 bytes) */
void jt_001152(void) {
    bus_write32(g_m68k.a[5] + 0x5d3a, 0); g_m68k.flag_n = false; g_m68k.flag_z = true; g_m68k.flag_v = false; g_m68k.flag_c = false;
    /* WARNING: function did not end with RTS */
}

/* $001156-$00115C  (1 instructions, 6 bytes) */
void jt_001156(void) {
    g_m68k.a[0] = 0x900020;
    /* WARNING: function did not end with RTS */
}

/* $001158-$00115E  (1 instructions, 6 bytes) */
void jt_001158(void) {
    { uint32_t _ea = g_m68k.a[0]; uint32_t _tmp = bus_read32(_ea); M68K_OR32(_tmp, 0x2043ed); bus_write32(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $00115A-$00115E  (1 instructions, 4 bytes) */
void jt_00115A(void) {
    { uint32_t _ea = g_m68k.a[0]; uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0xed); bus_write8(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $00115C-$001160  (1 instructions, 4 bytes) */
void jt_00115C(void) {
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d3e);
    /* WARNING: function did not end with RTS */
}

/* $001160-$001164  (1 instructions, 4 bytes) */
void jt_001160(void) {
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x1e)); M68K_TST16((uint16_t)g_m68k.d[7]);
    /* WARNING: function did not end with RTS */
}

/* $001162-$001166  (1 instructions, 4 bytes) */
void jt_001162(void) {
    { uint32_t _ea = g_m68k.a[6]; uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0x5c); bus_write8(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $001164-$001166  (1 instructions, 2 bytes) */
void jt_001164(void) {
    func_table_call(0x0011C2); /* loc_0011C2 */
    /* WARNING: function did not end with RTS */
}

/* $001166-$00116C  (1 instructions, 6 bytes) */
void jt_001166(void) {
    g_m68k.a[0] = 0x900400;
    /* WARNING: function did not end with RTS */
}

/* $001168-$00116E  (1 instructions, 6 bytes) */
void jt_001168(void) {
    { uint32_t _ea = g_m68k.a[0]; uint32_t _tmp = bus_read32(_ea); M68K_OR32(_tmp, 0x40043ed); bus_write32(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $00116A-$00116E  (1 instructions, 4 bytes) */
void jt_00116A(void) {
    M68K_SUB8(g_m68k.d[0], 0xed);
    /* WARNING: function did not end with RTS */
}

/* $00116C-$001170  (1 instructions, 4 bytes) */
void jt_00116C(void) {
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d42);
    /* WARNING: function did not end with RTS */
}

/* $00116E-$001170  (1 instructions, 2 bytes) */
void jt_00116E(void) {
    M68K_SUB16(g_m68k.d[2], 0x6);
    /* WARNING: function did not end with RTS */
}

/* $001170-$001174  (1 instructions, 4 bytes) */
void jt_001170(void) {
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x1f)); M68K_TST16((uint16_t)g_m68k.d[7]);
    /* WARNING: function did not end with RTS */
}

/* $001172-$001176  (1 instructions, 4 bytes) */
void jt_001172(void) {
    { uint32_t _ea = g_m68k.a[7]; uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0x3e); bus_write8(_ea, _tmp); }
    /* WARNING: function did not end with RTS */
}

/* $001174-$001176  (1 instructions, 2 bytes) */
void jt_001174(void) {
    func_table_call(0x0011B4); /* loc_0011B4 */
    /* WARNING: function did not end with RTS */
}

/* $001176-$00117C  (1 instructions, 6 bytes) */
void jt_001176(void) {
    g_m68k.a[0] = 0x900800;
    /* WARNING: function did not end with RTS */
}

/* $001178-$001184  (4 instructions, 12 bytes) */
void jt_001178(void) {
    { uint32_t _ea = g_m68k.a[0]; uint32_t _tmp = bus_read32(_ea); M68K_OR32(_tmp, 0x80043ed); bus_write32(_ea, _tmp); }
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d46);
    M68K_SUB16(g_m68k.d[6], 0x6);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | ((uint16_t)(0x1f)); M68K_TST16((uint16_t)g_m68k.d[7]);
    /* WARNING: function did not end with RTS */
}

/* $001182-$00118C  (3 instructions, 10 bytes) */
void jt_001182(void) {
    { uint32_t _ea = g_m68k.a[7]; uint8_t _tmp = bus_read8(_ea); M68K_OR8(_tmp, 0x2e); bus_write8(_ea, _tmp); }
    func_table_call(0x0011B4); /* loc_0011B4 */
    g_m68k.a[0] = 0x900C00;
    /* WARNING: function did not end with RTS */
}
