KDIR = /lib/modules/`uname -r`/build
PWD = $(shell pwd)
DEST = /lib/modules/`uname -r`/misc

obj-m := $ fw.o 

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


