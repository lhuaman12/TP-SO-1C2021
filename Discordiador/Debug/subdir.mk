################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../consola.c \
../discordiador.c \
../estructuras.c \
../paquetes.c 

OBJS += \
./consola.o \
./discordiador.o \
./estructuras.o \
./paquetes.o 

C_DEPS += \
./consola.d \
./discordiador.d \
./estructuras.d \
./paquetes.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2021-1c-bastardosSinGloria/SharedUtils" -I"/home/utnso/tp-2021-1c-bastardosSinGloria/SharedUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


