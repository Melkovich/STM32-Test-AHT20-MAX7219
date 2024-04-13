################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../I2C_Temp/I2C.c \
../I2C_Temp/pb.c 

OBJS += \
./I2C_Temp/I2C.o \
./I2C_Temp/pb.o 

C_DEPS += \
./I2C_Temp/I2C.d \
./I2C_Temp/pb.d 


# Each subdirectory must supply rules for building sources it contributes
I2C_Temp/%.o I2C_Temp/%.su I2C_Temp/%.cyclo: ../I2C_Temp/%.c I2C_Temp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/33789/Desktop/Projet stm32/Workspace/F411RE_MAX7219_MATRICE_LED/MAX7219" -I"C:/Users/33789/Desktop/Projet stm32/Workspace/F411RE_MAX7219_MATRICE_LED/I2C_Temp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-I2C_Temp

clean-I2C_Temp:
	-$(RM) ./I2C_Temp/I2C.cyclo ./I2C_Temp/I2C.d ./I2C_Temp/I2C.o ./I2C_Temp/I2C.su ./I2C_Temp/pb.cyclo ./I2C_Temp/pb.d ./I2C_Temp/pb.o ./I2C_Temp/pb.su

.PHONY: clean-I2C_Temp

