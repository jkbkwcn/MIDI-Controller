cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(OLED_SH1106_I2C)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (TARGET tinyusb_device)
        include_directories(${PROJECT_SOURCE_DIR})

        add_executable(OLED_SH1106_I2C
                main.c
                menu.c
                encoder.c
                OLED_I2C.c
                adc.c
                buffer.c
                keypad.c
                midi.c
                usb_descriptors.c
                pwm.c
                metronome.c
                )

        target_include_directories(OLED_SH1106_I2C PRIVATE ${CMAKE_CURRENT_LIST_DIR})

        # pull in common dependencies and additional i2c hardware support
        target_link_libraries(OLED_SH1106_I2C
                pico_stdlib     
                tinyusb_device
                tinyusb_board
                hardware_i2c
                hardware_adc
                hardware_timer
                hardware_pwm
                hardware_irq
                )

        # enable usb output, disable uart output
        pico_enable_stdio_usb(OLED_SH1106_I2C 1)
        pico_enable_stdio_uart(OLED_SH1106_I2C 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(OLED_SH1106_I2C)

        string(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,--print-memory-usage")

elseif(PICO_ON_DEVICE)
        message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
