INCLPATHS := -I. -I./core -I./core/gpio -I./core/can -I./core/core -I./core/printf -I./core/speedo -I./cmsis -I./cmsis/inc -I./cmsis/device -I./cmsis/device/src -I./cmsis/device/inc -I./out 
CC_SRC    := ./core/printf/printf.c ./cmsis/device/src/system_stm32f0xx.c   
CXX_SRC   := ./main.cpp ./core/gpio/gpio.cpp ./core/can/can.cpp ./core/core/timing.cpp ./core/core/errors.cpp ./core/rcc.cpp ./core/speedo/speedo.cpp 
AS_SRC    := ./cmsis/device/src/startup_stm32f042x6.s   
LDSCRIPT  := ./cmsis/device/STM32F042F4Px_FLASH.ld 
