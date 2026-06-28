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

/* ================================================================
 * sub_000910 — Main game loop + task scheduler.
 *
 * The auto-generated version is split into non-chaining fragments
 * because the recompiler can't handle the infinite dispatch loop.
 * This override does all init then runs the fiber-based scheduler.
 * ================================================================ */
void override_000910(void) {
    /* --- Init phase (replicated from $910-$A04) --- */

    g_m68k.a[7] = 0x000000;  /* LEA $0.W,A7 */
    g_m68k.a[5] = 0xFF8000;

    /* Clear task queue indices */
    g_m68k.d[0] = 0;
    bus_write16(g_m68k.a[5] + 0x24, 0);
    bus_write16(g_m68k.a[5] + 0x22, 0);
    bus_write16(g_m68k.a[5] + 0x20, 0);
    bus_write16(g_m68k.a[5] + 0x1e, 0);

    /* Init secondary task queue ($A2) to $FFFFFFFF — 32 pairs */
    g_m68k.a[0] = g_m68k.a[5] + 0xa2;
    for (int _i = 0; _i < 32; _i++) {
        bus_write32(g_m68k.a[0], 0xFFFFFFFF); g_m68k.a[0] += 4;
        bus_write32(g_m68k.a[0], 0xFFFFFFFF); g_m68k.a[0] += 4;
    }
    /* Init secondary task data ($1A2) to 0 */
    g_m68k.a[0] = g_m68k.a[5] + 0x1a2;
    for (int _i = 0; _i < 32; _i++) {
        bus_write32(g_m68k.a[0], 0); g_m68k.a[0] += 4;
        bus_write32(g_m68k.a[0], 0); g_m68k.a[0] += 4;
    }

    /* Region/difficulty */
    bus_write16(g_m68k.a[5] + 0x2c2, 2);

    /* GFX data transfer init */
    func_table_call(0x0015D4);
    func_table_call(0x00162C);
    func_table_call(0x001C4E);
    func_table_call(0x001D8E);

    /* DIP switch check */
    bus_write8(g_m68k.a[5] + 0x322, 0);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFFFF00u) | (uint8_t)bus_read8(0x800000);
    M68K_NOT8(g_m68k.d[0]);
    if ((uint8_t)g_m68k.d[0] == 0x50) {
        bus_write8(g_m68k.a[5] + 0x322, 1);
    }

    /* Copy default task USP from ROM table at $D44 to slot+$C */
    g_m68k.a[0] = 0x000D44;
    uint32_t slot_base = g_m68k.a[5] - 0x8000;
    g_m68k.a[6] = slot_base;
    for (int _i = 0; _i < 16; _i++) {
        uint32_t usp_val = bus_read32(g_m68k.a[0]); g_m68k.a[0] += 4;
        bus_write32(g_m68k.a[6] + 0xC, usp_val);
        g_m68k.a[6] += 0x20;
    }

    /* Stage data pointers at A5+$52DA */
    g_m68k.a[0] = g_m68k.a[5] + 0x528A;
    g_m68k.a[1] = g_m68k.a[5] + 0x52DA;
    for (int _i = 0; _i < 6; _i++) {
        bus_write32(g_m68k.a[1], g_m68k.a[0]);
        g_m68k.a[1] += 4;
        g_m68k.a[0] += 8;
    }
    /* Copy stage data from ROM at $DA4 */
    g_m68k.a[0] = g_m68k.a[5] + 0x528A;
    g_m68k.a[1] = 0x000DA4;
    bus_write32(g_m68k.a[5] + 0x5302, bus_read32(g_m68k.a[1]));
    for (int _i = 0; _i < 6; _i++) {
        uint32_t v1 = bus_read32(g_m68k.a[1]); g_m68k.a[1] += 4;
        bus_write32(g_m68k.a[0], v1); g_m68k.a[0] += 4;
        uint32_t v2 = bus_read32(g_m68k.a[1]); g_m68k.a[1] += 4;
        bus_write32(g_m68k.a[0], v2); g_m68k.a[0] += 4;
    }

    /* Init free task list: 8 slot addresses stored below A5-$75D0 */
    g_m68k.a[0] = 0x000D84;
    g_m68k.a[6] = g_m68k.a[5] - 0x75D0;
    for (int _i = 0; _i < 8; _i++) {
        uint32_t val = bus_read32(g_m68k.a[0]); g_m68k.a[0] += 4;
        g_m68k.a[6] -= 4;
        bus_write32(g_m68k.a[6], val);
    }
    bus_write32(g_m68k.a[5] - 0x7DF8, g_m68k.a[6]);
    bus_write16(g_m68k.a[5] - 0x7DF4, 8);

    bus_write8(g_m68k.a[5] + 0x307, 0);

    /* Install primary tasks via TRAP #0 handler logic ($B24):
     * Set slot status to $0C00, code entry at slot+4 */
    /* Slot 0: Attract mode ($639E) */
    bus_write16(slot_base + 0x00, 0x0C00);
    bus_write32(slot_base + 0x04, 0x0000639E);
    /* Slot 6 ($C0): Secondary scheduler ($14F2) */
    bus_write16(slot_base + 0xC0, 0x0C00);
    bus_write32(slot_base + 0xC4, 0x000014F2);

    /* --- Main dispatch loop (fiber-based) --- */
    for (;;) {
        /* Run VBlank handler logic: register copies, input, timer decrements.
         * The original $A94 handler runs as an IRQ; we call it directly. */
        func_table_call(0x000A94);

        /* Trigger frame render/present/sync via the VBlank hook */
        bus_wram_write8(0x020E, 0x00);  /* Clear flag so hook fires on read */
        bus_vblank_hook_arm();
        (void)bus_read8(0xFF020E);      /* Read triggers VBlank hook */

        /* Clear VBlank flag before dispatching tasks.
         * The VBlank handler sets this; tasks check it to know a VBlank occurred.
         * In original hardware, the main loop clears it at $A0C. */
        bus_write8(g_m68k.a[5] - 0x7DF2, 0);

        /* Scan all 16 task slots and dispatch ready ones */
        uint32_t a0 = slot_base;
        for (int i = 0; i < 16; i++) {
            uint8_t status = bus_read8(a0);

            if (status == 0x0C) {
                /* New task: create fiber, set status to running, dispatch */
                uint32_t code = bus_read32(a0 + 4);
                bus_write8(a0, 0x08);
                bus_write32(g_m68k.a[5] - 0x7DFC, a0);  /* current task ptr */
                task_fiber_create(i, code);
                task_fiber_switch_to(i);
            } else if (status >= 0x04) {
                /* Ready/running: resume fiber */
                bus_write32(g_m68k.a[5] - 0x7DFC, a0);
                if (task_fiber_exists(i)) {
                    task_fiber_switch_to(i);
                }
            }

            a0 += 0x20;
        }
    }
}

