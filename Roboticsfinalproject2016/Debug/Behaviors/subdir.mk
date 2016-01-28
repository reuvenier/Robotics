################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Behaviors/AvoidObstacle.cpp \
../Behaviors/Behavior.cpp \
../Behaviors/GoForward.cpp \
../Behaviors/GoToWayPoint.cpp 

OBJS += \
./Behaviors/AvoidObstacle.o \
./Behaviors/Behavior.o \
./Behaviors/GoForward.o \
./Behaviors/GoToWayPoint.o 

CPP_DEPS += \
./Behaviors/AvoidObstacle.d \
./Behaviors/Behavior.d \
./Behaviors/GoForward.d \
./Behaviors/GoToWayPoint.d 


# Each subdirectory must supply rules for building sources it contributes
Behaviors/%.o: ../Behaviors/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/player-2.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


