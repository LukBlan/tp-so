################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/instrucciones/instruccion.c \
../src/instrucciones/lista_instrucciones.c 

C_DEPS += \
./src/instrucciones/instruccion.d \
./src/instrucciones/lista_instrucciones.d 

OBJS += \
./src/instrucciones/instruccion.o \
./src/instrucciones/lista_instrucciones.o 


# Each subdirectory must supply rules for building sources it contributes
src/instrucciones/%.o: ../src/instrucciones/%.c src/instrucciones/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/Consola/include" -I"/home/utnso/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/shared/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-instrucciones

clean-src-2f-instrucciones:
	-$(RM) ./src/instrucciones/instruccion.d ./src/instrucciones/instruccion.o ./src/instrucciones/lista_instrucciones.d ./src/instrucciones/lista_instrucciones.o

.PHONY: clean-src-2f-instrucciones

