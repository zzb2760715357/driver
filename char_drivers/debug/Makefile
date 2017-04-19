obj-m += test.o
APP_NAME = app
APP_CC = arm-none-linux-gnueabi-gcc
KDIR:= /home/farsight/linux_system/kernel/linux-3.0.8


all:
	make -C $(KDIR) M=`pwd` modules
	$(APP_CC) $(APP_NAME).c -o $(APP_NAME)
clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
install:
	cp -raf  *.ko  /opt/rootfs/zbzhuang_drivers/
	cp -raf  $(APP_NAME)  /opt/rootfs/zbzhuang_drivers/







