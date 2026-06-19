from PIL import Image
import struct
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
input_folder = os.path.join(script_dir, "images_in")
output_folder = os.path.join(script_dir, "images_out")
os.makedirs(output_folder, exist_ok=True)


permutations = {
    "RGB": lambda r,g,b: ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3),
}

for filename in os.listdir(input_folder):
    if filename.lower().endswith((".png", ".jpg", ".jpeg")):
        img = Image.open(os.path.join(input_folder, filename))
        img = img.convert("RGB").resize((176, 220), Image.LANCZOS)
        pixels = list(img.getdata())
        
        for name, fn in permutations.items():
            outname = os.path.splitext(filename)[0] + f"_{name}.bin"
            with open(os.path.join(output_folder, outname), "wb") as f:
                for r, g, b in pixels:
                    f.write(struct.pack("<H", fn(r, g, b)))
            print(f"Written: {outname}")

print("Done!")