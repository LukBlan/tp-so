################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../include/socket/cliente.c \
../include/socket/servidor.c 

C_DEPS += \
./include/socket/cliente.d \
./include/socket/servidor.d 

OBJS += \
./include/socket/cliente.o \
./include/socket/servidor.o 


# Each subdirectory must supply rules for building sources it contributes
include/socket/%.o: ../include/socket/%.c include/socket/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-include-2f-socket

clean-include-2f-socket:
	-$(RM) ./include/socket/cliente.d ./include/socket/cliente.o ./include/socket/servidor.d ./include/socket/servidor.o

.PHONY: clean-include-2f-socket

