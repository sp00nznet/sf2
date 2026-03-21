#!/usr/bin/env python3
"""
CPS1 SF2 ROM Extractor — Extracts and assembles ROM files from a MAME-format zip.

CPS1 68K program ROMs are BYTE-INTERLEAVED in pairs (ROM_LOAD16_BYTE):
  Even-byte ROM + Odd-byte ROM at each 256KB region.

Usage:
    python tools/extract_roms.py sf2.zip [--output-dir build/]
"""

import os
import sys
import struct
import zipfile
import hashlib

# ============================================================
# SF2 ROM set definition (USA Rev A — "sf2ua")
# ============================================================

# 68K program ROM pairs: (even_rom, odd_rom, base_address, rom_size)
# MAME: ROM_LOAD16_BYTE layout
SF2_PROG_PAIRS = [
    ("sf2u.30a", "sf2u.37a", 0x00000, 0x20000),  # $00000-$3FFFF
    ("sf2u.31a", "sf2u.35a", 0x40000, 0x20000),  # $40000-$7FFFF
    ("sf2_36a.bin", "sf2u.38a", 0x80000, 0x20000),  # $80000-$BFFFF
]

TOTAL_PROG_SIZE = 0xC0000  # 768 KB (3 pairs × 128KB × 2)

# GFX ROMs — CPS1 interleaving scheme
SF2_GFX_ROMS = [
    ("sf2_06.bin", 0x80000), ("sf2_08.bin", 0x80000),
    ("sf2_05.bin", 0x80000), ("sf2_07.bin", 0x80000),
    ("sf2_15.bin", 0x80000), ("sf2_17.bin", 0x80000),
    ("sf2_14.bin", 0x80000), ("sf2_16.bin", 0x80000),
    ("sf2_25.bin", 0x80000), ("sf2_27.bin", 0x80000),
    ("sf2_24.bin", 0x80000), ("sf2_26.bin", 0x80000),
]

SF2_Z80_ROM = ("sf2_09.bin", 0x10000)

SF2_OKI_ROMS = [
    ("sf2_18.bin", 0x20000),
    ("sf2_19.bin", 0x20000),
]


def extract_file(zf, name, file_list):
    """Extract a file from the zip, trying case-insensitive match."""
    if name in file_list:
        return zf.read(name)
    name_lower = name.lower()
    for f in file_list:
        if f.lower() == name_lower:
            return zf.read(f)
    return None


def assemble_program(zf, file_list, output_dir):
    """Assemble 68K program ROM from byte-interleaved pairs."""
    prog = bytearray(TOTAL_PROG_SIZE)

    for even_name, odd_name, base_addr, rom_size in SF2_PROG_PAIRS:
        even_data = extract_file(zf, even_name, file_list)
        odd_data = extract_file(zf, odd_name, file_list)

        if even_data is None:
            print(f"  WARNING: Even ROM '{even_name}' not found!")
            continue
        if odd_data is None:
            print(f"  WARNING: Odd ROM '{odd_name}' not found!")
            continue

        # Byte-interleave: even ROM -> even bytes, odd ROM -> odd bytes
        size = min(len(even_data), len(odd_data))
        for i in range(size):
            prog[base_addr + i * 2 + 0] = even_data[i]
            prog[base_addr + i * 2 + 1] = odd_data[i]

        end_addr = base_addr + size * 2 - 1
        print(f"  {even_name} + {odd_name} -> ${base_addr:05X}-${end_addr:05X} ({size*2//1024} KB)")

    # Verify vectors
    ssp = struct.unpack('>I', prog[0:4])[0]
    pc  = struct.unpack('>I', prog[4:8])[0]
    irq2 = struct.unpack('>I', prog[0x68:0x6C])[0]
    print(f"  Vectors: SSP=${ssp:08X} PC=${pc:06X} VBlank=${irq2:06X}")

    path = os.path.join(output_dir, "sf2_68k.bin")
    with open(path, 'wb') as f:
        f.write(prog)

    md5 = hashlib.md5(prog).hexdigest()
    print(f"  Assembled: {len(prog)} bytes ({len(prog)//1024} KB), MD5: {md5}")
    return prog


