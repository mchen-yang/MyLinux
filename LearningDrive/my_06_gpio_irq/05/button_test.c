
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>

static int fd;

static void  sig_func(int   signal){
	int val;

	/* 3. 读文件 */
		read(fd, &val, 4);
		printf("get button : 0x%x\n", val);

}
/*
 * ./button_test /dev/100ask_button0
 *
 */
int main(int argc, char **argv)
{
	
	
	struct pollfd fds[1];
	int timeout_ms = 5000;
	int ret;
	
	/* 1. 判断参数 */
	if (argc != 2) 
	{
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	/* 2. 打开文件 */
	fd = open(argv[1], O_RDWR);
	if (fd == -1)
	{
		printf("can not open file %s\n", argv[1]);
		return -1;
	}
	signal(SIGIO, sig_func);

	fcntl(fd, F_SETOWN, getpid());
	flag = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flag | FASYNC);
	
	

	while (1)
	{
		sleep(2);
	}
	
	close(fd);
	
	return 0;
}


