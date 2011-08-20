###----------------------------------------------
### Makefile for OctaneMech
### 
### 9/20/2007
###----------------------------------------------

MISC_APP1=Test1
MISC_APP2=Test2
OCTANE_MECH=octanemech
MECH_TESTS=mechtests

HASKELL_SRC=MatrixUtils.hs	\
	MechDisplay.hs		\
	MechGLScreen.hs		\
	OctaneMech.hs		\
	MechKeys.hs		\
	BasicMechs.hs		\
	MechTerrain.hs

TEST_SRC=MechTests.hs		\
	Tests/GLTests.hs	\

###
### List of directories to be cleaned
### (edit accordingly to add to the list)
CLEAN_PROJ_DIRS = misc HUnit Tests

###
### Target Build Octane Mech
###
${OCTANE_MECH}: ${HASKELL_SRC}
	ghc --make OctaneMech.hs -prof -auto-all -o ${OCTANE_MECH}

${MECH_TESTS}: ${TEST_SRC}
	ghc --make MechTests -iHUnit -prof -auto-all -o ${MECH_TESTS}

###
### Target Clean
### 
clean:
	rm -vf *.o *.hi *.prof ${OCTANE_MECH} ${MECH_TESTS}
	rm -vf misc/${MISC_APP1} misc/${MISC_APP2}
	for ii in ${CLEAN_PROJ_DIRS} ; do 		\
		echo "cleaning dir= $$ii" ; 		\
		rm -vf $$ii/*.hi $$ii/*.o $ii/*.prof ;	\
	done

###----------------------------------------------
### End of the Script
###----------------------------------------------