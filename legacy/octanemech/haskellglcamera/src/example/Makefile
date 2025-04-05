###----------------------------------------------
### Makefile for OctaneMech
### 
### 9/20/2007
###----------------------------------------------

MISC_APP1=Test1
MISC_APP2=Test2

OCTANE_MECH=octanemech_ex

HASKELL_SRC=MatrixUtils.hs	\
	MechDisplay.hs		\
	MechGLScreen.hs		\
	OctaneMech.hs		\
	MechKeys.hs		\
	BasicMechs.hs		\
	MechTerrain.hs

###
### Target Build Octane Mech
###
${OCTANE_MECH}: ${HASKELL_SRC}
	ghc --make OctaneMech.hs -o -prof -auto-all ${OCTANE_MECH}

### Package the application
package: clean
	rm -Rfv build
	mkdir -vp build
	tar -cvf build/octanemech_exampl.tar Makefile *.hs *.txt
	gzip build/octanemech_exampl.tar

###
### Target Clean
### 
clean:
	rm -vf *.o *.hi ${OCTANE_MECH}

###----------------------------------------------
### End of the Script
###----------------------------------------------