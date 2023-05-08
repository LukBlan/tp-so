################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/socket/cliente.c \
../src/socket/servidor.c 

C_DEPS += \
./src/socket/cliente.d \
./src/socket/servidor.d 

OBJS += \
./src/socket/cliente.o \
./src/socket/servidor.o 


# Each subdirectory must supply rules for building sources it contributes
src/socket/%.o: ../src/socket/%.c src/socket/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Desktop/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/shared/include" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-socket

clean-src-2f-socket:
	-$(RM) ./src/socket/cliente.d ./src/socket/cliente.o ./src/socket/servidor.d ./src/socket/servidor.o

.PHONY: clean-src-2f-socket

