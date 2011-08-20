################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ant.c \
../src/bitmap.c \
../src/bot.c \
../src/camera.c \
../src/collision.c \
../src/commands.c \
../src/cube.c \
../src/fireants.c \
../src/garden.c \
../src/glant.c \
../src/gldrawlib.c \
../src/globals.c \
../src/grid.c \
../src/keys.c \
../src/lights.c \
../src/list.c \
../src/los.c \
../src/maps.c \
../src/norm_cube.c \
../src/objects.c \
../src/octree.c \
../src/particles.c \
../src/pheromone.c \
../src/play.c \
../src/plist.c \
../src/pyramid.c \
../src/sound.c \
../src/stars.c \
../src/static.c \
../src/text.c \
../src/tree.c \
../src/wirebox.c \
../src/world.c 

OBJS += \
./src/ant.o \
./src/bitmap.o \
./src/bot.o \
./src/camera.o \
./src/collision.o \
./src/commands.o \
./src/cube.o \
./src/fireants.o \
./src/garden.o \
./src/glant.o \
./src/gldrawlib.o \
./src/globals.o \
./src/grid.o \
./src/keys.o \
./src/lights.o \
./src/list.o \
./src/los.o \
./src/maps.o \
./src/norm_cube.o \
./src/objects.o \
./src/octree.o \
./src/particles.o \
./src/pheromone.o \
./src/play.o \
./src/plist.o \
./src/pyramid.o \
./src/sound.o \
./src/stars.o \
./src/static.o \
./src/text.o \
./src/tree.o \
./src/wirebox.o \
./src/world.o 

C_DEPS += \
./src/ant.d \
./src/bitmap.d \
./src/bot.d \
./src/camera.d \
./src/collision.d \
./src/commands.d \
./src/cube.d \
./src/fireants.d \
./src/garden.d \
./src/glant.d \
./src/gldrawlib.d \
./src/globals.d \
./src/grid.d \
./src/keys.d \
./src/lights.d \
./src/list.d \
./src/los.d \
./src/maps.d \
./src/norm_cube.d \
./src/objects.d \
./src/octree.d \
./src/particles.d \
./src/pheromone.d \
./src/play.d \
./src/plist.d \
./src/pyramid.d \
./src/sound.d \
./src/stars.d \
./src/static.d \
./src/text.d \
./src/tree.d \
./src/wirebox.d \
./src/world.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


