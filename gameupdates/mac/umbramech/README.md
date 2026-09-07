# Octane Mech

Octane Mech is a 3D, OpenGL based Mech game.  This variant is built for mac with OpenGL

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/screenshot_mech_game1.png" />

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/screenshot_mech_game2.png" />

Also see: http://code.google.com/p/octanemech/ 

# Controls and startup

The game opens in the paused menu with a raised third-person view over the mech.
Press P to begin, or select New Game with Enter. Spawns reject building footprints with a
6-unit clearance, including new-game resets.

- Move: arrow keys; shoot: Space.
- Cycle third-person / first-person / demo / world views: Tab (also while paused).
- Orbit camera: F / S, A / D, or drag with the left mouse button. Reset orbit: R.
- Zoom distance: + / - or mouse wheel (third-person starts 40 units behind).
- Camera height: O / L; field of view: K / I.
- Pause/unpause: P (also resumes from the startup menu or help); menu/back/resume: Esc; Enter also resumes a P-paused game; quit: Q.
- Menu selection: Up / Down and Enter or Space.
- Mute/unmute all audio: M.

Music plays quietly in the menu. The motor plays while moving, and shooting
uses overlapping sound effects. Audio is bundled for offline playback; see
[asset credits](resources/audio/CREDITS.md).

Build and run:

```sh
cmake -S . -B build
cmake --build build -j4
./build/bin/umbramech
```

# Deprecation

With mac, Using deprecated API's which is part of the OpenGL fixed-function pipeline, and Apple has deprecated it starting in macOS 10.14 (Mojave).

# Maintenance checks

After building, run `ctest --test-dir build --output-on-failure` to check input,
containers, geometry, projectile reset, clock, and BMP loading. The core checks
use AddressSanitizer and UndefinedBehaviorSanitizer. See the
[engineering review](docs/ENGINEERING_REVIEW.md) for scope and limitations.
