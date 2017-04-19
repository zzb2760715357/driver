
#if 1
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


const char *dev_path = "/dev/test";

int main(int argc,char **argv)
{
	int fd;

	fd = open("/dev/test",O_RDWR);
	if(fd < 0){
		printf("Error open %s\r\n",dev_path);
	}else{
		printf("Success open %s\r\n",dev_path);				
	}

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



