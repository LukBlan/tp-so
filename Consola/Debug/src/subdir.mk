################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/consola_instruccion.c \
../src/consola_recursos.c \
../src/consola_utils.c \
../src/main.c 

C_DEPS += \
./src/consola_instruccion.d \
./src/consola_recursos.d \
./src/consola_utils.d \
./src/main.d 

OBJS += \
./src/consola_instruccion.o \
./src/consola_recursos.o \
./src/consola_utils.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Desktop/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/Consola/include" -I"/home/utnso/Desktop/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/shared/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/consola_instruccion.d ./src/consola_instruccion.o ./src/consola_recursos.d ./src/consola_recursos.o ./src/consola_utils.d ./src/consola_utils.o ./src/main.d ./src/main.o

.PHONY: clean-src

