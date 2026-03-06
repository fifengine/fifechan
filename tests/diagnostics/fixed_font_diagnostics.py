from __future__ import annotations

import argparse
from pathlib import Path


KNOWN_FONT_MAPS: dict[str, str] = {
    "fixedfont.bmp": " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",
    "rpgfont.png": " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#[]\"",
}


def _load_image(path: Path):
    try:
        from PIL import Image
    except ImportError as exc:
        raise SystemExit(
            "Pillow is required. Install with: pip install pillow"
        ) from exc

    if not path.exists():
        raise SystemExit(f"Image not found: {path}")

    return Image.open(path).convert("RGBA")


def analyze_font_sheet(path: Path, max_runs: int = 8) -> None:
    img = _load_image(path)
    width, height = img.size
    px = img.load()

    sep = px[0, 0]

    top_counts: dict[tuple[int, int, int, int], int] = {}
    for x in range(width):
        color = px[x, 0]
        top_counts[color] = top_counts.get(color, 0) + 1

    full_separator_rows: list[int] = []
    for y in range(height):
        row_is_sep = True
        for x in range(width):
            color = px[x, y]
            if color[:3] != sep[:3]:
                row_is_sep = False
                break
        if row_is_sep:
            full_separator_rows.append(y)

    runs: list[dict[str, int]] = []
    x = 0
    while x < width:
        while x < width and px[x, 0][:3] == sep[:3]:
            x += 1
        if x >= width:
            break

        start = x
        while x < width and px[x, 0][:3] != sep[:3]:
            x += 1
        end = x - 1

        black = 0
        for yy in range(height):
            for xx in range(start, end + 1):
                c = px[xx, yy]
                if c[0] == 0 and c[1] == 0 and c[2] == 0:
                    black += 1

        runs.append({"start": start, "width": end - start + 1, "black": black})

    print(f"path={path}")
    print(f"size={width}x{height}")
    print(f"pixel00={sep[0]},{sep[1]},{sep[2]},{sep[3]}")
    print(f"topRowUnique={len(top_counts)}")

    top5 = sorted(top_counts.items(), key=lambda item: item[1], reverse=True)[:5]
    for color, count in top5:
        print(f"topColor {color[0]},{color[1]},{color[2]},{color[3]} count={count}")

    rows_text = ",".join(str(y) for y in full_separator_rows)
    print(f"fullSeparatorRows={rows_text}")

    print(f"glyphRuns={len(runs)}")
    for i, run in enumerate(runs[:max_runs]):
        print(f"run{i} start={run['start']} width={run['width']} black={run['black']}")

    filename = path.name.lower()
    if filename in KNOWN_FONT_MAPS:
        chars = KNOWN_FONT_MAPS[filename]
        print(f"charMapLength={len(chars)}")
        print(f"allChars={chars}")
        # print("charMapIndexed:")
        # for idx, ch in enumerate(chars):
        #     if ch == " ":
        #         shown = "<space>"
        #     else:
        #         shown = ch
        #     print(f"  {idx:03d}: {shown}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Analyze ImageFont-style bitmap font sheets")
    parser.add_argument(
        "images",
        nargs="*",
        default=["tests/resources/fixedfont.bmp", "tests/resources/rpgfont.png"],
        help="One or more font image paths (default: tests/resources/fixedfont.bmp tests/resources/rpgfont.png)",
    )
    parser.add_argument(
        "--max-runs",
        type=int,
        default=8,
        help="How many glyph runs to print (default: 8)",
    )
    args = parser.parse_args()

    for index, image_path in enumerate(args.images):
        if index > 0:
            print()
        analyze_font_sheet(Path(image_path), max_runs=args.max_runs)
