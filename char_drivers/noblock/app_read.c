#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int main(void)
{
	char buf[20];
	int fd;
	int ret;

	fd = open("/dev/test",O_RDWR|O_NONBLOCK);
	fd = open("/dev/test",O_RDWR);
	if(fd < 0){
		perror("open");
		return -1;
	}

	ret = read(fd,buf,10);
	if(ret == -1){
		//perror("read");
		printf("errno = %d\r\n",errno);
	}else{
		printf("<app read>buf is [%s]\r\n",buf);
	}
	close(fd);

	return 0;
}
