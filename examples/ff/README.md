## FF Demo (`ff_demo`)

This example recreates the classic Final Fantasy-style GUI demo using FifeGUI with the SDL2 backend.

It demonstrates:
- custom themed widgets (`FFContainer`, `FFListBox`, `FFScrollArea`, `FFCharacterChooser`)
- menu navigation and panel switching (Items, Status, Skills, Magic, About)
- image/font based UI skinning
- sound feedback via `SDL2_mixer`

## Controls

- `Up` / `Down` - Navigate list entries
- `Enter` - Select current entry
- `Esc` - Return to the main menu / close overlays
- `F` - Toggle fullscreen
- `Quit` menu entry - Exit the demo

## Runtime assets

The demo expects these folders next to the executable:
- `images/`
- `sound/`

The CMake target already copies these folders after build and installs them with the executable.

## Build

From the repository root:

```bash
cmake -S . -B out/build
cmake --build out/build --target ff_demo
```

## Run

```bash
./out/build/examples/ff/ff_demo
```

In case of missing audio device, you can run with `SDL_AUDIODRIVER=dummy` to disable audio:

```bash
SDL_AUDIODRIVER=dummy ./out/build/examples/ff/ff_demo
```

## Dependencies

`ff_demo` requires SDL2 support in this project and links:
- SDL2
- SDL2_image
- SDL2_mixer
- `fifechan::fifechan`
- `fifechan::fifechan_sdl2`

# Original Authors

- Per Larsson (finalman) - code
- Olof Naessen (yakslem) - code, chars
- Tomas Almgren (peak) - font
- Henrik Vahlgren (haiko) - darkbits logo
