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

#ifdef _WIN32
    SwitchToFiber(s_task_fibers[slot]);
#endif

    /* Execution resumes here when the task fiber yields or terminates */
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
#ifdef _WIN32
    if (s_main_fiber) {
        SwitchToFiber(s_main_fiber);
    }
#endif
    /* Execution resumes here when the main loop dispatches this task again */
}

bool task_fiber_exists(int slot) {
    if (slot < 0 || slot >= TASK_SLOT_COUNT) return false;
    return s_task_fibers[slot] != NULL;
}

void task_fiber_delete(int slot) {
    if (slot < 0 || slot >= TASK_SLOT_COUNT) return;
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
