cmake_minimum_required(VERSION 3.1.3)

set(CMAKE_TOOLCHAIN_FILE ${PROJECT_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
link_directories(${PROJECT_SOURCE_DIR}/thirdparty/arduino-libs)

project(uno-firmware C CXX)

generate_arduino_firmware(uno-firmware
    SKETCH firmware.ino
    PORT  /dev/ttyACM0
    BOARD uno
)
