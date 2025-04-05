=============================
==
== Octane Mech Haskell Example (initial framework)
== 9/22/2007
=============================

=============================
 Description
=============================
 The following source files included demostrate
 writing OpenGL games in the Haskell programming language.
 
 The initial code was based on the following blog article from "Jim"
 http://myawesomeblag.blogspot.com/2007/03/opengl-tetris-in-haskell.html

 The complete Octane Mech application will attempt to recreate
 "glants" mech game.
 
 http://glants.sourceforge.net/

=============================
 Compile and Running
=============================
 In order to build this application, you will need the latest
 version of GHC and OpenGL libraries installed
 
 With UBUNTU, the following freeglut opengl libraries were installed:
 sudo apt-get install freeglut3-dev
 sudo apt-get install libxi-dev
 sudo apt-get install libxmu-dev
 
 The current test build was built with the following version.
 "The Glorious Glasgow Haskell Compilation System, version 6.6.1"
 
 After GHC and the proper libraries are installed, you can simply
 invoke the "make" command at the unix prompt.
 
 $ make
 
 and then to launch the GL window (application):
 
 $ ./octanemech_ex 
 
 You should see the following:
 
 bbrown@houston:~/workspace/OctaneMech/src/example$ make
ghc --make OctaneMech.hs -o -prof -auto-all octanemech_ex
[1 of 9] Compiling ColorDefs        ( ColorDefs.hs, ColorDefs.o )
[2 of 9] Compiling MatrixUtils      ( MatrixUtils.hs, MatrixUtils.o )
[3 of 9] Compiling MechGenericObjects ( MechGenericObjects.hs, MechGenericObjects.o )
[4 of 9] Compiling MechTerrain      ( MechTerrain.hs, MechTerrain.o )
[5 of 9] Compiling BasicMechs       ( BasicMechs.hs, BasicMechs.o )
[6 of 9] Compiling MechDisplay      ( MechDisplay.hs, MechDisplay.o )
[7 of 9] Compiling MechKeys         ( MechKeys.hs, MechKeys.o )
[8 of 9] Compiling MechGLScreen     ( MechGLScreen.hs, MechGLScreen.o )
[9 of 9] Compiling Main             ( OctaneMech.hs, OctaneMech.o )
Linking octanemech_ex ...
 
 
 