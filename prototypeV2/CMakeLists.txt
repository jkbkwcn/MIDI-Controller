cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(Prototyp)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# rest of your project

if (TARGET tinyusb_device)
    add_executable(main
            main.c
            usb_descriptors.c
            keypad.c
            buffer.c
            adc.c
            midi.c
            )

            target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR})

    # pull in common dependencies
    target_link_libraries(main PRIVATE
        pico_stdlib
        tinyusb_device
        tinyusb_board
        hardware_adc
        hardware_timer
        )

    # enable usb output, disable uart output
    pico_enable_stdio_usb(main 1)
    pico_enable_stdio_uart(main 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(main)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
