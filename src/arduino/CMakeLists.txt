#*************************************************************************
# WiBot Project
#*************************************************************************
# @file    CMakeLists.txt
# @date
# @author  Hlieb Romanov
# @brief   Arduino firmware build configuration (for Arduino Pro Mini 5V 16 MHz board with Optiboot)
#***********************************************************************/
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 3.1.3)

project(firmware C CXX)

set(ARDUINO_EXTRA_LIBRARIES_PATH ${CMAKE_SOURCE_DIR}/../../thirdparty/arduino-libs)

include_directories(${CMAKE_SOURCE_DIR}/../)
link_directories(${ARDUINO_EXTRA_LIBRARIES_PATH})

register_hardware_platform(${CMAKE_SOURCE_DIR}/platform)
print_board_list()

GENERATE_ARDUINO_FIRMWARE(firmware
    SKETCH "firmware.ino"
    PORT  /dev/ttyACM0
    BOARD promini
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/firmware.hex DESTINATION bin)