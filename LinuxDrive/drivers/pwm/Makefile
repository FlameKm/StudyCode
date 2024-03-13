export ARCH=arm64
export TOOLCHAIN_PATH=/home/hyc/Project/orangepi-build/toolchains/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu
export CROSS_COMPILE=${TOOLCHAIN_PATH}/bin/aarch64-none-linux-gnu-
export TOOLCHAIN_LD_LIBRARY_PATH=${TOOLCHAIN_PATH}/lib/x86_64-linux-gnu/

CC 		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)g++
AR 		= $(CROSS_COMPILE)ar
STRIP 	= $(CROSS_COMPILE)strip
LD 		= $(CROSS_COMPILE)ld

KERN_DIR = /home/hyc/Project/kernel

# CFLAGS = -Wall -I$(INCLUDE_DIR)

all: 
	make -C $(KERN_DIR) M=`pwd` modules 
	${CROSS_COMPILE}gcc -o test pwm_test.c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean

obj-m += pwm.o