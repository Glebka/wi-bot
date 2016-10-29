set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 3.1.3)

project(uno-firmware C CXX)

set(ARDUINO_EXTRA_LIBRARIES_PATH ${CMAKE_SOURCE_DIR}../../thirdparty/arduino-libs)

link_directories(${ARDUINO_EXTRA_LIBRARIES_PATH})
include_directories(SYSTEM
	${ARDUINO_EXTRA_LIBRARIES_PATH}/Timer
	${ARDUINO_EXTRA_LIBRARIES_PATH}/Adafruit_PWMServoDriver
)

print_board_list()
print_programmer_list()

GENERATE_ARDUINO_FIRMWARE(uno-firmware
    SKETCH "firmware.ino"
    PORT  /dev/ttyACM0
    BOARD uno)
