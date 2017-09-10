#*************************************************************************
# WiBot Project
#*************************************************************************
# @file    CMakeLists.txt
# @date
# @author  Hlieb Romanov
# @brief   WiBot Console application build configuration
#***********************************************************************/
cmake_minimum_required(VERSION 3.1.3)

project(wibot_console)

file(GLOB_RECURSE headers
   *.hpp
)

file(GLOB_RECURSE sources
   *.cpp
)

file(GLOB_RECURSE resources
   *.qrc
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(JPEG REQUIRED)

include_directories(${JPEG_INCLUDE_DIR} SYSTEM)

qt5_add_resources(QRC ${resources})

if (WIN32)
	add_executable(wibot_console WIN32
		${headers}
		${sources}
		${QRC}
	)
else()
	add_executable(wibot_console 
		${headers}
		${sources}
		${QRC}
	)
endif()

qt5_use_modules(wibot_console Core Widgets Quick)

target_link_libraries(wibot_console ${JPEG_LIBRARIES})

install(TARGETS wibot_console
	RUNTIME DESTINATION usr/bin	
)

