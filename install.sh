#!/bin/bash

# This script builds a lightning-boot image.

set -e

. $PROJECT_PATH/scripts/functions

# make sure all of the environment variables are good
check_vars

set_standard_opts $*

pushd $PROJECT_PATH/lightning-boot-2.0/

if [ "$CLEAN" == "1" ]; then
	make clean
fi

# grab the kernel config
LINUX_CONFIG=include/linux/autoconf.h
if [ -e $KERNELDIR/$LINUX_CONFIG ]; then
	cp $KERNELDIR/$LINUX_CONFIG ./include/
else
	echo "Error: $KERNELDIR/$LINUX_CONFIG not found, you must configure and build the kernel before running this script."
	exit 1
fi

# grab the machine types
MACH_TYPES=include/asm-arm/mach-types.h
if [ -e $KERNELDIR/$MACH_TYPES ]; then
	cp $KERNELDIR/$MACH_TYPES ./include/
else
	echo "Error: $KERNELDIR/$MACH_TYPES not found, you must configure and build the kernel for ARM before running this script."
	exit 1
fi

LINUX_DRIVERS=$KERNELDIR/drivers/lf1000
cp $LINUX_DRIVERS/dpc/dpc_hal.h ./include/
cp $LINUX_DRIVERS/mlc/mlc_hal.h ./include/
cp $LINUX_DRIVERS/pwm/pwm_hal.h ./include/

UBOOT_SUPPORT=$UBOOTLOADERS KERNELDIR=$KERNELDIR CROSS_COMPILE=$CROSS_COMPILE make 

# echo "" >> lightning-boot.bin
VERSION=`grep "CURRENT_MAJOR_VERSION=" ../packages/version/install.sh | cut -d = -f 2`
# BUILD=`svn info $PROJECT_PATH | grep Revision | awk '{print $2}'`
# BUILD=1337
# echo "VERSION:$VERSION.$BUILD" >> lightning-boot.bin
# echo "" >> lightning-boot.bin

cp lightning-boot.bin $TFTP_PATH/
popd
