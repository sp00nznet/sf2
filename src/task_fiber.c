/*
 * task_fiber.c — Windows Fiber-based cooperative multitasking for SF2 tasks.
 */

#include "task_fiber.h"
#include <cps1recomp/cps1recomp.h>
#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/* ---- State ---- */

static void *s_main_fiber = NULL;
static void *s_task_fibers[TASK_SLOT_COUNT] = {0};
static uint32_t s_fiber_handler[TASK_SLOT_COUNT] = {0};
static bool s_fiber_terminated[TASK_SLOT_COUNT] = {0};
static int s_current_slot = -1;  /* -1 = main fiber */

/* Per-fiber 68k register snapshots. The recompiled code shares the single
 * global g_m68k register file, but the original game's tasks are coroutines
 * that keep their own register state across a yield (the TRAP wrapper pushes
 * d0-d7/a0-a6 to the task's stack). A bare SwitchToFiber preserves only the C
 * stack, so a task that holds a 68k register across a sleep (e.g. the GFX-DMA
 * wait loop at $1220 keeps A6 = task control block while it sleeps A6+$13
 * frames) would resume with whatever the last task left in g_m68k.a[6], read a
 * garbage sleep count of 0, and park forever at status $02. Snapshot g_m68k on
 * every fiber boundary so each task sees its own registers on resume. */
static m68k_context_t s_fiber_regs[TASK_SLOT_COUNT];
static m68k_context_t s_main_regs;

/* ---- Fiber entry point ---- */

#ifdef _WIN32
static void CALLBACK fiber_entry(LPVOID param)
#else
static void fiber_entry(void *param)
#endif
{
    int slot = (int)(uintptr_t)param;
    uint32_t handler = s_fiber_handler[slot];

    /* Call the task's code entry point.
       The task runs until it yields (TRAP #3/#4) or terminates (TRAP #1/#9).
       After a yield, the fiber is suspended.  When the main loop resumes it,
       execution continues from the SwitchToFiber return in the yield handler.
       If the task function ever returns normally, treat it as a termination. */
    func_table_call(handler);

    /* Task returned without explicit TRAP — treat as termination */
    printf("[fiber] slot %d: task $%06X returned normally\n", slot, handler);
    fflush(stdout);

    s_fiber_terminated[slot] = true;
#ifdef _WIN32
    SwitchToFiber(s_main_fiber);
#endif

    /* Should never reach here.  If we do, spin to avoid crashing. */
    for (;;) {
#ifdef _WIN32
        SwitchToFiber(s_main_fiber);
#endif
    }
}

/* ---- Public API ---- */

void task_fiber_init(void) {
#ifdef _WIN32
    s_main_fiber = ConvertThreadToFiber(NULL);
    if (!s_main_fiber) {
        fprintf(stderr, "[fiber] ConvertThreadToFiber failed: %lu\n", GetLastError());
        return;
    }
#endif
    for (int i = 0; i < TASK_SLOT_COUNT; i++) {
        s_task_fibers[i] = NULL;
        s_fiber_terminated[i] = false;
    }
    s_current_slot = -1;
    printf("[fiber] Fiber system initialized\n");
    fflush(stdout);
}

void task_fiber_shutdown(void) {
    for (int i = 0; i < TASK_SLOT_COUNT; i++) {
        if (s_task_fibers[i]) {
#ifdef _WIN32
            DeleteFiber(s_task_fibers[i]);
#endif
            s_task_fibers[i] = NULL;
        }
    }
#ifdef _WIN32
    ConvertFiberToThread();
#endif
    s_main_fiber = NULL;
    printf("[fiber] Fiber system shut down\n");
    fflush(stdout);
}

