# Street Fighter II Recompiled

**The legendary 1991 arcade fighter, ripped from the CPS1 board and rebuilt as a native PC executable.**

No emulator. No interpreter. No JIT. Just Ryu's `SHORYUKEN` compiled to x86-64 and running at full native speed.

---

## What Is This?

This project takes the original **Motorola 68000 machine code** from the Street Fighter II arcade PCB and **statically recompiles** it into C source code. That C code is then compiled with a modern optimizing compiler (MSVC/Clang/GCC) into a native Windows executable.

The result? SF2 running natively on your PC, exactly as Capcom wrote it in 1991 -- every frame, every hitbox, every dizzy animation -- but compiled to run on modern hardware instead of being interpreted instruction-by-instruction.

```c
// What the CPS1 board's 68000 CPU executed in 1991:
//   move.w  d0, $FF8000       ; store player state
//   add.w   #1, d0            ; advance frame counter
//   bsr     check_inputs      ; read the joystick
//   beq     .idle             ; no input? idle animation

// What your PC runs in 2026:
bus_write16(0xFF8000, (uint16_t)g_m68k.d[0]);
M68K_ADD16(g_m68k.d[0], 1);
func_table_call(0x012A4C);    // check_inputs
if (M68K_CC_EQ) goto idle;
```

## The CPS1 Hardware We're Replacing

The original Street Fighter II ran on Capcom's **CPS1** (Capcom Play System 1) arcade board. Here's what's inside -- and what this project replaces with native code:

