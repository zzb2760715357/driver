#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	char buf[20];
	int fd;	
	int ret;
	
	fd = open("/dev/test",O_RDWR);
	if(fd<0){
		perror("open");
		return -1;	
	}

	write(fd,"zbzhuang",10);

	close(fd);


	return 0;
}
