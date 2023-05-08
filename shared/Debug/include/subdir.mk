################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../include/estructuras.c 

C_DEPS += \
./include/estructuras.d 

OBJS += \
./include/estructuras.o 


# Each subdirectory must supply rules for building sources it contributes
include/%.o: ../include/%.c include/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-include

clean-include:
	-$(RM) ./include/estructuras.d ./include/estructuras.o

.PHONY: clean-include