| CPS1 Component | Spec | Our Replacement |
|----------------|------|-----------------|
| Main CPU | **Motorola 68000** @ 10 MHz | Statically recompiled to native x86-64 C |
| Audio CPU | **Zilog Z80** @ 3.579 MHz | Interpreted Z80 core |
| FM Synth | **Yamaha YM2151** (OPM) | [ymfm](https://github.com/aaronsgiles/ymfm) library |
| Sample Playback | **OKI MSM6295** (ADPCM) | Custom 4-channel ADPCM decoder |
| Graphics | **CPS-A + CPS-B** custom ASICs | Software renderer (scroll layers + sprites) |
| Resolution | 384 x 224 @ ~59.63 Hz | SDL2 window, any scale |
| Palettes | 192 x 16 colors, 12-bit RGB | Real-time palette conversion |
| Controls | 6-button arcade panel | Keyboard + gamepad via SDL2 |

## How Static Recompilation Works

```
 1991                                          2026
 ┌──────────────┐                             ┌──────────────┐
 │  SF2 ROMs    │     analyze_rom.py          │  sf2.exe     │
 │  (68000      │──────────────────┐          │  (native     │
 │   machine    │                  │          │   x86-64)    │
 │   code)      │                  ▼          │              │
 └──────────────┘          ┌───────────────┐  └──────┬───────┘
                           │ generate_     │         │
                           │ recomp.py     │         │ links against
                           │               │         │
                           │ M68K → C      │         ▼
                           │ translator    │  ┌──────────────┐
                           └───────┬───────┘  │ cps1recomp   │
                                   │          │ runtime      │
                                   ▼          │              │
                           ┌───────────────┐  │ • CPS1 video │
                           │ recomp_*.c    │  │ • YM2151 FM  │
                           │               │──│ • OKI ADPCM  │
                           │ (recompiled   │  │ • Z80 sound  │
                           │  C source)    │  │ • SDL2 I/O   │
                           └───────────────┘  └──────────────┘
```

1. **Disassemble** -- Recursive descent through the 68K program ROMs using Capstone
2. **Lift** -- Translate every M68K instruction to an equivalent C macro (`M68K_ADD16`, `M68K_CMP32`, `M68K_BCC`, etc.)
3. **Link** -- Compile against `cps1recomp`, which provides the CPS1 memory map, video rendering, audio chips, and input handling
4. **Run** -- Out comes `sf2.exe`, a native Windows executable. No emulator in the loop.

## Project Structure

```
$RECOMP_ROOT\cps1\
├── cps1recomp/                    # CPS1 hardware runtime library
│   ├── include/cps1recomp/
│   │   ├── cps1recomp.h           # Top-level API
│   │   ├── m68k.h                 # 68000 CPU context + instruction macros
│   │   ├── bus.h                  # CPS1 memory map
│   │   ├── func_table.h           # Recompiled function dispatch
│   │   ├── video.h                # CPS1 scroll layers + sprites
│   │   ├── palette.h              # 12-bit RGB palette system
│   │   ├── ym2151.h               # YM2151 FM synthesis (via ymfm)
│   │   ├── oki6295.h              # OKI MSM6295 ADPCM
│   │   ├── z80.h                  # Z80 audio CPU
│   │   ├── io.h                   # Input (6-button fighter layout)
│   │   ├── timer.h                # Interrupt timing
│   │   ├── rom.h                  # Multi-ROM loader
│   │   ├── platform.h             # SDL2 windowing/audio/input
│   │   └── debug.h                # Tracing, breakpoints
│   └── src/
│       └── *.c                    # Implementation files
│
└── sf2/                           # The game project
    ├── sf2.zip                    # ROM files (you supply these)
    ├── tools/
    │   ├── extract_roms.py        # ROM extraction + GFX decoding
    │   ├── analyze_rom.py         # Disassembly + function discovery
    │   └── generate_recomp.py     # M68K → C code generator
    └── src/
        ├── main.c                 # Entry point
        └── recomp/                # Generated recompiled code
            └── recomp_*.c
```

## Building

### Prerequisites

- **Windows 11** (Linux/macOS planned)
- **CMake** 3.16+
- **SDL2** (`vcpkg install sdl2:x64-windows`)
- **Visual Studio 2022** (or Clang/GCC)
- **Python 3.8+** with `capstone` (`pip install capstone`)

### Build

```bash
# Clone with submodules
git clone --recursive https://github.com/sp00nznet/sf2.git
cd sf2

# Step 1: Extract and analyze the ROM
python tools/extract_roms.py sf2.zip
python tools/analyze_rom.py build/sf2_68k.bin
python tools/generate_recomp.py build/sf2_68k.bin build/functions.json

# Step 2: Build the executable
cmake -B build -G "Visual Studio 17 2022" -A x64 \
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release

# Step 3: FIGHT!
./build/Release/sf2.exe
```

## Controls

| Action | Keyboard | Gamepad |
|--------|----------|---------|
| Move | Arrow keys | D-pad / Left stick |
| Light Punch | A | X |
| Medium Punch | S | Y |
| Heavy Punch | D | RB |
| Light Kick | Z | A |
| Medium Kick | X | B |
| Heavy Kick | C | RT |
| Start | Enter | Start |
| Coin | 5 | Back/Select |

## The 8 World Warriors

```
  ┌─────────┬─────────┬─────────┬─────────┐
  │  RYU    │  HONDA  │ BLANKA  │ GUILE   │
  │  Japan  │  Japan  │ Brazil  │  USA    │
  │ 🥋     │ 🏋️     │ ⚡     │ 🎖️     │
  ├─────────┼─────────┼─────────┼─────────┤
  │  KEN    │ CHUN-LI │ ZANGIEF │ DHALSIM │
  │  USA    │  China  │ USSR    │  India  │
  │ 🔥     │ 💪     │ 🐻     │ 🧘     │
  └─────────┴─────────┴─────────┴─────────┘
```

## Implementation Status

| Phase | Component | Status |
|-------|-----------|--------|
| **0** | Project skeleton + build system | Done |
| **1** | ROM loader + CPS1 memory bus | Done |
| **2** | M68K disassembler + lifter tools | Done |
| **3** | Scroll layers + palette renderer | Done |
| **4** | Sprite engine | Done |
| **5** | Platform layer + input + first boot | Done |
| **6** | Z80 sound CPU | Done (interpreter, ported from z80recomp) |
| **7** | YM2151 FM audio (via ymfm) | Done |
| **8** | OKI MSM6295 ADPCM | Done |
| **9** | Cooperative task system (Windows Fibers) | Done |
| **10** | Attract mode state machine | Done |
| **11** | Coin / credit / START input flow | Done |
| **12** | Game-start sequence + **character select reached** | Done |
| **13** | GFX rendering pipeline (sprites, scroll layers) | Playable text/screens; portraits WIP |

### Current State (July 2026)

**The game now runs end to end from boot to the character-select screen.** Boot -> attract demo -> insert coin -> press START -> game-start sequence -> **PLAYER SELECT screen**, all driven by real 68000 game logic on the cooperative fiber task system. The attract "warning / disclaimer" screen and the player-select screen both render with **legible text on a clean background**.

Getting here meant fixing a chain of deep bugs, each of which blocked everything downstream. In order:

- **Fiber register preservation** -- task fibers shared the single global `g_m68k` register file; a bare `SwitchToFiber` preserved the C stack but not the 68k registers, so any task holding a register across a sleep resumed corrupted. Snapshotting `g_m68k` per fiber un-froze the entire task system (attract had been stuck at state 12 forever).
- **Missing 68k instructions** -- implemented `roxl`/`roxr` (rotate-through-extend) and `abcd`/`sbcd`/`nbcd` (BCD math), needed by the coin/credit debounce and counter.
- **Code-generator control-flow fixes** -- a conditional `if(cc){...return;}` as a function's last statement wrongly suppressed fall-through to the next function; a `dbra`-to-own-entry compiled to recursive `func_table_call` (stack overflow) instead of a loop; a cross-function `bra`/`jmp` target was left unregistered.
- **Analyzer: `movea.l` task handlers** -- SF2 installs tasks via `movea.l #addr,a0` then a TRAP, but the address-load scan only matched `move.l`, so the game-start handler (`$6B52`) was never registered and its task never ran. Matching `movea.l` was the single fix that unlocked the whole game-start sequence.
- **TRAP #11 infinite recursion** -- a wrapper override called its own registered handler address through the function table, re-entering itself.
- **Renderer fixes** (in `cps1recomp`): CPS1 object/sprite field order was scrambled (X/Y/tile/attr) + no multi-tile sprites; scroll3 was drawn as 16x16 when it is a **32x32-tile layer**; the backdrop was filled with palette entry 0 (red) instead of black. Fixing these turned garbage into legible screens.

The **Z80 sound CPU is live**: a single-step Z80 interpreter (ported from the sp00nznet `z80recomp` runtime) executes the original sound program against the CPS1 sound memory map, paced by the YM2151 timer IRQ driving the ymfm engine -- verified with audible FM output.

**What's working:**
- Full attract -> coin -> credit -> START -> game-start -> character-select flow on real game logic
- Cooperative fiber task system with per-fiber register state; main loop, VBlank handler, all 16 TRAP handlers
- Coin/credit counting and START handling reach the game via the correct CPS1 input map
- ~4900 recompiled functions; only a handful of func_table misses (recovered via `extra_entries`)
- Renderer: 8x8 scroll1 text, 16x16 scroll2 + sprites (incl. multi-tile), 32x32 scroll3, layer-enable, black backdrop
- Legible attract/warning text and PLAYER SELECT screen

**Known gaps / next up:**
- Character-select **portraits and world-map not yet rendering** (additional sprites/graphics loaded later in the select sequence -- needs tracing)
- CPS-B register `$800148` uses a provisional readback value (`0x0407`) to pass SF2's attract-terminate check -- the real hardware value should be sourced
- Palette brightness (4-bit intensity scale) is approximated; exact 32x32 tile arrangement unverified against a reference
- The remaining func-table misses in the game-start subtree are patched via `extra_entries.txt`; the analyzer could recover them automatically

## Standing on Giants

This project is built on the proven [sp00nznet](https://github.com/sp00nznet) static recompilation ecosystem:

- **[genrecomp](https://github.com/sp00nznet/genrecomp)** -- M68K recompilation runtime for Sega Genesis. Our M68K instruction macros and function dispatch table come directly from here.
- **[neogeorecomp](https://github.com/sp00nznet/neogeorecomp)** -- Neo Geo runtime. Closest architectural sibling (same 68000 CPU, similar arcade hardware).
- **[N64Recomp](https://github.com/N64Recomp/N64Recomp)** -- The project that proved static recompilation works at scale.
- **[ymfm](https://github.com/aaronsgiles/ymfm)** -- Cycle-accurate FM synthesis by the MAME lead. Powers our YM2151.

## Legal

This project does not include any copyrighted game code, graphics, audio, or ROM files. You must supply your own legally obtained SF2 ROM set. `cps1recomp` is a tool -- what you build with it is your responsibility.

## License

MIT License. Use it for anything.

---

*"You must defeat Sheng Long to stand a chance."*

**Built with love for the game that started it all.** Part of [sp00nznet](https://github.com/sp00nznet).
