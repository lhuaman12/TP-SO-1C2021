################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../archivos.c \
../bitmap.c \
../blocks.c \
../interfaz.c \
../servidor.c 

OBJS += \
./archivos.o \
./bitmap.o \
./blocks.o \
./interfaz.o \
./servidor.o 

C_DEPS += \
./archivos.d \
./bitmap.d \
./blocks.d \
./interfaz.d \
./servidor.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/shared-library-so/SharedUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


