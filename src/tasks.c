/*
 * tasks.c — Hand-written task entry points and TRAP handler overrides.
 *
 * These functions OVERRIDE the auto-generated versions because they need
 * fiber-aware behavior (infinite loops, TRAP yield/sleep/terminate).
 * They are registered AFTER the auto-generated functions in recomp_funcs.h
 * so the last registration wins.
 *
 * This file lives outside src/recomp/ so the code generator won't delete it.
 */

#include <cps1recomp/cps1recomp.h>
#include "recomp/recomp_funcs.h"
#include "task_fiber.h"
#include <stdio.h>

/* ================================================================
 * sub_00639E — Attract mode state machine (primary task, slot 0).
 * Infinite loop: read state, dispatch handler, sleep 1 frame.
 * ================================================================ */
void override_00639E(void) {
    func_table_call(0x002138);  /* one-time init */
    for (;;) {
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u)
                     | bus_read16(g_m68k.a[5] + 0x0);
        int16_t disp = (int16_t)bus_read16(
            0x63B8 + (int16_t)(uint16_t)g_m68k.d[0]);
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)disp;
        uint32_t handler = (uint32_t)((int32_t)0x63B8 + disp);
        func_table_call(handler);
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)(0x1);
        M68K_TST16((uint16_t)g_m68k.d[0]);
        func_table_call(0x000CB8);  /* TRAP #3: sleep 1 frame */
    }
}

/* ================================================================
 * sub_0014F2 — Secondary task scheduler (task, slot 6).
 * Infinite loop: scan task slots, yield, check queue.
 * ================================================================ */
void override_0014F2(void) {
    for (;;) {
        bus_write16(g_m68k.a[5] + (-0x75d0), 0);
        g_m68k.flag_n = false; g_m68k.flag_z = true;
        g_m68k.flag_v = false; g_m68k.flag_c = false;

        M68K_TST8(bus_read8(g_m68k.a[5] + 0x2e1));
        if (M68K_CC_NE) goto do_scan;
        M68K_TST8(bus_read8(g_m68k.a[5] + 0x2d7));
        if (M68K_CC_EQ) goto do_yield;
        M68K_BTST(bus_read8(g_m68k.a[5] + 0x8c), 0x0);
        if (M68K_CC_EQ) goto do_yield;

    do_scan:
        { uint16_t _mv = 1; bus_write16(g_m68k.a[5] + (-0x75d0), _mv); M68K_TST16(_mv); }
        g_m68k.a[0] = (g_m68k.a[5] + (-0x8000));
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0xf));
        M68K_TST16((uint16_t)g_m68k.d[0]);
        {
        loc_001518:
            M68K_TST8(bus_read8(g_m68k.a[0] + 0x1f));
            if (M68K_CC_NE) goto loc_001532;
            M68K_CMP8(bus_read8(g_m68k.a[0] + 0x0), 0x1);
            if (M68K_CC_NE) goto loc_001532;
            { uint32_t _ea = (g_m68k.a[0] + 0x1); uint8_t _tmp = bus_read8(_ea);
              M68K_SUB8(_tmp, 0x1); bus_write8(_ea, _tmp); }
            if (M68K_CC_NE) goto loc_001532;
            { uint8_t _mv = 0x4; bus_write8(g_m68k.a[0] + 0x0, _mv); M68K_TST8(_mv); }
        loc_001532:
            g_m68k.a[0] = (g_m68k.a[0] + 0x20);
            { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--;
              g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt;
              if (_cnt != -1) goto loc_001518; }
        }

    do_yield:
        func_table_call(0x000CEC);  /* TRAP #4: yield */
        M68K_TST8(bus_read8(g_m68k.a[5] + (-0x7df2)));
        if (M68K_CC_NE) continue;
        M68K_TST16(bus_read16(g_m68k.a[5] + (-0x7df4)));
        if (M68K_CC_EQ) continue;
        g_m68k.a[0] = (g_m68k.a[5] + 0xa2);
        g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u)
                     | ((uint16_t)(bus_read16(g_m68k.a[5] + 0x20)));
        M68K_TST16((uint16_t)g_m68k.d[2]);
        g_m68k.d[0] = bus_read32(g_m68k.a[0] + 0
                     + (int16_t)(uint16_t)g_m68k.d[2]);
        M68K_TST32((uint32_t)g_m68k.d[0]);
        if (M68K_CC_PL) {
            func_table_call(0x0014CC);
        }
    }
}

/* ================================================================
 * TRAP handler overrides (fiber-aware)
 * ================================================================ */

/* TRAP #9: Terminate + return slot to free list */
void override_000BAE(void) {
    uint32_t task_addr = bus_read32(g_m68k.a[5] + (-0x7dfc));
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    bus_write32(task_addr + 0x10, 0);
    bus_write32(task_addr + 0x14, 0);
    bus_write32(task_addr + 0x18, 0);
    bus_write32(task_addr + 0x1c, 0);
    bus_write8(task_addr + 0x0, 0);
    uint32_t free_ptr = bus_read32(g_m68k.a[5] + (-0x7df8));
    free_ptr -= 4;
    bus_write32(free_ptr, task_addr);
    bus_write32(g_m68k.a[5] + (-0x7df8), free_ptr);
    { uint32_t _ea = g_m68k.a[5] + (-0x7df4);
      uint16_t _tmp = bus_read16(_ea);
      M68K_ADD16(_tmp, 0x1);
      bus_write16(_ea, _tmp); }
    task_fiber_mark_terminated();
    task_fiber_yield_to_main();
}