/* ================================================================
 * vec_irq2_vblank ($A94) — VBlank IRQ handler override.
 *
 * Per-frame: copy shadow registers to hardware, process input,
 * decrement sleeping task timers.  The auto-generated version
 * can't handle RTE; we call the sub-functions directly.
 * ================================================================ */
void override_000A94(void) {
    g_m68k.a[5] = 0xFF8000;

    /* Copy shadow scroll/palette registers to hardware. The native vblank
     * (sub_0007CC) and $001BAA only push scroll1/palette/other; the scroll2 and
     * scroll3 base registers ($800102/$800104) and the object base ($800106) are
     * never written, so those layers default to base $0000 and read the wrong
     * GFX region (empty/garbage). Push them from their WRAM shadows here. */
    bus_write16(0x800100, bus_read16(g_m68k.a[5] + 0x2A));  /* scroll1 base  */
    bus_write16(0x800102, bus_read16(g_m68k.a[5] + 0x2C));  /* scroll2 base  */
    bus_write16(0x800104, bus_read16(g_m68k.a[5] + 0x2E));  /* scroll3 base  */
    bus_write16(0x800106, bus_read16(g_m68k.a[5] + 0x30));  /* object base   */
    bus_write16(0x800108, bus_read16(g_m68k.a[5] + 0x32));  /* palette base  */
    bus_write16(g_m68k.a[5] + 0x5E, bus_read16(0x800148));

    func_table_call(0x001BAA);  /* scroll register copy */
    func_table_call(0x000B06);  /* priority/palette + delay */
    func_table_call(0x001B14);  /* palette DMA */
    func_table_call(0x00626C);  /* input processing */
    func_table_call(0x001ED0);  /* per-frame processing 1 */
    func_table_call(0x001FE2);  /* per-frame processing 2 */
    func_table_call(0x001D72);  /* per-frame processing 3 */

    /* Increment frame counter */
    uint8_t fc = bus_read8(g_m68k.a[5] + 0x1C);
    bus_write8(g_m68k.a[5] + 0x1C, fc + 1);

    /* Set VBlank occurred flag */
    bus_write8(g_m68k.a[5] - 0x7DF2, 0xFF);

    /* Clear flag $2E2 */
    bus_write8(g_m68k.a[5] + 0x2E2, 0);

    /* Decrement sleeping task timers */
    uint32_t a0 = g_m68k.a[5] - 0x8000;
    for (int i = 0; i < 16; i++) {
        if (bus_read8(a0) == 0x01) {
            uint8_t timer = bus_read8(a0 + 1);
            timer--;
            bus_write8(a0 + 1, timer);
            if (timer == 0) {
                bus_write8(a0, 0x04);  /* Ready to run */
            }
        }
        a0 += 0x20;
    }
}

