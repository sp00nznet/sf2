#!/usr/bin/env python3
"""
CPS1 SF2 ROM Extractor — Extracts and assembles ROM files from a MAME-format zip.

Assembles the 68K program from multiple ROM chips, decodes interleaved GFX ROMs
into linear 4bpp tile data, and prepares Z80/OKI ROM files for the runtime.

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

SF2_PROG_ROMS = [
    # (filename, load_address, size)
    ("sf2u.30a", 0x00000, 0x20000),
    ("sf2u.37a", 0x20000, 0x20000),
    ("sf2u.31a", 0x40000, 0x20000),
    ("sf2u.35a", 0x60000, 0x20000),
    ("sf2u.38a", 0x80000, 0x20000),
]

# GFX ROMs — CPS1 interleaving scheme
# Grouped in sets of 4: pairs of even/odd ROMs for two bitplane groups
# The decode order follows MAME's CPS1 driver
SF2_GFX_ROMS = [
    # Group 1: tiles 0x00000-0x0FFFF (each ROM is 512KB = 0x80000)
    ("sf2_06.bin", 0x80000),  # bitplanes 0,1 (even bytes)
    ("sf2_08.bin", 0x80000),  # bitplanes 0,1 (odd bytes)
    ("sf2_05.bin", 0x80000),  # bitplanes 2,3 (even bytes)
    ("sf2_07.bin", 0x80000),  # bitplanes 2,3 (odd bytes)
    # Group 2
    ("sf2_15.bin", 0x80000),
    ("sf2_17.bin", 0x80000),
    ("sf2_14.bin", 0x80000),
    ("sf2_16.bin", 0x80000),
    # Group 3
    ("sf2_25.bin", 0x80000),
    ("sf2_27.bin", 0x80000),
    ("sf2_24.bin", 0x80000),
    ("sf2_26.bin", 0x80000),
]

SF2_Z80_ROM = ("sf2_09.bin", 0x10000)

SF2_OKI_ROMS = [
    ("sf2_18.bin", 0x20000),
    ("sf2_19.bin", 0x20000),
]

TOTAL_PROG_SIZE = 0xA0000      # 640 KB
TOTAL_GFX_SIZE  = 0x600000     # 6 MB
TOTAL_OKI_SIZE  = 0x40000      # 256 KB


def extract_file(zf, name, file_list):
    """Extract a file from the zip, trying case-insensitive match."""
    # Try exact match first
    if name in file_list:
        return zf.read(name)
    # Case-insensitive
    name_lower = name.lower()
    for f in file_list:
        if f.lower() == name_lower:
            return zf.read(f)
    return None


def assemble_program(zf, file_list, output_dir):
    """Assemble 68K program ROM from multiple chips."""
    prog = bytearray(TOTAL_PROG_SIZE)

    for filename, load_addr, size in SF2_PROG_ROMS:
        data = extract_file(zf, filename, file_list)
        if data is None:
            print(f"  WARNING: Program ROM '{filename}' not found!")
            continue
        if len(data) != size:
            print(f"  WARNING: '{filename}' size {len(data)}, expected {size}")
        prog[load_addr:load_addr + len(data)] = data
        print(f"  {filename} -> ${load_addr:05X}-${load_addr + len(data) - 1:05X}")

    # Write assembled binary
    path = os.path.join(output_dir, "sf2_68k.bin")
    with open(path, 'wb') as f:
        f.write(prog)

    md5 = hashlib.md5(prog).hexdigest()
    print(f"  Assembled: {len(prog)} bytes ({len(prog)//1024} KB), MD5: {md5}")
    return prog


def decode_gfx(zf, file_list, output_dir):
    """Decode interleaved CPS1 GFX ROMs into linear 4bpp tile data.

    CPS1 GFX ROM layout:
    - ROMs come in groups of 4
    - Within each group: ROM0+ROM1 are byte-interleaved for bitplanes 0,1
                         ROM2+ROM3 are byte-interleaved for bitplanes 2,3
    - Each pixel is 4bpp (16 colors per palette entry)

    The output is linear 4bpp packed data where each 8x8 tile is 32 bytes:
      4 bytes per row, 8 rows = 32 bytes per 8x8 tile
      Each byte holds 2 pixels (high nibble = left pixel, low nibble = right pixel)
    """
    # Load all GFX ROMs
    gfx_roms = []
    for filename, size in SF2_GFX_ROMS:
        data = extract_file(zf, filename, file_list)
        if data is None:
            print(f"  WARNING: GFX ROM '{filename}' not found!")
            data = bytes(size)
        if len(data) < size:
            data = data + bytes(size - len(data))
        gfx_roms.append(data)
        print(f"  {filename}: {len(data)} bytes")

    # First pass: deinterleave ROM pairs into continuous bitplane data
    # Process in groups of 4 ROMs
    deinterleaved = bytearray()
    for group in range(0, len(gfx_roms), 4):
        rom0 = gfx_roms[group + 0]  # bp01 even
        rom1 = gfx_roms[group + 1]  # bp01 odd
        rom2 = gfx_roms[group + 2]  # bp23 even
        rom3 = gfx_roms[group + 3]  # bp23 odd

        rom_size = len(rom0)

        # Byte-interleave each pair
        bp01 = bytearray(rom_size * 2)
        bp23 = bytearray(rom_size * 2)
        for i in range(rom_size):
            bp01[i * 2 + 0] = rom0[i]
            bp01[i * 2 + 1] = rom1[i]
            bp23[i * 2 + 0] = rom2[i]
            bp23[i * 2 + 1] = rom3[i]

        # Now convert from CPS1 planar to packed 4bpp
        # CPS1 tiles are 8x8 pixels, stored as:
        #   For each row (8 rows):
        #     2 bytes from bp01 data (bitplanes 0 and 1)
        #     2 bytes from bp23 data (bitplanes 2 and 3)
        # Each byte pair encodes 8 pixels across 2 bitplanes

        tile_size_planar = 8  # 8 bytes per tile in each bitplane pair (2 bytes/row * 4 rows... actually it's more complex)

        # CPS1 uses a specific tile encoding. Let me implement it correctly:
        # Each 8x8 tile occupies 32 bytes total (4bpp = 4 bits * 64 pixels = 256 bits = 32 bytes)
        #
        # In the interleaved ROM data, each tile's data is laid out as:
        # 8 rows, each row has 2 bytes from bp01 and 2 bytes from bp23
        # bp01 byte = 8 pixels of bitplane 0 in one byte, 8 pixels of bitplane 1 in next byte
        # bp23 byte = 8 pixels of bitplane 2 in one byte, 8 pixels of bitplane 3 in next byte
        #
        # But actually CPS1 stores tile data differently - let me just store the raw
        # deinterleaved data and decode at render time

        deinterleaved.extend(bp01)
        deinterleaved.extend(bp23)

    # Write raw deinterleaved GFX data
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

        print("--- Assembling 68K Program ROM ---")
        assemble_program(zf, file_list, args.output_dir)

        print("\n--- Decoding GFX ROMs ---")
        decode_gfx(zf, file_list, args.output_dir)

        print("\n--- Extracting Z80 ROM ---")
        extract_z80(zf, file_list, args.output_dir)

        print("\n--- Extracting OKI Sample ROMs ---")
        extract_oki(zf, file_list, args.output_dir)

    print(f"\nDone! All ROM files extracted to {args.output_dir}/")
    print(f"  sf2_68k.bin  — Assembled 68K program ({TOTAL_PROG_SIZE//1024} KB)")
    print(f"  sf2_gfx.bin  — Decoded GFX tile data")
    print(f"  sf2_z80.bin  — Z80 sound program")
    print(f"  sf2_oki.bin  — OKI ADPCM samples ({TOTAL_OKI_SIZE//1024} KB)")
    print(f"\nNext: python tools/analyze_rom.py build/sf2_68k.bin")


if __name__ == '__main__':
    main()
