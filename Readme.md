# Wi-Bot [![Build Status](https://travis-ci.org/Glebka/wi-bot.svg?branch=master)](https://travis-ci.org/Glebka/wi-bot)

Wi-Bot is a robotic platform powered by TP-LINK TL MR3020 wireless router with OpenWRT installed on it and 
Arduino board for communicating with peripheral devices.

## Hardware

### TP LINK TL MR3020 router
It is a robot's brain. This is low cost router perfect for DIY robotics and customization.

* SoC: Atheros AR9330 rev 1 400 MHz
* 32 MiB of RAM
* 4 MiB of flash memory
* 802.11 b/g/n 150 Mbps
* Powered via USB B-Mini (5 Volts)
* Tiny form factor
    * 5.7 cm x 5.7 cm PCB
    * 6.7 cm x 7.4 cm x 2.2 cm case
* On board USB port
* UART pins for debugging and interfacing with other devices

### Arduino Uno

Arduino - spinal cord of Wi-Bot. It controls 2 servo motors for rotating the camera 
and two DC motors for each wheel. Arduino communicates with router using UART.

### USB Web Camera
Generic UVC camera that produces raw video stream, connected to USB port on the router.

## Software

### OpenWRT Chaos Calmer (15.05)
It is a tiny linux distribution for embedded devices, especially for routers.
I use it as a base system for Wi-Bot specific applications like Command and Control Server 
and mjpg-streamer for streaming video from web-cam.

### Command and Control Server
It is a C application for OpenWRT that opens TCP socket and listens for commands from 
Wi-Bot Console desktop application. It simply transmits received commands via UART to Arduino Uno firmware.

You can find sources of the server in ``src/cmd-server/`` subdirectory.

### mjpeg-streamer with custom output plugin
 
For video streaming I decided to use mjpeg-streamer application that is provided as a binary package for 
OpenWRT. But for video output I developed custom plugin that listens for a TCP connection and then transmits 
jpeg-compressed frames to the client.

You can find sources of the plugin here: ``src/mjpgstreamer-plugins/output_rawtcp/``

### Arduino Firmware

Simply reads commands from UART, decodes them and controls servo motors for rotating the camera (for now).
Find sources here: ``src/arduino/``

### Wi-Bot Console

Qt QML application running on Linux and Windows. Now it allows user to view video from Wi-Bot's camera and control 
camera position using mouse.
Find sources here: ``src/wibot-console/``

## How to build
 
On Linux:
1. Install dependencies:

        # apt-get update -qq 
        # apt-get install --no-install-recommends git subversion make cmake gcc pkg-config build-essential libssl-dev libncurses5-dev unzip gawk wget python file
        # apt-get install libssl-dev libffi-dev qt5-default qtdeclarative5-dev qtdeclarative5-dev-tools
        # apt-get install arduino arduino-core gcc-avr binutils-avr avr-libc avrdude

2. Download and install OpenWRT SDK:

        OPENWRT_SDK_NAME=OpenWrt-SDK-15.05.1-ar71xx-generic_gcc-4.8-linaro_uClibc-0.9.33.2.Linux-x86_64
        cd /opt
        wget -nv https://downloads.openwrt.org/chaos_calmer/15.05.1/ar71xx/generic/$OPENWRT_SDK_NAME.tar.bz2
        tar xfj $OPENWRT_SDK_NAME.tar.bz2 $OPENWRT_SDK_NAME/staging_dir/target-mips_34kc_uClibc-0.9.33.2 --strip-components=2
	    tar xfj $OPENWRT_SDK_NAME.tar.bz2 $OPENWRT_SDK_NAME/staging_dir/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2 --strip-components=2

3. Build Wi-Bot Console and OpenWRT stuff

        $ cmake . && make
        
4. Build Arduino firmware

        $ cd src/arduino
        $ cmake . && make

On Windows you can build Wi-Bot Console using Cygwin or Qt Creator with preinstalled CMake.
For building OpenWRT stuff use Docker Toolbox and docker file in the root of repository.