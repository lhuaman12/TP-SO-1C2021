################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../discordiador/discordiador.c \
../discordiador/utils.c 

OBJS += \
./discordiador/discordiador.o \
./discordiador/utils.o 

C_DEPS += \
./discordiador/discordiador.d \
./discordiador/utils.d 


# Each subdirectory must supply rules for building sources it contributes
discordiador/%.o: ../discordiador/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


