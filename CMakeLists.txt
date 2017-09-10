#*************************************************************************
# WiBot Project
#*************************************************************************
# @file    CMakeLists.txt
# @date
# @author  Hlieb Romanov
# @brief   Main CMake build configuration
#***********************************************************************/
cmake_minimum_required(VERSION 3.1.3)

project(wi-bot)

include(ExternalProject)

if(NOT WIN32)
	set(ENV{STAGING_DIR} /opt)
	set(ENV{TOOLCHAIN_DIR} $ENV{STAGING_DIR}/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2)

	ExternalProject_Add(
		mjpg-streamer-download
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/mjpg-streamer
		SVN_REPOSITORY svn://svn.code.sf.net/p/mjpg-streamer/code/
		UPDATE_COMMAND ""
		PATCH_COMMAND ${CMAKE_SOURCE_DIR}/thirdparty/patch.sh
		CONFIGURE_COMMAND "" 
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		TEST_COMMAND ""
	)

	ExternalProject_Add(
		output-rawtcp
		DEPENDS mjpg-streamer-download
		DOWNLOAD_COMMAND ""
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/mjpgstreamer-plugins/output_rawtcp
		CMAKE_ARGS -DTHIRDPARTY_DIR=${CMAKE_SOURCE_DIR}/thirdparty -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/dist/HU
		INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/HU
	)
	
	ExternalProject_Add(
		cmd-server
		DOWNLOAD_COMMAND ""
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/cmd-server
		CMAKE_ARGS -DTHIRDPARTY_DIR=${CMAKE_SOURCE_DIR}/thirdparty -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/dist/HU
		INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/HU
	)

endif()

ExternalProject_Add(
	arduino-firmware
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/arduino
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/dist/ARDU	
	INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/ARDU
)

ExternalProject_Add(
	libjpeg
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/libjpeg
    CMAKE_ARGS -DBUILD_STATIC=ON -DBUILD_EXECUTABLES=OFF -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/dist/PC
	BINARY_DIR ${CMAKE_BINARY_DIR}/libjpeg
	INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/PC
)

ExternalProject_Add(
	wibot-console
	DEPENDS libjpeg
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/wibot-console
    CMAKE_ARGS -DCMAKE_PREFIX_PATH=${CMAKE_SOURCE_DIR}/dist/PC -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/dist/PC
	INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/PC
)

ExternalProject_Add(
    wibot-service-tool
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/wibot-service-tool
	CMAKE_ARGS -DCMAKE_PREFIX_PATH=${CMAKE_SOURCE_DIR}/dist/PC -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/dist/PC
	INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/PC
)
