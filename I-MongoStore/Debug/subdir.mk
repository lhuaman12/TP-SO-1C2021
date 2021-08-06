################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../archivos.c \
../bitmap.c \
../blocks.c \
../fsck.c \
../interfaz.c \
../md5.c \
../recibir_paquete.c \
../servidor.c 

OBJS += \
./archivos.o \
./bitmap.o \
./blocks.o \
./fsck.o \
./interfaz.o \
./md5.o \
./recibir_paquete.o \
./servidor.o 

C_DEPS += \
./archivos.d \
./bitmap.d \
./blocks.d \
./fsck.d \
./interfaz.d \
./md5.d \
./recibir_paquete.d \
./servidor.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2021-1c-bastardosSinGloria/SharedUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


