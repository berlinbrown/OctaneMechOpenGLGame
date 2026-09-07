# Minimal engineering maintenance review

Reviewed the compiled C++ sources with Clang's static analyzer, inspected the
supporting headers and macOS audio/build integration, and addressed concrete
correctness defects. Archived code under `docs/older_code` is not compiled.
The existing C++17, fixed-function OpenGL, GLUT, and native audio approach remains.

## Corrections

- List deletion now updates the head before freeing it. Map destruction releases
  list nodes, and map construction respects capacity across both loops.
- Tree searches return recursive results and compare against the current node.
  The legacy tree test no longer writes beyond its array; printf types match.
- Bullet resets initialize the full slot. Reused bullets discard old wall hits,
  and a ray miss no longer produces an impact at an uninitialized location.
- Collision collection and texture access guard array bounds. Collision queries
  reject unsupported query types before reading uninitialized coordinates.
- Line-of-sight tests handle vertical edges without uninitialized slope reads;
  coincident AI targets keep their current heading instead of producing NaN.
- The fixed spatial grid uses integer indices for its 64 allocated cells.
- BMP loading validates 24-bit uncompressed, bottom-up files using fixed-width
  header fields, handles row padding, checks file length, and closes files on
  success and failure. Unsupported formats fail without changing the output.
- Formatted text uses bounded writes and const format strings. In-game help
  reflects the current controls.
- Timing uses a monotonic wall clock. FPS cannot divide by zero; paused frames
  do not advance elapsed game time, foliage, or pheromone lifetimes.
- Special-key callbacks check readiness and index bounds. Existing pause/input
  regression fixes remain covered.
- CMake's minimum version now matches its precompiled-header feature; the
  Objective-C++ audio target explicitly requires C++17.

## Verification

Build with `cmake -S . -B build` and `cmake --build build -j4`.
Run `ctest --test-dir build --output-on-failure`, or run the two Python scripts in
`tests` directly. Python 3 and the installed Apple Clang toolchain are required.
The core regression harness uses AddressSanitizer and UndefinedBehaviorSanitizer;
it exercises production functions without requiring a GLUT window. The input
harness exercises keyboard dispatch, including a missed key-release scenario.

The review deliberately does not convert the engine to a fixed simulation
step, replace rendering or memory ownership, or implement the legacy networking
stubs. Movement remains frame-based, as before. Static analysis still identifies
some unused assignments in legacy code; those are not runtime correctness errors.
Builds and focused tests do not replace a full interactive gameplay test.
