/*
 * main.c — Street Fighter II Recompiled entry point.
 *
 * Initializes the CPS1 runtime, registers all recompiled functions,
 * and enters the main game loop.
 */

#include <cps1recomp/cps1recomp.h>
#include "recomp/recomp_funcs.h"
#include "task_fiber.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    const char *rom_path = "build";  /* Default: pre-extracted ROMs in build/ */
    int scale = 3;

    /* Parse command line */
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            rom_path = argv[i];
        } else if (strcmp(argv[i], "--scale") == 0 && i + 1 < argc) {
            scale = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--fullscreen") == 0 || strcmp(argv[i], "-f") == 0) {
            /* handled below */
        }
    }

    /* Redirect output to log file (Windows GUI apps have no console) */
    freopen("sf2_log.txt", "w", stdout);
    freopen("sf2_log.txt", "a", stderr);

    printf("Street Fighter II Recompiled\n");
    printf("============================\n");
    printf("ROM path: %s\n\n", rom_path);
    fflush(stdout);

    /* Initialize the CPS1 runtime */
    cps1_config_t config = {
        .rom_path = rom_path,
        .window_scale = scale,
        .fullscreen = false,
        .vsync = true,
    };

    if (cps1_init(&config) != 0) {
        fprintf(stderr, "Failed to initialize CPS1 runtime\n");
        return 1;
    }

    /* Initialize fiber system for cooperative multitasking */
    task_fiber_init();

    /* Register all recompiled functions, then override with hand-written versions */
    printf("[sf2] Registering functions...\n"); fflush(stdout);
    recomp_register_all();
    recomp_register_overrides();
    printf("[sf2] Registered %u functions (%u overrides)\n", func_table_count(), 11u);
    fflush(stdout);

    /* Install TRAP vectors (game init uses LEA+BRA which recompiler can't follow) */
    bus_write32(0x000080, 0x000B24);  /* TRAP #0: install secondary task */
    bus_write32(0x000084, 0x000B62);  /* TRAP #1: kill current task */
    bus_write32(0x00008C, 0x000CB8);  /* TRAP #3: sleep for D0.w frames */
    bus_write32(0x000090, 0x000CDA);  /* TRAP #4: conditional sleep */
    bus_write32(0x00009C, 0x000D48);  /* TRAP #7: jump to secondary code */
    bus_write32(0x0000A4, 0x000DAC);  /* TRAP #9: free-list termination */

    /* Check if entry point has a function */
    if (func_table_lookup(g_m68k.pc)) {
        printf("[sf2] Entry function at $%06X: FOUND\n", g_m68k.pc);
    } else {
        printf("[sf2] Entry function at $%06X: NOT FOUND\n", g_m68k.pc);
    }
    fflush(stdout);

    /* Enter the main loop (never returns) */
    printf("[sf2] Entering main loop...\n"); fflush(stdout);
    cps1_run();

    task_fiber_shutdown();
    cps1_shutdown();
    return 0;
}
