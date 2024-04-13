################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MAX7219/MAX7219.c \
../MAX7219/MAX7219_Utility.c 

OBJS += \
./MAX7219/MAX7219.o \
./MAX7219/MAX7219_Utility.o 

C_DEPS += \
./MAX7219/MAX7219.d \
./MAX7219/MAX7219_Utility.d 


# Each subdirectory must supply rules for building sources it contributes
MAX7219/%.o MAX7219/%.su MAX7219/%.cyclo: ../MAX7219/%.c MAX7219/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/33789/Desktop/Projet stm32/Workspace/F411RE_MAX7219_MATRICE_LED/MAX7219" -I"C:/Users/33789/Desktop/Projet stm32/Workspace/F411RE_MAX7219_MATRICE_LED/I2C_Temp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MAX7219

clean-MAX7219:
	-$(RM) ./MAX7219/MAX7219.cyclo ./MAX7219/MAX7219.d ./MAX7219/MAX7219.o ./MAX7219/MAX7219.su ./MAX7219/MAX7219_Utility.cyclo ./MAX7219/MAX7219_Utility.d ./MAX7219/MAX7219_Utility.o ./MAX7219/MAX7219_Utility.su

.PHONY: clean-MAX7219

