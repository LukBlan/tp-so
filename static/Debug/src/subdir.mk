################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/estructuras.c 

C_DEPS += \
./src/estructuras.d 

OBJS += \
./src/estructuras.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/static/include" -include"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/static/include/socket/cliente.h" -include"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/static/include/estructuras.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/estructuras.d ./src/estructuras.o

.PHONY: clean-src

