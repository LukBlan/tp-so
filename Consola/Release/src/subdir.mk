################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/consola_config.c \
../src/consola_utils.c \
../src/main.c 

C_DEPS += \
./src/consola_config.d \
./src/consola_utils.d \
./src/main.d 

OBJS += \
./src/consola_config.o \
./src/consola_utils.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/static/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/consola_config.d ./src/consola_config.o ./src/consola_utils.d ./src/consola_utils.o ./src/main.d ./src/main.o

.PHONY: clean-src

