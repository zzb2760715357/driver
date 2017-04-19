#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

int main(void)
{
	int fd;
	fd_set r_fset;

	fd = open("/dev/test",O_RDWR);
	if(fd < 0){
		perror("open");
		return -1;
	}

	FD_ZERO(&r_fset);

	while(1){
		FD_SET(fd,&r_fset);
		select(fd+1,&r_fset,NULL,NULL,NULL);

		if(FD_ISSET(fd,&r_fset)){
			printf("<app>monitor:[device readable]\n");
			sleep(4);
		}
	}

	return 0;
}
