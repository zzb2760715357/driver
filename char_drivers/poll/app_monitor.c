#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

int main(void)
{
	int fd;
	fd_set r_fset,w_fset;

	fd = open("/dev/test",O_RDWR);
	if(fd < 0){
		perror("open");
		return -1;
	}

	FD_ZERO(&r_fset);
	FD_ZERO(&w_fset);

	while(1){
		FD_SET(fd,&r_fset);
		FD_SET(fd,&w_fset);
		select(fd+1,&r_fset,&w_fset,NULL,NULL);

		if(FD_ISSET(fd,&r_fset)){
			printf("<app>monitor:[device readable]\n");
			
		}

		if(FD_ISSET(fd,&w_fset))
			printf("<app>monitor:[device writeable]\r\n");
		sleep(4);
	}

	return 0;
}
