################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Discordiador

dependents:
	-cd /home/utnso/shared-library-so/SharedUtils/Debug && $(MAKE) all

# Tool invocations
Discordiador: $(OBJS) $(USER_OBJS) /home/utnso/shared-library-so/SharedUtils/Debug/libSharedUtils.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "Discordiador" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Discordiador
	-@echo ' '

.PHONY: all clean dependents
/home/utnso/shared-library-so/SharedUtils/Debug/libSharedUtils.so:

-include ../makefile.targets
