################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../estructurasMemoria.c \
../interfaz.c \
../mapa.c \
../miram.c \
../paginacion.c \
../segmentacion.c 

OBJS += \
./estructurasMemoria.o \
./interfaz.o \
./mapa.o \
./miram.o \
./paginacion.o \
./segmentacion.o 

C_DEPS += \
./estructurasMemoria.d \
./interfaz.d \
./mapa.d \
./miram.d \
./paginacion.d \
./segmentacion.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/shared-library-so/SharedUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


