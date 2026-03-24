#!/usr/bin/env python3
"""Patches recomp_funcs.h with the manual override block after regeneration."""

import sys

OVERRIDE_BLOCK = '''
/* ================================================================
 * Manual overrides - replace recompiled stubs with hand-written C.
 * These are defined in tasks.c (fiber-aware infinite loops, TRAP handlers).
 * ================================================================ */
extern void override_00639E(void);
extern void override_0014F2(void);
extern void override_000B8A(void);
extern void override_000BAE(void);
extern void override_000CB8(void);
extern void override_000CEC(void);
extern void override_000C5C(void);
extern void override_0010E0(void);
extern void override_000E4C(void);
extern void override_000E9E(void);
extern void override_000910(void);
extern void override_000A94(void);
extern void override_000B20(void);
extern void trap_return_to_main(void);

static inline void recomp_register_overrides(void) {
    func_table_register(0x000910, override_000910);
    func_table_register(0x000A94, override_000A94);
    func_table_register(0x000B20, override_000B20);
    func_table_register(0x00639E, override_00639E);
    func_table_register(0x0014F2, override_0014F2);
    func_table_register(0x000B8A, override_000B8A);
    func_table_register(0x000BAE, override_000BAE);
    func_table_register(0x000CB8, override_000CB8);
    func_table_register(0x000CEC, override_000CEC);
    func_table_register(0x000C5C, override_000C5C);
    func_table_register(0x0010E0, override_0010E0);
    func_table_register(0x000E4C, override_000E4C);
    func_table_register(0x000E9E, override_000E9E);
}

'''

def main():
    path = 'src/recomp/recomp_funcs.h'
    with open(path, 'r') as f:
        content = f.read()

    if 'override_' in content:
        print(f'{path}: already patched')
        return

    content = content.replace(
        '#endif /* RECOMP_FUNCS_H */',
        OVERRIDE_BLOCK + '#endif /* RECOMP_FUNCS_H */'
    )

    with open(path, 'w') as f:
        f.write(content)
    print(f'{path}: patched with overrides')

if __name__ == '__main__':
    main()
