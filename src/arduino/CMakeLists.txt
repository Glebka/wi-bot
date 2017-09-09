set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 3.1.3)

project(firmware C CXX)

set(ARDUINO_EXTRA_LIBRARIES_PATH ${CMAKE_SOURCE_DIR}/../../thirdparty/arduino-libs)

link_directories(${ARDUINO_EXTRA_LIBRARIES_PATH})

print_board_list()
print_programmer_list()

GENERATE_ARDUINO_FIRMWARE(firmware
    SKETCH "firmware.ino"
    PORT  /dev/ttyACM0
    BOARD promini)

# avrdude -Cavrdude.conf -v -patmega328p -carduino -PCOM4 -b57600 -D -Uflash:w:firmware.hex:i