# Copilot instructions for OctaneMechOpenGLGame

Purpose: short, actionable guidance to make AI coding agents productive immediately in this repo.

## Quick summary ✅
- Language: C++ (C-style legacy code), uses OpenGL/GLUT, partially modernized to C++17.
- Main mac target: `gameupdates/mac/umbramech` (build with CMake/Makefile wrapper).
- Old platform variants and network code live in `legacy/` and `octanemech/` (use for reference).

## How to build & run (mac) 🔧
- Build (from project root):
  - cd `gameupdates/mac/umbramech`
  - `make build` (wrapper for `cmake -S . -B build && cmake --build build`)
- Run: `make run` or `./build/bin/umbramech`
- CMake notes: project uses C++17 and links macOS frameworks (`-framework OpenGL -framework GLUT ...`). CMake disables deprecated warnings with `-Wno-deprecated-declarations`.

## Big-picture architecture & important files 🧭
- `gameupdates/mac/umbramech/` — primary current code: `include/` (headers), `src/` (implementation), `resources/` (config and assets).
  - Entry / main logic: `src/umbramain.cpp` (game loop, input, HUD, GL setup)
  - Globals & config: `include/globals.hpp` + `src/globals.cpp` (see `Load_ConfigFile()`)
  - Rendering helpers: `src/gldrawlib.cpp`, `src/objects.cpp`, `src/particles.cpp`
  - Game entities: `src/bot.cpp`, `src/fireants.cpp`, `src/DriverBots.cpp`
- Legacy network/server/client code: `legacy/.../src/network/` — consult for multiplayer behaviour and protocol if needed.

## Project-specific conventions & patterns 📐
- Heavy use of a single global state: `AntGlobals *ant_globals` and many macros mapping fields (e.g., `#define BOT_SPEED ant_globals->bot_speed`). Search `ant_globals` to find where state is used/modified.
- C-style structs and global macros are common; prefer minimally-invasive changes when editing (follow existing style unless doing a focused refactor).
- Config-driven: `resources/config.ini` is read by `Load_ConfigFile()`; many runtime settings map to `ant_globals` fields and `ID_*` constants.
- UI/menu state and input mapping are centralized in `umbramain.cpp` with macros like `MENU_RUN_MODE`, `MENU_SETTINGS_MODE`.
- OpenGL usage: fixed-function pipeline (immediate mode, `glBegin`/`glEnd`, matrix stack, `gluPerspective`) — expect deprecated APIs on modern macOS.

## Typical tasks & where to start 🔎
- Change gameplay parameter: update `include/globals.hpp` macros/constants or implement new config keys and extend `Load_ConfigFile()` + `resources/config.ini`.
- Fix rendering/port to modern GL: search for `glBegin`, `glMatrixMode`, `gluPerspective`, `glTranslatef` and modernize regions incrementally.
- Add logging or debug prints: use existing `Super_Printf()` or add localized debug functions; the code uses `Super_*` helpers for text/UI output.

## Debugging & build pitfalls ⚠️
- Code contains platform-specific (X11/GLX) remnants and macOS GLUT paths — inspect platform blocks before compiling cross-platform.
- No automated tests detected in repo; rely on manual run/dev builds and visual verification.
- Build flags: to enable debugging build `-DCMAKE_BUILD_TYPE=Debug`.

## Useful search queries (examples) 🔍
- `ant_globals` — find state uses and config mappings
- `Load_ConfigFile` — where `resources/config.ini` is parsed/written
- `glBegin|gluPerspective|glMatrixMode` — find legacy GL usage
- `MENU_` / `TogglePaused` / `GameTick` — game loop and UI control

> Notes: Files include BSD-style license headers (Berlin Brown). Prefer preserving license headers when editing files.

If any section is unclear or you'd like more detail for a particular component (network, rendering modernization, or config upgrade), tell me which area to expand and I'll iterate. 👩‍💻✨
