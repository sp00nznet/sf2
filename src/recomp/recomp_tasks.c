/*
 * recomp_tasks.c — Hand-written recompiled task entry points.
 *
 * These functions are task code that the auto-generator missed because
 * the entry points weren't discovered during recursive descent, or
 * because the code was embedded as a goto label inside another function.
 */

#include <cps1recomp/cps1recomp.h>
#include "recomp_funcs.h"
#include "../task_fiber.h"
#include <stdio.h>

/* ================================================================
 * sub_00639E — Attract mode state machine (primary task, slot 0).
 *
 * Original 68k:
 *   $639E: JSR $2138             ; one-time init
 *   $63A2: MOVE.W (A5),D0        ; read current state
 *   $63A6: MOVE.W $63B8(PC,D0),D0; table lookup (word displacement)
 *   $63AA: JSR $63B8(PC,D0)      ; call state handler
 *   $63AE: MOVE.W #1,D0          ; sleep 1 frame
 *   $63B2: JSR $CB8              ; TRAP #3 wrapper (yield)
 *   $63B6: BRA.B $63A2           ; loop
 *   $63B8: <state dispatch table>
 * ================================================================ */
void sub_00639E(void) {
    /* One-time initialisation */
    func_table_call(0x002138);

    /* State machine loop — runs once per frame */
    for (;;) {
        /* Read current state word from (A5) */
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u)
                     | bus_read16(g_m68k.a[5] + 0x0);

        /* Table lookup: word displacement at $63B8 + state */
        int16_t disp = (int16_t)bus_read16(
            0x63B8 + (int16_t)(uint16_t)g_m68k.d[0]);
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)disp;

        /* Call state handler at $63B8 + displacement */
        uint32_t handler = (uint32_t)((int32_t)0x63B8 + disp);
        func_table_call(handler);

        /* Sleep 1 frame via TRAP #3 wrapper */
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)(0x1);
        M68K_TST16((uint16_t)g_m68k.d[0]);
        func_table_call(0x000CB8);   /* sub_000CB8 — yields fiber */
    }
}

/* ================================================================
 * sub_0014F2 — Secondary task scheduler (task, slot 6 = offset 0xC0).
 *
 * Runs as a task in the primary scheduler.  Each frame it:
 *   1. Checks conditions ($2E1, $2D7, bit 0 of $8C)
 *   2. If conditions met: scans 16 secondary task slots,
 *      decrements sleep counters, promotes status 1→4
 *   3. Yields via TRAP #4
 *   4. Checks for pending tasks in the queue and installs them
 *   5. Loops
 * ================================================================ */
void sub_0014F2(void) {
    for (;;) {
        /* $14F2: Clear secondary scheduler flag */
        bus_write16(g_m68k.a[5] + (-0x75d0), 0);
        g_m68k.flag_n = false; g_m68k.flag_z = true;
        g_m68k.flag_v = false; g_m68k.flag_c = false;

        /* $14F6-$1508: Check conditions for task scanning */
        M68K_TST8(bus_read8(g_m68k.a[5] + 0x2e1));
        if (M68K_CC_NE) goto do_scan;

        M68K_TST8(bus_read8(g_m68k.a[5] + 0x2d7));
        if (M68K_CC_EQ) goto do_yield;

        M68K_BTST(bus_read8(g_m68k.a[5] + 0x8c), 0x0);
        if (M68K_CC_EQ) goto do_yield;

    do_scan:
        /* $150A: Set flag — secondary tasks are being scanned */
        { uint16_t _mv = (uint16_t)(0x1);
          bus_write16(g_m68k.a[5] + (-0x75d0), _mv);
          M68K_TST16(_mv); }

        /* $1510-$1536: Scan 16 secondary task slots */
        g_m68k.a[0] = (g_m68k.a[5] + (-0x8000));
        g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | ((uint16_t)(0xf));
        M68K_TST16((uint16_t)g_m68k.d[0]);
        {
        loc_001518:
            M68K_TST8(bus_read8(g_m68k.a[0] + 0x1f));
            if (M68K_CC_NE) goto loc_001532;
            M68K_CMP8(bus_read8(g_m68k.a[0] + 0x0), 0x1);
            if (M68K_CC_NE) goto loc_001532;
            { uint32_t _ea = (g_m68k.a[0] + 0x1);
              uint8_t _tmp = bus_read8(_ea);
              M68K_SUB8(_tmp, 0x1);
              bus_write8(_ea, _tmp); }
            if (M68K_CC_NE) goto loc_001532;
            { uint8_t _mv = (uint8_t)(0x4);
              bus_write8(g_m68k.a[0] + 0x0, _mv);
              M68K_TST8(_mv); }
        loc_001532:
            g_m68k.a[0] = (g_m68k.a[0] + 0x20);
            { int16_t _cnt = (int16_t)(uint16_t)g_m68k.d[0]; _cnt--;
              g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | (uint16_t)_cnt;
              if (_cnt != -1) goto loc_001518; }
        }

    do_yield:
        /* $153A: TRAP #4 — yield to main loop */
        func_table_call(0x000CEC);

        /* $153E: After yield — check vblank flag */
        M68K_TST8(bus_read8(g_m68k.a[5] + (-0x7df2)));
        if (M68K_CC_NE) continue;   /* vblank set → loop */

        /* $1544: Check free slot count */
        M68K_TST16(bus_read16(g_m68k.a[5] + (-0x7df4)));
        if (M68K_CC_EQ) continue;   /* no free slots → loop */

        /* $154A-$1556: Check pending task queue */
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
        /* $155A: BRA $14F2 — loop */
    }
}

