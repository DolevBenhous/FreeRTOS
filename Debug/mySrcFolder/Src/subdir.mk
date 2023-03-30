################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mySrcFolder/Src/DB_MAIN.c \
../mySrcFolder/Src/Tools.c 

OBJS += \
./mySrcFolder/Src/DB_MAIN.o \
./mySrcFolder/Src/Tools.o 

C_DEPS += \
./mySrcFolder/Src/DB_MAIN.d \
./mySrcFolder/Src/Tools.d 


# Each subdirectory must supply rules for building sources it contributes
mySrcFolder/Src/%.o mySrcFolder/Src/%.su: ../mySrcFolder/Src/%.c mySrcFolder/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I"/home/dolev/Downloads/Shmulik_Atias_300987443_SE_FREE_RTOS_PROJ/FREE_RTOS_PROJ/mySrcFolder/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-mySrcFolder-2f-Src

clean-mySrcFolder-2f-Src:
	-$(RM) ./mySrcFolder/Src/DB_MAIN.d ./mySrcFolder/Src/DB_MAIN.o ./mySrcFolder/Src/DB_MAIN.su ./mySrcFolder/Src/Tools.d ./mySrcFolder/Src/Tools.o ./mySrcFolder/Src/Tools.su

.PHONY: clean-mySrcFolder-2f-Src

