import struct
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
input_folder = os.path.join(script_dir, "images_out")  # reads from images_out where .bin files are

for filename in os.listdir(input_folder):
    if filename.lower().endswith(".bin"):
        bin_path = os.path.join(input_folder, filename)
        var_name = os.path.splitext(filename)[0]  # e.g. "image1_BGR"
        h_name = var_name + ".h"
        h_path = os.path.join(input_folder, h_name)

        with open(bin_path, "rb") as f:
            data = f.read()

        pixels = struct.unpack(f"<{len(data)//2}H", data)  # ✅ little-endian to match converter

        with open(h_path, "w") as f:
            f.write("#pragma once\n")
            f.write(f"const uint16_t {var_name}[] = {{\n  ")  # ✅ no PROGMEM
            for i, p in enumerate(pixels):
                f.write(f"0x{p:04X},")
                if (i + 1) % 16 == 0:
                    f.write("\n  ")
            f.write("\n};\n")

        print(f"Written: {h_name}")

print("Done!")