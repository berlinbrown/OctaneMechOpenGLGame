###----------------------------------------------
### Makefile for OctaneMech
### Camera movement demo
### Berlin Brown
###
### 9/27/2007
###----------------------------------------------

OCTANE_MECH=mechtext

HASKELL_SRC =			\
	DisplayLoop.hs		\
	MechTextDemo.hs		\
	MechGenericObjects.hs	\

all: ${OCTANE_MECH}

###
### Target Build Octane Mech
###
${OCTANE_MECH}: ${HASKELL_SRC}
	ghc --make MechTextDemo.hs -prof -auto-all -o $@

###
### Target Clean
### 
clean:
	rm -vf *.o *.hi *.prof ${OCTANE_MECH}
	rm -vf Text/*.o Text/*.hi *.prof

###----------------------------------------------
### End of the Script
###----------------------------------------------