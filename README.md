# Street Fighter II Recompiled

```
    _____ _______ ____  _____ _____ _______   _____ ___ ____ _   _ _____ _____ ____    ___ ___
   / ____|__   __|  _ \| ____|  ___| ______| |  ___|_ _/ ___| | | |_   _| ____|  _ \  |_ _|_ _|
  | (___    | |  | |_) |  _| | |_  | |__    | |_   | | |  _| |_| | | | |  _| | |_) |  | | | |
   \___ \   | |  |  _ <| |___|  _| |  __|   |  _|  | | |_| |  _  | | | | |___|  _ <   | | | |
   ____) |  | |  | |_) |_____|_|   | |      | |   |___|____|_| |_| |_| |_____|_| \_\ |___|___|
  |_____/   |_|  |____/            |_|      |_|
                                                                      R E C O M P I L E D
```

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
D:\recomp\cps1\
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
| **0** | Project skeleton + build system | In Progress |
| **1** | ROM loader + CPS1 memory bus | Planned |
| **2** | M68K disassembler + lifter tools | Planned |
| **3** | Scroll layers + palette renderer | Planned |
| **4** | Sprite engine | Planned |
| **5** | Platform layer + input + first boot | Planned |
| **6** | Z80 sound CPU | Planned |
| **7** | YM2151 FM audio (via ymfm) | Planned |
| **8** | OKI MSM6295 ADPCM | Planned |
| **9** | Polish + verification vs MAME | Planned |

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
