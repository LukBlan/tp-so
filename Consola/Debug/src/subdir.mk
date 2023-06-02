################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/consola_conexiones.c \
../src/consola_utils.c \
../src/main.c \
../src/recursos.c 

C_DEPS += \
./src/consola_conexiones.d \
./src/consola_utils.d \
./src/main.d \
./src/recursos.d 

OBJS += \
./src/consola_conexiones.o \
./src/consola_utils.o \
./src/main.o \
./src/recursos.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/Consola/include" -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/shared/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/consola_conexiones.d ./src/consola_conexiones.o ./src/consola_utils.d ./src/consola_utils.o ./src/main.d ./src/main.o ./src/recursos.d ./src/recursos.o

.PHONY: clean-src

