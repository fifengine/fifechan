# Keycode Header Generator

Regenerate `key_gen.h` from a local SDL3 installation:

```bash
python3 tools/keygen/generate.py /path/to/SDL3/include/SDL3/SDL_keycode.h -o /path/to/key_gen.h
```

Example using vcpkg:

```bash
python3 tools/keygen/generate.py vcpkg_installed/x64-linux/include/SDL3/SDL_keycode.h -o include/fifechan/key_gen.h
```

## Scripts

- **`parse_sdl3.py`** — Reads `SDL_keycode.h` (plus `SDL_version.h` from the same directory) and extracts all `SDLK_*` macro definitions, outputting them as JSON to stdout.

- **`generate_enum.py`** — Reads the JSON from stdin (as produced by `parse_sdl3.py`) and writes a C++ header to stdout. The header contains a `fcn::Key` class with a `KeyType` enum mirroring all SDL3 keycodes, plus a `static_assert` verifying the enum is 32-bit.

- **`generate.py`** — Convenience wrapper that chains `parse_sdl3.py` and `generate_enum.py` together. Takes a path to `SDL_keycode.h` and an output path (`-o`). This is the recommended one-step invocation.