/* ================================================================
 * sub_000B20 — Install primary task (TRAP #0 wrapper).
 * D0.W = slot offset, A0 = handler address.
 * The auto-generated version can't handle TRAP; we inline it.
 * ================================================================ */
void override_000B20(void) {
    /* TRAP #0 handler at $B24: install at slot D0 if empty */
    uint32_t slot = (g_m68k.a[5] - 0x8000) + (uint16_t)g_m68k.d[0];
    if (bus_read8(slot) == 0) {
        bus_write16(slot, 0x0C00);
        bus_write32(slot + 4, g_m68k.a[0]);
        bus_write16(slot + 0x10, (uint16_t)g_m68k.d[1]);
        bus_write16(slot + 0x12, (uint16_t)g_m68k.d[2]);
    }
}

/* TRAP wrapper overrides — the recompiler skips TRAP instructions,
 * so we need to manually dispatch to the vector handlers. */

/* $B5E: TRAP #7 (install secondary task from free list) */
void override_000B5E(void) {
    func_table_call(0x000B62);  /* TRAP #7 handler */
}

/* $B5A: TRAP #1 (kill current task, no free list return) — wrapper */
/* Actually $B8A: TRAP #1; ADDQ #6,A7; ... */
/* Already handled by override_000B8A */

/* $B20: TRAP #0 (install primary task) — wrapper */
/* Already handled by override_000B20 */

/* $B42: TRAP #10 (force install primary task) */
void override_000B42(void) {
    func_table_call(0x000B46);  /* TRAP #10 handler */
}

/* $BE0: TRAP #2 */
void override_000BE0(void) {
    func_table_call(0x000BE4);  /* TRAP #2 handler */
}

/* $C0E: TRAP #8 */
void override_000C0E(void) {
    func_table_call(0x000C12);  /* TRAP #8 handler */
}

/* $C9E: TRAP #12 */
void override_000C9E(void) {
    func_table_call(0x000CA2);  /* TRAP #12 handler */
}

/* $D10: TRAP #5 */
void override_000D10(void) {
    func_table_call(0x000D18);  /* TRAP #5 handler */
}

/* $D2C: TRAP #6 */
void override_000D2C(void) {
    func_table_call(0x000D34);  /* TRAP #6 handler */
}

/* $DA8: TRAP #9 (alternate terminate entry) */
void override_000DA8(void) {
    func_table_call(0x000BB0);  /* TRAP #9 handler (same as $BAE but different entry) */
}

/* Safety net for func_table_call(0x000A5E) */
void trap_return_to_main(void) {
    task_fiber_yield_to_main();
}
