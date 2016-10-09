cmake_minimum_required(VERSION 3.1.3)

project(wi-bot)

if(NOT WIN32)
	set(ENV{STAGING_DIR} /opt)
	set(ENV{TOOLCHAIN_DIR} $ENV{STAGING_DIR}/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2)
	add_subdirectory(src/mjpgstreamer)
	add_subdirectory(src/cmd-server)
endif()

add_subdirectory(src/wibot-console)
