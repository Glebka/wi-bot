cmake_minimum_required(VERSION 3.1.3)

project(output_rawtcp)

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

include_directories(SYSTEM
	${PROJECT_SOURCE_DIR}/thirdparty/mjpg-streamer/mjpg-streamer-experimental
)

set(sources
	output_rawtcp.c
)

add_library(output_rawtcp SHARED
	${sources}
)

add_dependencies(output_rawtcp mjpg-streamer)

target_link_libraries(output_rawtcp
	pthread
	dl
)