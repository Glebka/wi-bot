#*************************************************************************
# WiBot Project
#*************************************************************************
# @file    CMakeLists.txt
# @date
# @author  Hlieb Romanov
# @brief   WiBot Service Tool application build configuration
#***********************************************************************/
cmake_minimum_required(VERSION 3.1.3)

project(wibot_service_tool)

file(GLOB headers
   *.h*
)

file(GLOB sources
   *.cpp
)

file(GLOB resources
   *.qrc
)

file(GLOB ui
   *.ui
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5SerialPort REQUIRED)

qt5_add_resources(QRC ${resources})

include_directories(${CMAKE_SOURCE_DIR}/..)

if (WIN32)
	add_executable(wibot_service_tool WIN32
		${headers}
		${sources}
        ${ui}
		${QRC}
	)
else()
	add_executable(wibot_service_tool 
		${headers}
		${sources}
        ${ui}
		${QRC}
	)
endif()

qt5_use_modules(wibot_service_tool Core Widgets SerialPort)

install(TARGETS wibot_service_tool
	RUNTIME DESTINATION usr/bin	
)
