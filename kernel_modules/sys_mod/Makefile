KDIR = /lib/modules/`uname -r`/build
PWD = $(shell pwd)
DEST = /lib/modules/`uname -r`/misc

obj-m := $ hello_sys.o 

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