def decode_gfx(zf, file_list, output_dir):
    """Decode interleaved CPS1 GFX ROMs."""
    gfx_roms = []
    for filename, size in SF2_GFX_ROMS:
        data = extract_file(zf, filename, file_list)
        if data is None:
            print(f"  WARNING: GFX ROM '{filename}' not found!")
            data = bytes(size)
        gfx_roms.append(data)
        print(f"  {filename}: {len(data)} bytes")

    # Deinterleave ROM pairs into continuous bitplane data
    deinterleaved = bytearray()
    for group in range(0, len(gfx_roms), 4):
        rom0 = gfx_roms[group + 0]
        rom1 = gfx_roms[group + 1]
        rom2 = gfx_roms[group + 2]
        rom3 = gfx_roms[group + 3]
        rom_size = len(rom0)

        bp01 = bytearray(rom_size * 2)
        bp23 = bytearray(rom_size * 2)
        for i in range(rom_size):
            bp01[i * 2 + 0] = rom0[i]
            bp01[i * 2 + 1] = rom1[i]
            bp23[i * 2 + 0] = rom2[i]
            bp23[i * 2 + 1] = rom3[i]

        deinterleaved.extend(bp01)
        deinterleaved.extend(bp23)

    path = os.path.join(output_dir, "sf2_gfx.bin")
    with open(path, 'wb') as f:
        f.write(deinterleaved)

    md5 = hashlib.md5(deinterleaved).hexdigest()
    print(f"  Decoded GFX: {len(deinterleaved)} bytes ({len(deinterleaved)//1024} KB), MD5: {md5}")
    return deinterleaved


def extract_z80(zf, file_list, output_dir):
    """Extract Z80 sound program ROM."""
    filename, size = SF2_Z80_ROM
    data = extract_file(zf, filename, file_list)
    if data is None:
        print(f"  WARNING: Z80 ROM '{filename}' not found!")
        return None
    path = os.path.join(output_dir, "sf2_z80.bin")
    with open(path, 'wb') as f:
        f.write(data)
    print(f"  {filename}: {len(data)} bytes ({len(data)//1024} KB)")
    return data


def extract_oki(zf, file_list, output_dir):
    """Extract and concatenate OKI ADPCM sample ROMs."""
    oki_data = bytearray()
    for filename, size in SF2_OKI_ROMS:
        data = extract_file(zf, filename, file_list)
        if data is None:
            print(f"  WARNING: OKI ROM '{filename}' not found!")
            data = bytes(size)
        oki_data.extend(data)
        print(f"  {filename}: {len(data)} bytes")

    path = os.path.join(output_dir, "sf2_oki.bin")
    with open(path, 'wb') as f:
        f.write(oki_data)
    print(f"  OKI samples: {len(oki_data)} bytes ({len(oki_data)//1024} KB)")
    return oki_data


def main():
    import argparse
    parser = argparse.ArgumentParser(description='CPS1 SF2 ROM Extractor')
    parser.add_argument('rom_zip', help='Path to SF2 ROM zip file')
    parser.add_argument('--output-dir', '-o', default='build', help='Output directory')
    args = parser.parse_args()

    os.makedirs(args.output_dir, exist_ok=True)

    print(f"=== CPS1 SF2 ROM Extractor ===\n")
    print(f"Input:  {args.rom_zip}")
    print(f"Output: {args.output_dir}\n")

    with zipfile.ZipFile(args.rom_zip, 'r') as zf:
        file_list = zf.namelist()
        print(f"Zip contains {len(file_list)} files\n")

        print("--- Assembling 68K Program ROM (byte-interleaved pairs) ---")
        assemble_program(zf, file_list, args.output_dir)

        print("\n--- Decoding GFX ROMs ---")
        decode_gfx(zf, file_list, args.output_dir)

        print("\n--- Extracting Z80 ROM ---")
        extract_z80(zf, file_list, args.output_dir)

        print("\n--- Extracting OKI Sample ROMs ---")
        extract_oki(zf, file_list, args.output_dir)

    print(f"\nDone! All ROM files extracted to {args.output_dir}/")
    print(f"\nNext: python tools/analyze_rom.py build/sf2_68k.bin --stats")


if __name__ == '__main__':
    main()
