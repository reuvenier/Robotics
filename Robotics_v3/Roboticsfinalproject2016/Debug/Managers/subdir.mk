################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Managers/ConfigurationManager.cpp \
../Managers/Manager.cpp 

OBJS += \
./Managers/ConfigurationManager.o \
./Managers/Manager.o 

CPP_DEPS += \
./Managers/ConfigurationManager.d \
./Managers/Manager.d 


# Each subdirectory must supply rules for building sources it contributes
Managers/%.o: ../Managers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/player-2.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


