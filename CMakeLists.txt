cmake_minimum_required(VERSION 3.1.3)

project(wi-bot)

if(NOT WIN32)
	set(ENV{STAGING_DIR} /opt)
	set(ENV{TOOLCHAIN_DIR} $ENV{STAGING_DIR}/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2)
	add_subdirectory(src/mjpgstreamer-plugins/output_rawtcp)
	add_subdirectory(src/cmd-server)
	add_subdirectory(thirdparty)
endif()

if (CYGWIN)
	set (CMAKE_PREFIX_PATH "/cygdrive/c/Qt/5.4/mingw491_32/")
elseif(WIN32)
	set (CMAKE_PREFIX_PATH "c:/Qt/5.4/mingw491_32/")
endif()

SET(BUILD_STATIC ON CACHE BOOL "Build static libjpeg")
SET(BUILD_EXECUTABLES OFF CACHE BOOL "Do not build libjpeg executables")
SET(BUILD_TESTS OFF CACHE BOOL "Do not build libjpeg tests")
add_subdirectory(thirdparty/libjpeg)

add_subdirectory(src/wibot-console)
