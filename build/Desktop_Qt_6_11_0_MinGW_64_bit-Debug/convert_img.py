"""
Image format converter — loads any format via PIL/Pillow and outputs raw RGBA bytes to stdout.
Used by the C++ app as a fallback when Qt's built-in image plugins can't handle a format.
Output format: first 8 bytes = width (uint32 LE) + height (uint32 LE), then RGBA pixel data.
"""
import sys
import struct
import os

try:
    from PIL import Image
except ImportError:
    print("ERROR:PIL not installed. Run: pip install Pillow", file=sys.stderr)
    sys.exit(1)

input_path = sys.argv[1]

if not os.path.exists(input_path):
    print(f"ERROR:file not found: {input_path}", file=sys.stderr)
    sys.exit(2)

try:
    img = Image.open(input_path)
    img = img.convert("RGBA")
    w, h = img.size

    # Write header: width (4 bytes LE) + height (4 bytes LE)
    sys.stdout.buffer.write(struct.pack("<II", w, h))
    # Write pixel data
    sys.stdout.buffer.write(img.tobytes())
    sys.stdout.buffer.flush()
except Exception as e:
    print(f"ERROR:{e}", file=sys.stderr)
    sys.exit(3)
