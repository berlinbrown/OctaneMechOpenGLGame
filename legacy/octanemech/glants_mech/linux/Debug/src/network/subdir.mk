################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/network/clients.c \
../src/network/connect.c \
../src/network/msg.c \
../src/network/network.c \
../src/network/server.c \
../src/network/stats.c 

OBJS += \
./src/network/clients.o \
./src/network/connect.o \
./src/network/msg.o \
./src/network/network.o \
./src/network/server.o \
./src/network/stats.o 

C_DEPS += \
./src/network/clients.d \
./src/network/connect.d \
./src/network/msg.d \
./src/network/network.d \
./src/network/server.d \
./src/network/stats.d 


# Each subdirectory must supply rules for building sources it contributes
src/network/%.o: ../src/network/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


