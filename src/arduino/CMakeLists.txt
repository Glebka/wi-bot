set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 3.1.3)

project(uno-firmware C CXX)

set(ARDUINO_LIBS_ROOT ${CMAKE_SOURCE_DIR}../../thirdparty/arduino-libs)

link_directories(${ARDUINO_LIBS_ROOT})
include_directories(SYSTEM
	${ARDUINO_LIBS_ROOT}/Timer
	${ARDUINO_LIBS_ROOT}/Adafruit_PWMServoDriver
)

print_board_list()
print_programmer_list()

GENERATE_ARDUINO_FIRMWARE(uno-firmware
    SKETCH "firmware.ino"
    PORT  /dev/ttyACM0
    BOARD uno)
