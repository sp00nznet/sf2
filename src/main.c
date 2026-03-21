/*
 * main.c — Street Fighter II Recompiled entry point.
 *
 * Initializes the CPS1 runtime, registers all recompiled functions,
 * and enters the main game loop.
 */

#include <cps1recomp/cps1recomp.h>
#include "recomp/recomp_funcs.h"
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

    /* Redirect stdout to a log file for debugging */
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

    /* Register all recompiled functions */
    printf("[sf2] Registering functions...\n"); fflush(stdout);
    recomp_register_all();
    printf("[sf2] Registered %u recompiled functions\n", func_table_count());
    fflush(stdout);

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

    cps1_shutdown();
    return 0;
}
