obj-m := module_a.o
#obj-m := module_b.o

#KBUILD_EXTRA_SYMBOLS := /home/chinhpc2/Desktop/Linux_Training_ChinhPC2/Linux-Training/Kernel_Module_Exercise/Module.symvers

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

