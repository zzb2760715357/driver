
#if 1
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#include "test_cmd.h"

const char *dev_path = "/dev/test";

struct ioctl_data my_data = {
	.size = 10,
	.buf = "1234567890",

};


int main(int argc,char **argv)
{
	int fd,ret;
	char buf[128];

	fd = open("/dev/test",O_RDWR);
	if(fd < 0){
		printf("Error open %s\r\n",dev_path);
	}else{
		printf("Success open %s\r\n",dev_path);				
	}




	memcpy(buf,"hello kernel",strlen("hello kern"));
	ret = write(fd,buf,10);
	//ret = write(fd,(const void *)(0),strlen("hello kernel"));
	if(ret < 0){
		printf("Error wirte\r\n");
		perror("write");
		return -1;
	}

	//ret = lseek(fd,0,SEEK_SET);
	//ioctl(fd,TEST_CLEAR);
	//ioctl(fd,TEST_OFFSET,-12);
	ioctl(fd,TEST_KBUF,&my_data);

	ret = read(fd,buf,10);
	if(ret < 0){
		printf("Error read\r\n");
		return -1;
	}
	printf("app:%s\r\n",buf);
	//fprintf(stderr,"app:%s\r\n",buf);
	

	close(fd);
	
	return 0;
}

#else
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	close(fd);
	return 0;
}


#endif



