obj-m +=  irq.o irq2.o
#APP_WRITE = app_write
#APP_READ  = app_read
#APP_MONITOR  = app_monitor
#APP_TEST  = app
APP_CC = arm-none-linux-gnueabi-gcc
DRIVER_DIR = irq
KDIR:= /home/farsight/linux_system/kernel/linux-3.0.8

all:
	make -C $(KDIR) M=`pwd` modules
	#$(APP_CC) $(APP_WRITE).c -o $(APP_WRITE)
	#$(APP_CC) $(APP_READ).c -o $(APP_READ)
	#$(APP_CC) $(APP_MONITOR).c -o $(APP_MONITOR)
	#$(APP_CC) $(APP_TEST).c -o $(APP_TEST)
clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
	#rm $(APP_WRITE)
	#rm $(APP_READ)
	#rm $(APP_MONITOR)
	#rm $(APP_TEST)
	
install:
	cp -raf  *.ko  /opt/rootfs/zbzhuang_drivers/$(DRIVER_DIR)/
	#cp -raf  $(APP_WRITE)  /opt/rootfs/zbzhuang_drivers/$(DRIVER_DIR)/
	#cp -raf  $(APP_READ)  /opt/rootfs/zbzhuang_drivers/$(DRIVER_DIR)/
	#cp -raf  $(APP_MONITOR)  /opt/rootfs/zbzhuang_drivers/$(DRIVER_DIR)/
	#cp -raf  $(APP_TEST)  /opt/rootfs/zbzhuang_drivers/$(DRIVER_DIR)/