/* ================================================================
 * sub_0014CC — Process a pending task from the secondary queue.
 *
 * Called from sub_0014F2 when a pending task entry is found.
 * Writes -1 to the queue slot (consuming it), advances the
 * circular index, extracts slot offset + code address, and
 * installs the task via TRAP #7 (free-list installer).
 * ================================================================ */
void sub_0014CC(void) {
    /* $14CC: Mark queue entry as consumed */
    { uint32_t _mv = (uint32_t)(0xffffffff);
      bus_write32(g_m68k.a[0] + 0
                + (int16_t)(uint16_t)g_m68k.d[2], _mv);
      M68K_TST32(_mv); }

    /* Advance circular queue index */
    M68K_ADD16(g_m68k.d[2], 0x4);
    M68K_AND16(g_m68k.d[2], 0xfc);
    { uint16_t _mv = (uint16_t)(g_m68k.d[2]);
      bus_write16(g_m68k.a[5] + 0x20, _mv);
      M68K_TST16(_mv); }

    /* Extract task parameters from packed D0 */
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u)
                 | ((uint16_t)(g_m68k.d[0]));
    M68K_TST16((uint16_t)g_m68k.d[1]);
    M68K_SWAP(g_m68k.d[0]);
    g_m68k.d[2] = (g_m68k.d[2] & 0xFFFF0000u)
                 | ((uint16_t)(g_m68k.d[0]));
    M68K_TST16((uint16_t)g_m68k.d[2]);
    M68K_LSR16(g_m68k.d[2], 0x8);
    M68K_AND16(g_m68k.d[0], 0xff);

    /* Look up task code address from table at $155C */
    g_m68k.a[0] = bus_read32(0x155c
                + (int16_t)(uint16_t)g_m68k.d[2]);

    /* Install via TRAP #7 (free-list task installer) */
    func_table_call(0x000B5E);
}

/* ================================================================
 * Attract mode state handlers ($63CA-$6498).
 * These are called from the state machine table at $63B8.
 * ================================================================ */

/* $63CA: State 0 — advance state, set timer, clear flag */
void sub_0063CA(void) {
    { uint32_t _ea = g_m68k.a[5] + 0x0; uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x2); bus_write16(_ea, _tmp); }
    { uint16_t _mv = 0x1C3; bus_write16(g_m68k.a[5] + 0x2C4, _mv); M68K_TST16(_mv); }
    { uint8_t _mv = 0; bus_write8(g_m68k.a[5] + 0x2E1, _mv); M68K_TST8(_mv); }
}

/* $63DA: State 2 — start attract mode setup */
void sub_0063DA(void) {
    { uint32_t _ea = g_m68k.a[5] + 0x0; uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x2); bus_write16(_ea, _tmp); }
    func_table_call(0x001D9A);
    func_table_call(0x001DF0);
    func_table_call(0x001E16);
    { uint8_t _mv = 1; bus_write8(g_m68k.a[5] + 0x2DA, _mv); M68K_TST8(_mv); }
    { uint16_t _mv = 0x100; bus_write16(g_m68k.a[5] + 0x2B8, _mv); M68K_TST16(_mv); }
    func_table_call(0x001692);
    func_table_call(0x000DFC);
}

/* $63FE: State 6 — wait for $2E0 flag to clear */
void sub_0063FE(void) {
    M68K_TST8(bus_read8(g_m68k.a[5] + 0x2E0));
    if (M68K_CC_NE) return;
    { uint32_t _ea = g_m68k.a[5] + 0x0; uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x2); bus_write16(_ea, _tmp); }
}

/* $640A: State 8/14 — countdown timer, reinit when done */
void sub_00640A(void) {
    { uint32_t _ea = g_m68k.a[5] + 0x2; uint16_t _tmp = bus_read16(_ea); M68K_SUB16(_tmp, 0x1); bus_write16(_ea, _tmp); }
    if (M68K_CC_NE) { func_table_call(0x00237E); return; }
    { uint32_t _ea = g_m68k.a[5] + 0x0; uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x2); bus_write16(_ea, _tmp); }
    { uint8_t _mv = 0; bus_write8(g_m68k.a[5] + 0x2E1, _mv); M68K_TST8(_mv); }
    func_table_call(0x002138);
}

