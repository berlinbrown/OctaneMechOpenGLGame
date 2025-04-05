###----------------------------------------------
### Makefile for OctaneMech
### Camera movement demo (C Version)
### Berlin Brown
###
### 9/27/2007
###----------------------------------------------

OCTANE_MECH=mechcamera

OBJS =				\
	MechCamera.o		\

COMPILERFLAGS = -Wall

INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 

GCC = gcc
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBS=-lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

all: ${OCTANE_MECH}

###
### Compile based on the variables and targets below
###
.c.o:
	$(GCC) -O -I. $(INCLUDE) -c -Wall -o $@ $<

$(OCTANE_MECH): $(OBJS)
	$(GCC) $(CFLAGS) -o $@ $(OBJS) $(LIBDIR) $(LIBS)

###
### Target Clean
### 
clean:
	rm -vf *.o *.hi *.prof ${OCTANE_MECH}

###----------------------------------------------
### End of the Script
###----------------------------------------------