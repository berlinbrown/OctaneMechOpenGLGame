#
# Berlin Brown
# bigbinc@hotmail.com
#
# Makefile for glAnts
# under linux gcc-2.96
# Mandrake 8.2
#
NET_DIR = network
NET_INC = network/include
DEBUG_ON = -g
DEBUG_OFF = 
DEBUG_STATE = $(DEBUG_OFF)
LIBDIR = -L/usr/X11R6/lib
GCC = gcc
INCLUDE = -I/usr/include/
CFLAGS = -Wall $(INCLUDE)
LIBS = -lm -lX11 -lXi -lXmu -lGL -lGL -lGLU -lXxf86vm -lpthread

PROG	= glants

all: 	$(PROG)

# Object Files
OBJS = 	ant.o bitmap.o bot.o 			\
	camera.o collision.o commands.o cube.o	\
	fireants.o garden.o 			\
	glant.o gldrawlib.o globals.o grid.o	\
	lights.o list.o 			\
	los.o maps.o norm_cube.o objects.o	\
	octree.o particles.o 			\
	pheromone.o plist.o pyramid.o		\
	stars.o static.o text.o			\
	tree.o wirebox.o world.o		\
	keys.o					\
	play.o					\
	sound.o					\
	$(NET_DIR)/connect.o $(NET_DIR)/network.o \
	$(NET_DIR)/msg.o $(NET_DIR)/stats.o 	\
	$(NET_DIR)/server.o			\
	$(NET_DIR)/clients.o

$(PROG): $(OBJS)
	$(GCC) $(CFLAGS) -o $@ $(OBJS) $(LIBDIR) $(LIBS)

#
.c.o:
	$(GCC) -O -I. $(INCLUDE) $(DEBUG_STATE) -c -Wall -o $@ $<

#
# Clean
#
clean: 
	rm -f *.o *~ 
	rm -f $(PROG)
	rm -f $(NET_DIR)/*.o
	rm -f $(NET_DIR)/*~
	rm -f $(NET_INC)/*~

#
# Dependencies
#
ant.o: bot.h gldrawlib.h objects.h
bitmap.o: bot.h globals.h gldrawlib.h objects.h bot.h \
	menu.h camera.h fireants.h $(NET_INC)/connect.h
bot.o: bot.h camera.h objects.h gldrawlib.h world.h \
	globals.h octree.h plist.h collision.h
camera.o: bot.h camera.h gldrawlib.h globals.h fireants.h menu.h
collision.o: bot.h collision.h gldrawlib.h camera.h fireants.h
commands.o: bot.h 
cube.o: bot.h objects.h
fireants.o: bot.h gldrawlib.h objects.h lights.h bot.h camera.h \
	world.h particles.h collision.h globals.h camera.h \
	fireants.h menu.h $(NET_INC)/msg.h $(NET_INC)/connect.h sound.h \
	$(NET_DIR)/network.h
garden.o: bot.h gldrawlib.h globals.h
glant.o: bot.h gldrawlib.h objects.h camera.h bot.h world.h \
	lights.h globals.h list.h plist.h tree.h octree.h \
	particles.h maps.h world.h collision.h walls.h \
	fireants.h menu.h text.h keys.h $(NET_INC)/connect.h sound.h
gldrawlib.o: bot.h gldrawlib.h
globals.o: bot.h defaults.h gldrawlib.h globals.h camera.h \
		text.h menu.h keys.h fireants.h \
	$(NET_INC)/connect.h $(NET_INC)/clients.h
grid.o: bot.h objects.h
lights.o: bot.h camera.h objects.h gldrawlib.h lights.h \
		fireants.h
list.o: bot.h list.h
los.o: bot.h globals.h gldrawlib.h
maps.o: bot.h plist.h maps.h
norm_cube.o: bot.h objects.h gldrawlib.h lights.h
objects.o: bot.h lights.h objects.h bot.h gldrawlib.h fireants.h
octree.o: bot.h objects.h tree.h gldrawlib.h plist.h octree.h 
particles.o: bot.h particles.h
pheromone.o: bot.h globals.h gldrawlib.h objects.h octree.h
plist.o: bot.h plist.h
pyramid.o: bot.h gldrawlib.h objects.h lights.h plist.h \
	collision.h walls.h
stars.o: bot.h objects.h
static.o: bot.h globals.h camera.h objects.h
text.o: bot.h gldrawlib.h text.h globals.h
tree.o: bot.h tree.h
wirebox.o: bot.h gldrawlib.h objects.h 
world.o: bot.h gldrawlib.h objects.h world.h lights.h
keys.o: globals.h keys.h menu.h $(NET_INC)/connect.h
sound.o: sound.h
play.o: sound.h

$(NET_DIR)/connect.o: $(NET_INC)/connect.h keys.h $(NET_DIR)/network.h $(NET_INC)/msg.h \
		$(NET_INC)/clients.h
$(NET_DIR)/msg.o: $(NET_INC)/connect.h $(NET_INC)/msg.h $(NET_DIR)/network.h
$(NET_DIR)/network.o: $(NET_INC)/connect.h $(NET_INC)/msg.h $(NET_DIR)/network.h globals.h bot.h fireants.h
$(NET_DIR)/stats.o: $(NET_INC)/connect.h $(NET_INC)/msg.h $(NET_DIR)/network.h
$(NET_DIR)/server.o: $(NET_INC)/msg.h $(NET_DIR)/network.h $(NET_INC)/clients.h $(NET_INC)/connect.h bot.h globals.h
$(NET_DIR)/clients.o: $(NET_INC)/clients.h bot.h globals.h $(NET_INC)/msg.h $(NET_DIR)/network.h $(NET_INC)/connect.h
