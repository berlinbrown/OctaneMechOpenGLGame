###----------------------------------------------
### Makefile for OctaneMech
### Camera movement demo
### Berlin Brown
###
### 9/27/2007
###----------------------------------------------

OCTANE_MECH=mechcamera

HASKELL_SRC =			\
	DisplayLoop.hs		\
	MechCamera.hs		\
	MechGenericObjects.hs	\

all: ${OCTANE_MECH}

###
### Target Build Octane Mech
###
${OCTANE_MECH}: ${HASKELL_SRC}
	ghc --make MechCamera.hs -prof -auto-all -o $@

###
### Target Clean
### 
clean:
	rm -vf *.o *.hi *.prof ${OCTANE_MECH}

###----------------------------------------------
### End of the Script
###----------------------------------------------