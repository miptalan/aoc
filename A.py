#!/usr/bin/env python3
"""
make_qr.py  file1.txt  file2.txt  ...

Creates frame_01.png … frame_NN.png,
each a QR code holding a slice of the payload.
"""
import sys, os, io, math, zlib, base64, zipfile, qrcode

# 1. Pack the input files into an in-memory ZIP
payload = io.BytesIO()
with zipfile.ZipFile(payload, "w", zipfile.ZIP_DEFLATED) as z:
    for path in sys.argv[1:]:
        z.write(path, arcname=os.path.basename(path))
blob = payload.getvalue()                                # raw bytes

# 2. Prepend a CRC-32 so the receiver can validate
crc  = zlib.crc32(blob) & 0xFFFFFFFF                     # 8 hex digits
header  = f"{crc:08X}\n".encode()                       # e.g. "9A3F2C17\n"
package = header + blob

# 3. Base64-encode (ASCII is QR-friendly), then slice
b64     = base64.b64encode(package).decode()
CHUNK   = 1800                                           # fits QR V20-L
total   = math.ceil(len(b64) / CHUNK)

for i in range(total):
    piece = b64[i*CHUNK : (i+1)*CHUNK]
    qrtxt = f"{i+1}/{total}|{piece}"                     # “index/total|data”
    img   = qrcode.make(qrtxt)
    img.save(f"frame_{i+1:02d}.png")

print(f"Created {total} QR images (frame_01.png … frame_{total:02d}.png)")
