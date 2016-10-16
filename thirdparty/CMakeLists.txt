cmake_minimum_required(VERSION 3.0)

include(ExternalProject)

ExternalProject_Add(mjpg-streamer
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/mjpg-streamer
	SVN_REPOSITORY svn://svn.code.sf.net/p/mjpg-streamer/code/
	UPDATE_COMMAND ""
	PATCH_COMMAND ${PROJECT_SOURCE_DIR}/thirdparty/patch.sh
	CONFIGURE_COMMAND "" 
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	TEST_COMMAND ""
)