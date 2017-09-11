#*************************************************************************
# WiBot Project
#*************************************************************************
# @file    CMakeLists.txt
# @date
# @author  Hlieb Romanov
# @brief   Build configuration for Command Server on HU
#***********************************************************************/
cmake_minimum_required(VERSION 3.1.3)

project(cmd-server)

#OpenWRT 15.05
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 15.05)
SET(CMAKE_SYSTEM_PROCESSOR ar71xx)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /opt/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-gcc)
SET(CMAKE_CXX_COMPILER /opt/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /opt/target-mips_34kc_uClibc-0.9.33.2)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

add_executable(cmd-server server.c)

install(TARGETS cmd-server
    RUNTIME DESTINATION usr/bin
)
