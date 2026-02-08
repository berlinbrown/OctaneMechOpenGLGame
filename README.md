# Umbra Octane Mech

Octane Mech is a 3D, OpenGL based Mech game

## umbra mech - opengl based game

glAnts(Umbra Mech or Octane Mech) is an example OpenGL C++ project that simulates a mech-style game featuring 3D rendering and basic real-time networking. Originally written in C, it's a retro-style graphical demo showcasing object-based rendering, camera movement, particle effects, and rudimentary multiplayer capabilities. It's useful as a learning project for OpenGL, game architecture, and client-server networking concepts in C++.

## Updates 2025

See the most recent updates under the gameupdates directory

Older code can be found under "legacy"

## History

The game started around 1999 many years ago.  The goal was to run on Linux and Windows.  When OpenGL was coming out, to support graphics card including 3dfx cards.

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/screenshot_mech_game1.png" />

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/screenshot_mech_game2.png" />

## Remaster and Relearn Journey

As part of the remaster and relearn, I converted some of the code to compile and had to break the code down into small modules.

See the updates as some basic OpenGL in C/C++ examples

Navigate to the directory: https://github.com/berlinbrown/OctaneMechOpenGLGame/tree/master/retutorials

...

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/Screenshot 2025-05-02_1.png" />

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/Screenshot2025-05-02_2.png" />


Also see: http://code.google.com/p/octanemech/ 

## Developer guide 🔧

### Quick start (mac)  
- cd `gameupdates/mac/umbramech`  
- `make build` (wrapper for `cmake -S . -B build && cmake --build build`)  
- `make run` or `./build/bin/umbramech`  
- To build a debug build: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build`
- Notes: CMake links macOS frameworks (`-framework OpenGL -framework GLUT -framework Cocoa -framework IOKit -framework CoreVideo`). The project disables deprecated OpenGL warnings with `-Wno-deprecated-declarations`.

### Big-picture architecture 🧭
- Primary active code: `gameupdates/mac/umbramech/` — `include/` (headers), `src/` (implementation), `resources/` (config and assets).
  - Entry: `src/umbramain.cpp` (main loop, input, HUD, GL setup)
  - Globals & config: `include/globals.hpp` + `src/globals.cpp` (see `Load_ConfigFile()`)
  - Rendering helpers: `src/gldrawlib.cpp`, `src/objects.cpp`, `src/particles.cpp`
  - Game entities: `src/bot.cpp`, `src/fireants.cpp`, `src/DriverBots.cpp`
- Legacy code and platform variants live under `legacy/` and `octanemech/` (consult for older implementations and networking details).

### Project-specific conventions & patterns 📐
- Heavy use of a single global state: `AntGlobals *ant_globals` and many macros mapping fields (e.g., `#define BOT_SPEED ant_globals->bot_speed`). Search `ant_globals` to find where state is used/modified.
- C-style structs and global macros dominate; prefer minimally-invasive edits (follow existing style unless doing a focused refactor).
- Config-driven: `resources/config.ini` is parsed by `Load_ConfigFile()` and maps many runtime settings to `ant_globals` fields (look for `ID_*` constants in `include/globals.hpp`).
- UI/menu state and input mapping are centralized in `umbramain.cpp` with macros like `MENU_RUN_MODE` and functions such as `TogglePaused()` and `GameTick()`.
- Rendering uses the OpenGL fixed-function pipeline (immediate mode, `glBegin`/`glEnd`, `gluPerspective`) — expect deprecated APIs on modern macOS; porting should be incremental.

### Debugging & development tips 🧪
- There are no automated test suites; prefer manual builds and visual verification for rendering changes.
- Use `Super_Printf()` for in-game debug output and the `Super_*` helpers for text/UI debug features.
- When working cross-platform, inspect platform-specific blocks (X11/GLX vs macOS GLUT) before editing.

### Network & integration 🕸️
- Network/server/client code is available in legacy paths (e.g., `legacy/.../src/network/`). Consult these files for multiplayer behaviour and protocols.

### Contributing & style ✅
- A `.clang-format` is present at the repo root (Google-based with project tweaks) — run `clang-format -i` on changed C/C++ files.
- I applied code formatting across C/C++ sources on branch `style/clang-format` (recommended to review changes in a PR).

> Notes: Files include BSD-style license headers (Berlin Brown). Preserve license headers when editing files.

If you'd like, I can add a short "how to test" checklist or a developer CONTRIBUTING.md — which would you prefer?  
