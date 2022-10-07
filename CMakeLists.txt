cmake_minimum_required(VERSION 3.19)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# project settings
project(STM32L4R9AII6_TEMPLATE VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_EXE_LINKER arm-none-eabi-g++)
set(OBJCOPY arm-none-eabi-objcopy)
set(SIZE arm-none-eabi-size)

set(ELF_FILE ${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_NAME}.bin)
set(MAP_FILE ${PROJECT_NAME}.map)

# mpu
set(CPU cortex-m4)
set(FPU -mfpu=fpv4-sp-d16)
set(FLOAT_ABI -mfloat-abi=hard)
set(LDSCRIPT STM32L4R9AIIx_FLASH.ld)

# compiler options
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-Wall)
add_compile_options(-ffunction-sections)
add_compile_options(-fdata-sections)
add_compile_options(-fstack-usage)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif()

set(CMAKE_C_FLAGS "-mcpu=${CPU} -std=gnu11 ${FPU} ${FLOAT_ABI} -mthumb")
set(CMAKE_CXX_FLAGS "-mcpu=${CPU} -std=gnu++14 -fno-use-cxa-atexit ${FPU} ${FLOAT_ABI} -mthumb")
set(CMAKE_AS_FLAGS "-mcpu=${CPU} -x assembler-with-cpp ${FPU} ${FLOAT_ABI} -mthumb")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=${CPU} -T${CMAKE_CURRENT_SOURCE_DIR}/${LDSCRIPT} --specs=nosys.specs -u_printf_float -Wl,-Map=${MAP_FILE} -Wl,--print-memory-usage -Wl,--gc-sections -static --specs=nano.specs -u _scanf_float ${FPU} ${FLOAT_ABI} -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group")

enable_language(ASM)

# define
add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-DSTM32L4R9xx)
add_definitions(-DUSB_SERIAL_BAUD=1000000)

# # for Debug

# packages

# includes
include_directories(
    Core/Inc
    USB_DEVICE/App
    USB_DEVICE/Target
    Drivers/STM32L4xx_HAL_Driver/Inc
    Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    Drivers/CMSIS/Device/ST/STM32L4xx/Include
    Drivers/CMSIS/Include
    3rdParty/fibre/cpp/include
    3rdParty/fibre/cpp/include/fibre
    Bsp
    Bsp/communication
    Bsp/utils
    Bsp/utils/arm_math
    UserApp
    UserApp/Protocols
)

# sources
file(GLOB C_SRCS
    Core/Src/main.c
    Core/Src/gpio.c
    Core/Src/freertos.c
    Core/Src/dma.c
    Core/Src/fmc.c
    Core/Src/i2c.c
    Core/Src/sai.c
    Core/Src/sdmmc.c
    Core/Src/spi.c
    Core/Src/tim.c
    Core/Src/usart.c
    Core/Src/stm32l4xx_it.c
    Core/Src/stm32l4xx_hal_msp.c
    Core/Src/stm32l4xx_hal_timebase_tim.c
    USB_DEVICE/App/usb_device.c
    USB_DEVICE/App/usbd_desc.c
    USB_DEVICE/App/usbd_cdc_if.c
    USB_DEVICE/Target/usbd_conf.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_fmc.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sram.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_sdmmc.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
    Core/Src/system_stm32l4xx.c
    Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
)
file(GLOB_RECURSE CPP_SRCS
    "UserApp/*.*"
    "Bsp/*.*"
    "3rdParty/fibre/*.*"
)
file(GLOB ASM_SRCS
    startup_stm32l4r9xx.s
)

# executables
add_executable(${ELF_FILE} ${C_SRCS} ${CPP_SRCS} ${ASM_SRCS})

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${OBJCOPY} -Oihex ${ELF_FILE} ${HEX_FILE}
    COMMENT "Invoking: Cross ARM GNU Print Size"
)
