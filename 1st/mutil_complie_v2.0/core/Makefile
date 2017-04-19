obj-m += haha.o

KDIR:= /home/farsight/linux_system/kernel/linux-3.0.8

all:
	make -C $(KDIR) M=`pwd` modules
clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order

install:
	cp -raf  *.ko  /opt/rootfs/zbzhuang_drivers/

