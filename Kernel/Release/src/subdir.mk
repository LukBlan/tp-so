################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/kernel_conexiones.c \
../src/main.c \
../src/planificacion.c \
../src/recursos.c 

C_DEPS += \
./src/kernel_conexiones.d \
./src/main.d \
./src/planificacion.d \
./src/recursos.d 

OBJS += \
./src/kernel_conexiones.o \
./src/main.o \
./src/planificacion.o \
./src/recursos.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/kernel_conexiones.d ./src/kernel_conexiones.o ./src/main.d ./src/main.o ./src/planificacion.d ./src/planificacion.o ./src/recursos.d ./src/recursos.o

.PHONY: clean-src

