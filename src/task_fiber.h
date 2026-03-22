/*
 * task_fiber.h — Windows Fiber-based cooperative multitasking for SF2 tasks.
 *
 * The original 68000 code uses TRAP instructions to yield/sleep/terminate
 * tasks cooperatively. In the recompiled C code, each task gets its own
 * Windows Fiber with a separate C stack. The main loop runs in the "main"
 * fiber and switches to task fibers to dispatch them.
 *
 * Yield (TRAP #3/#4): saves 68k state, switches back to main fiber.
 * Resume: switches to the task fiber, which resumes exactly where it yielded.
 * Terminate (TRAP #1/#9): marks fiber for deletion, switches to main fiber.
 */

#ifndef TASK_FIBER_H
#define TASK_FIBER_H

#include <stdint.h>
#include <stdbool.h>

#define TASK_SLOT_COUNT  16
#define TASK_SLOT_SIZE   0x20
#define FIBER_STACK_SIZE (64 * 1024)  /* 64 KB per task fiber */

/* Initialize the fiber system.  Must be called once from the main thread
   before any task dispatch.  Converts the main thread to a fiber. */
void task_fiber_init(void);

/* Shut down the fiber system.  Deletes all task fibers and converts
   the main fiber back to a normal thread. */
void task_fiber_shutdown(void);

/* Create a new fiber for the given task slot.
   handler_addr is the 68k address of the task's code entry point.
   Any existing fiber for this slot is deleted first. */
void task_fiber_create(int slot, uint32_t handler_addr);

/* Switch from the main fiber to the task fiber for the given slot.
   Returns when the task fiber yields or terminates. */
void task_fiber_switch_to(int slot);

/* Called from within a task fiber (TRAP handler) to yield back to main. */
void task_fiber_yield_to_main(void);

/* Check whether a fiber exists for the given slot. */
bool task_fiber_exists(int slot);

/* Delete the fiber for the given slot (called from main fiber only). */
void task_fiber_delete(int slot);

/* Returns true if the most recently switched-to fiber has terminated
   (called TRAP #1/#9).  The main fiber should delete it after this. */
bool task_fiber_terminated(int slot);

/* Mark the current task fiber as terminated (called from TRAP #1/#9). */
void task_fiber_mark_terminated(void);

#endif /* TASK_FIBER_H */
