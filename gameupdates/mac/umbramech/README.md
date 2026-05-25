# Octane Mech

Octane Mech is a 3D, OpenGL based Mech game.  This variant is built for mac with OpenGL

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/screenshot_mech_game1.png" />

<img src="https://github.com/berlinbrown/OctaneMechOpenGLGame/raw/master/octanemech/glAntsMechGameWinNew/media/screenshot_mech_game2.png" />

Also see: http://code.google.com/p/octanemech/ 

# Updated Keys

Move mech: arrow keys
Rotate camera around player: F / S
Reset camera rotation: R
Toggle view mode (first/third): Tab
Pause: P
Menu back/escape: Esc
Quit: Q
Menu selection: Up/Down + Enter or Space
Zoom:

Mouse up/down changes zoom (camera distance) in third-person mode.
O raises camera height, L lowers camera height (your temporary debug camera offset).
K increases FOV (acts like zooming out); there is currently no matching key to zoom back in via FOV.
If you want, I can add clean zoom keys next:

= / - for in/out zoom distance
[ / ] for FOV in/out
Keep mouse zoom too, with tighter limits so it feels controlled.

# Deprecation

With mac, Using deprecated API's which is part of the OpenGL fixed-function pipeline, and Apple has deprecated it starting in macOS 10.14 (Mojave).
