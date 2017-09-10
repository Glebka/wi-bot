#!/bin/bash

#
# This script applies patches from OpenWRT community onto mjpg-streamer sources
#

PATCHES_DIR=../openwrt-packages/multimedia/mjpg-streamer/patches
SOURCES_DIR=mjpg-streamer-experimental
PATCHES_PATTERN=$PATCHES_DIR/*

for patch in $PATCHES_PATTERN
do
	echo "Applying patch: $patch"
	git apply --ignore-whitespace --whitespace=nowarn --directory=$SOURCES_DIR $patch
	rc=$?
	if [ $rc != 0 ]; then
		echo "Applying patch $patch with patch(1) util"
		patch -d $SOURCES_DIR -l -s -t -N < $patch
		if [ $? == 0 ]; then
			echo "Patch - OK"
		else
			echo "Patch - FAILED"
		fi
	else
		echo "Patch - OK"
	fi
done