/* TRAP #1: Terminate task (no free-list return) */
void override_000B8A(void) {
    uint32_t task_addr = bus_read32(g_m68k.a[5] + (-0x7dfc));
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    bus_write32(task_addr + 0x10, 0);
    bus_write32(task_addr + 0x14, 0);
    bus_write32(task_addr + 0x18, 0);
    bus_write32(task_addr + 0x1c, 0);
    bus_write8(task_addr + 0x0, 0);
    task_fiber_mark_terminated();
    task_fiber_yield_to_main();
}

/* TRAP #3: Sleep for D0.w frames */
void override_000CB8(void) {
    uint32_t task_addr = bus_read32(g_m68k.a[5] + (-0x7dfc));
    uint16_t status_word;
    if (((uint8_t)g_m68k.d[0]) == 0) {
        status_word = 0x0200;
    } else {
        status_word = 0x0100 | ((uint8_t)g_m68k.d[0]);
    }
    bus_write16(task_addr + 0x0, status_word);
    task_fiber_yield_to_main();
}

/* TRAP #4: Yield ready */
void override_000CEC(void) {
    uint32_t task_addr = bus_read32(g_m68k.a[5] + (-0x7dfc));
    bus_write8(task_addr + 0x0, 0x04);
    task_fiber_yield_to_main();
}

/* TRAP #0xB: Clear secondary task slots */
void override_000C5C(void) {
    func_table_call(0x000C60);  /* original cleanup */
    for (int i = 8; i < TASK_SLOT_COUNT; i++) {
        if (task_fiber_exists(i)) task_fiber_delete(i);
    }
}

/* ================================================================
 * override_0010E0 — Complete GFX processing function.
 *
 * The auto-generated version was truncated by the code generator
 * (only 1 of 6 BSR $1142 calls captured). This override has the
 * full function: clears $5D3A, processes 6 GFX RAM regions via
 * $1142, and accumulates the counter.
 *
 * Original 68k ($10E0-$1150):
 *   CLR.L $5D3A(A5)
 *   6× { LEA gfx_base,A0; LEA work_base,A1; MOVE.W count,D7; BSR $1142 }
 *   (falls through to $1142 on the last block)
 *   $1142: BSR $11EA; DBRA D7,$1142; MOVE.L (A1),D0; ADD.L D0,$5D3A(A5); RTS
 * ================================================================ */
void override_0010E0(void) {
    /* Clear counter */
    bus_write32(g_m68k.a[5] + 0x5d3a, 0);
    g_m68k.flag_n = false; g_m68k.flag_z = true;
    g_m68k.flag_v = false; g_m68k.flag_c = false;

    /* Block 1: $900000, work at A5+$5D3E, 32 entries */
    g_m68k.a[0] = 0x900000;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d3e);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | 0x1f;
    func_table_call(0x001142);

    /* Block 2: $900400, work at A5+$5D42, 32 entries */
    g_m68k.a[0] = 0x900400;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d42);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | 0x1f;
    func_table_call(0x001142);

    /* Block 3: $900800, work at A5+$5D46, 32 entries */
    g_m68k.a[0] = 0x900800;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d46);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | 0x1f;
    func_table_call(0x001142);

    /* Block 4: $900C00, work at A5+$5D4A, 32 entries */
    g_m68k.a[0] = 0x900c00;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d4a);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | 0x1f;
    func_table_call(0x001142);

    /* Block 5: $901000, work at A5+$5D4E, 8 entries */
    g_m68k.a[0] = 0x901000;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d4e);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | 0x07;
    func_table_call(0x001142);

    /* Block 6: $901400, work at A5+$5D52, 8 entries */
    g_m68k.a[0] = 0x901400;
    g_m68k.a[1] = (g_m68k.a[5] + 0x5d52);
    g_m68k.d[7] = (g_m68k.d[7] & 0xFFFF0000u) | 0x07;
    func_table_call(0x001142);
}

/* $E4C: Clear $5D56 and fall through to $E50 (re-enter processing loop) */
void override_000E4C(void) {
    bus_write16(g_m68k.a[5] + 0x5d56, 0);
    g_m68k.flag_n = false; g_m68k.flag_z = true;
    g_m68k.flag_v = false; g_m68k.flag_c = false;
    func_table_call(0x000E50);  /* fall through to state 0 handler */
}

/* $E9E: Set $5D56=1 and fall through to $EA4 (state 2 handler) */
void override_000E9E(void) {
    { uint16_t _mv = 1; bus_write16(g_m68k.a[5] + 0x5d56, _mv); M68K_TST16(_mv); }
    func_table_call(0x000EA4);
}

/* Safety net for func_table_call(0x000A5E) */
void trap_return_to_main(void) {
    task_fiber_yield_to_main();
}