void task_fiber_create(int slot, uint32_t handler_addr) {
    if (slot < 0 || slot >= TASK_SLOT_COUNT) return;

    /* Delete existing fiber if any */
    if (s_task_fibers[slot]) {
#ifdef _WIN32
        DeleteFiber(s_task_fibers[slot]);
#endif
        s_task_fibers[slot] = NULL;
    }

    s_fiber_handler[slot] = handler_addr;
    s_fiber_terminated[slot] = false;
    /* Seed the new task's register file from the current (main) state so it
     * inherits a valid A5 (work-RAM base $FF8000) etc.; the task sets up its
     * own registers from there at entry. */
    s_fiber_regs[slot] = g_m68k;

#ifdef _WIN32
    s_task_fibers[slot] = CreateFiber(
        FIBER_STACK_SIZE,
        fiber_entry,
        (LPVOID)(uintptr_t)slot
    );
    if (!s_task_fibers[slot]) {
        fprintf(stderr, "[fiber] CreateFiber failed for slot %d: %lu\n",
                slot, GetLastError());
    }
#endif
}

void task_fiber_switch_to(int slot) {
    if (slot < 0 || slot >= TASK_SLOT_COUNT) return;
    if (!s_task_fibers[slot]) return;

    s_current_slot = slot;

    /* Save the main fiber's registers, load this task's, then switch. */
    s_main_regs = g_m68k;
    g_m68k = s_fiber_regs[slot];

    /*TEMP: verify a5 of the game-start task (slot 2, handler $6B52). */
    if (slot == 2 && s_fiber_handler[2] == 0x006B52) {
        static int n; if (n++ < 5) { FILE*f=fopen("a5.txt","a");
            if(f){ fprintf(f,"slot2 $6B52 a5=$%08X (5d56 read addr=$%08X)\n",
                (unsigned)g_m68k.a[5], (unsigned)(g_m68k.a[5]+0x5d56)); fclose(f);} }
    }

#ifdef _WIN32
    SwitchToFiber(s_task_fibers[slot]);
#endif

    /* Execution resumes here when the task fiber yields or terminates.
     * Restore the main fiber's registers (the task saved its own on yield). */
    g_m68k = s_main_regs;
    s_current_slot = -1;

    /* Clean up terminated fibers */
    if (s_fiber_terminated[slot]) {
#ifdef _WIN32
        DeleteFiber(s_task_fibers[slot]);
#endif
        s_task_fibers[slot] = NULL;
        s_fiber_terminated[slot] = false;
    }
}

void task_fiber_yield_to_main(void) {
    /* Save this task's registers so they survive while other tasks run; they
     * are reloaded by task_fiber_switch_to before this fiber is resumed. */
    if (s_current_slot >= 0 && s_current_slot < TASK_SLOT_COUNT) {
        s_fiber_regs[s_current_slot] = g_m68k;
    }
#ifdef _WIN32
    if (s_main_fiber) {
        SwitchToFiber(s_main_fiber);
    }
#endif
    /* Execution resumes here when the main loop dispatches this task again;
     * g_m68k has been restored to this task's snapshot by switch_to. */
}

bool task_fiber_exists(int slot) {
    if (slot < 0 || slot >= TASK_SLOT_COUNT) return false;
    return s_task_fibers[slot] != NULL;
}

void task_fiber_delete(int slot) {
    if (slot < 0 || slot >= TASK_SLOT_COUNT) return;
    /* Never delete the currently-running fiber from within itself — DeleteFiber
     * on the running fiber terminates the whole thread. SF2's TRAP #11 cleanup
     * ($C5C) bulk-deletes the secondary-task slots (8-15); when the caller is
     * itself one of those slots (e.g. the credit-mode task that runs the
     * start/auto-start sequence), it must survive to finish and terminate
     * normally. Mark it terminated so the main loop reaps it after it yields. */
    if (slot == s_current_slot) {
        s_fiber_terminated[slot] = true;
        return;
    }
    if (s_task_fibers[slot]) {
#ifdef _WIN32
        DeleteFiber(s_task_fibers[slot]);
#endif
        s_task_fibers[slot] = NULL;
        s_fiber_terminated[slot] = false;
    }
}

bool task_fiber_terminated(int slot) {
    if (slot < 0 || slot >= TASK_SLOT_COUNT) return false;
    return s_fiber_terminated[slot];
}

void task_fiber_mark_terminated(void) {
    if (s_current_slot >= 0 && s_current_slot < TASK_SLOT_COUNT) {
        s_fiber_terminated[s_current_slot] = true;
    }
}
