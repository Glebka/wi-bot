cmake_minimum_required(VERSION 3.1.3)

project(wi-bot)

if(NOT WIN32)
	add_subdirectory(src/mjpgstreamer)
	add_subdirectory(src/cmd-server)
endif()

add_subdirectory(src/wibot-console)
