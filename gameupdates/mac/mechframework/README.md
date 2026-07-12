# Mech Framework

This project is a stripped-down OpenGL framework extracted from UmbraMech for low-level testing.

Included:
- Camera orbit and zoom controls
- Ground plane + grid
- Starfield background
- Single test box object

Removed from runtime path:
- AI/ants systems
- Bot spawning and world simulation
- Menu/game-state logic

## Build (macOS)

```
mkdir -p build
cd build
cmake ..
cmake --build .
./bin/mechframework
```

## Controls

- A / D: rotate camera yaw
- Mouse drag: rotate camera yaw + vertical offset
- Mouse wheel or + / -: zoom camera distance
- O / L: vertical camera offset
- I / K: decrease/increase FOV
- R: reset camera values
- Esc or Q: quit
