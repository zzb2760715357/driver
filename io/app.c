#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/ioctl.h>

#include "led_ioctl.h"



int main(int argc,char *argv[])
{
	int fd;
	fd = open("/dev/led_driver",O_RDWR);
	if(fd < 0){
		perror("open");
		return -1;
	}

	ioctl(fd,LED_CONF);
	if(!strncasecmp("on",argv[1],3))
		ioctl(fd,LED_ON);

	if(!strncasecmp("off",argv[1],3))
		ioctl(fd,LED_OFF);
	while(1);

	return 0;
}

