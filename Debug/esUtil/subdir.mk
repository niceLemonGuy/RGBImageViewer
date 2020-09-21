################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../esUtil/esShader.c \
../esUtil/esShapes.c \
../esUtil/esTransform.c \
../esUtil/esUtil.c 

OBJS += \
./esUtil/esShader.o \
./esUtil/esShapes.o \
./esUtil/esTransform.o \
./esUtil/esUtil.o 

C_DEPS += \
./esUtil/esShader.d \
./esUtil/esShapes.d \
./esUtil/esTransform.d \
./esUtil/esUtil.d 


# Each subdirectory must supply rules for building sources it contributes
esUtil/%.o: ../esUtil/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