/* $6420: State 12 — wait for $2E0, then advance and call $237E */
void sub_006420(void) {
    M68K_TST8(bus_read8(g_m68k.a[5] + 0x2E0));
    if (M68K_CC_NE) { func_table_call(0x00237E); return; }
    { uint8_t _mv = 0; bus_write8(g_m68k.a[5] + 0x2E1, _mv); M68K_TST8(_mv); }
    { uint32_t _ea = g_m68k.a[5] + 0x0; uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x2); bus_write16(_ea, _tmp); }
    func_table_call(0x00237E);
}

/* $6432: State 4 — setup scene with counter and palette */
void sub_006432(void) {
    { uint32_t _ea = g_m68k.a[5] + 0x0; uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x2); bus_write16(_ea, _tmp); }
    { uint16_t _mv = 0xB4; bus_write16(g_m68k.a[5] + 0x2, _mv); M68K_TST16(_mv); }
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | 0x2; M68K_TST16(0x2);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | 0x5; M68K_TST16(0x5);
    func_table_call(0x0021E2);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | bus_read16(g_m68k.a[5] + 0x2C2);
    M68K_TST16((uint16_t)g_m68k.d[0]);
    /* PC-relative table at $645E: displacement indexed by D0 */
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u)
                 | bus_read16(0x645E + (int16_t)(uint16_t)g_m68k.d[0]);
    M68K_TST16((uint16_t)g_m68k.d[0]);
    { uint8_t _mv = 1; bus_write8(g_m68k.a[5] + 0x2E0, _mv); M68K_TST8(_mv); }
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | 0x100; M68K_TST16(0x100);
    func_table_call(0x0021E2);
}

/* $6464: State 10 — another scene setup */
void sub_006464(void) {
    { uint32_t _ea = g_m68k.a[5] + 0x0; uint16_t _tmp = bus_read16(_ea); M68K_ADD16(_tmp, 0x2); bus_write16(_ea, _tmp); }
    { uint16_t _mv = 0x3C; bus_write16(g_m68k.a[5] + 0x2, _mv); M68K_TST16(_mv); }
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | 0x2; M68K_TST16(0x2);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | 0x5; M68K_TST16(0x5);
    func_table_call(0x0021E2);
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | bus_read16(g_m68k.a[5] + 0x2C2);
    M68K_TST16((uint16_t)g_m68k.d[0]);
    /* PC-relative table at $6490 */
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u)
                 | bus_read16(0x6490 + (int16_t)(uint16_t)g_m68k.d[0]);
    M68K_TST16((uint16_t)g_m68k.d[0]);
    { uint8_t _mv = 1; bus_write8(g_m68k.a[5] + 0x2E0, _mv); M68K_TST8(_mv); }
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | 0x101; M68K_TST16(0x101);
    func_table_call(0x0021E2);
}

/* $237E: Check joystick/button input, set $2E1 flag if any input */
void sub_00237E(void) {
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | bus_read16(g_m68k.a[5] + 0x7E);
    M68K_TST16((uint16_t)g_m68k.d[0]);
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | bus_read16(g_m68k.a[5] + 0x84);
    M68K_TST16((uint16_t)g_m68k.d[1]);
    goto loc_0023AE;

    /* $2388 path (not reached from $237E, but included for completeness) */
    M68K_TST8(bus_read8(g_m68k.a[5] + 0x2DA));
    if (M68K_CC_NE) return;  /* goto loc_0023BC */
    { uint8_t _mv = 0; bus_write8(g_m68k.a[5] + 0x2E1, _mv); M68K_TST8(_mv); }
    g_m68k.d[0] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[0]);
    M68K_BTST(bus_read8(g_m68k.a[5] + 0x2E4), 0x0);
    if (M68K_CC_EQ) goto loc_0023A0;
    g_m68k.d[0] = (g_m68k.d[0] & 0xFFFF0000u) | bus_read16(g_m68k.a[5] + 0x658);
    M68K_TST16((uint16_t)g_m68k.d[0]);
loc_0023A0:
    g_m68k.d[1] = (uint32_t)(int32_t)(int8_t)(0x0); M68K_TST32(g_m68k.d[1]);
    M68K_BTST(bus_read8(g_m68k.a[5] + 0x2E4), 0x1);
    if (M68K_CC_EQ) goto loc_0023AE;
    g_m68k.d[1] = (g_m68k.d[1] & 0xFFFF0000u) | bus_read16(g_m68k.a[5] + 0x958);
    M68K_TST16((uint16_t)g_m68k.d[1]);
loc_0023AE:
    M68K_OR16(g_m68k.d[0], g_m68k.d[1]);
    M68K_AND16(g_m68k.d[0], 0x770);
    if (M68K_CC_EQ) return;
    { uint8_t _mv = 1; bus_write8(g_m68k.a[5] + 0x2E1, _mv); M68K_TST8(_mv); }
}

/* ================================================================
 * trap_return_to_main — Safety net for func_table_call(0x000A5E).
 *
 * Some TRAP handlers in dead code paths call func_table_call(0xA5E)
 * to return to the main loop.  With fibers, we just yield instead.
 * ================================================================ */
void trap_return_to_main(void) {
    task_fiber_yield_to_main();
}
