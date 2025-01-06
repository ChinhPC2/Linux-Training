RPI_KERNEL := /home/chinhpc2/linux 
TOOLCHAIN := aarch64-linux-gnu-
 
EXTRA_CFLAGS=-Wall
obj-m := device_driver.o
 
all:
	make ARCH=arm64 CROSS_COMPILE=$(TOOLCHAIN) -C $(RPI_KERNEL) M=$(shell pwd) modules
 
clean:
	make -C $(RPI_KERNEL) M=$(shell pwd) clean