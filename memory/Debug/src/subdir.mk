################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/memory_conexiones.c \
../src/recursos.c 

C_DEPS += \
./src/main.d \
./src/memory_conexiones.d \
./src/recursos.d 

OBJS += \
./src/main.o \
./src/memory_conexiones.o \
./src/recursos.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/memory/include" -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/shared/include" -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/Consola/src" -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/Cpu/src" -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/memory/include" -include"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/shared/include/estructuras.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/main.d ./src/main.o ./src/memory_conexiones.d ./src/memory_conexiones.o ./src/recursos.d ./src/recursos.o

.PHONY: clean-src

