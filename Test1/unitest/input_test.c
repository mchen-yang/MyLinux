#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <input_manager.h>


int main(int argc, char **argv)
{
	int ret;
	InputData tData;

	InputInit();

	InitInputDevice();
	printf("start run\n");
	while(1){
	 ret = GetInputData(&tData);
	 printf("ret\n");
	 /* handle data */
		if(!ret){
		printf("GetData\n");
			if(tData.InputType == INPUT_TYPE_TOUCH)	{
				printf("Type     : TOUCH\n");
				printf("X        : %d\n", tData.X);
				printf("Y        : %d\n", tData.Y);
				printf("Pressure : %d\n", tData.Pressure);
			}
			else if(tData.InputType == INPUT_TYPE_NET)	{
				printf("Type     : NET\n");
				printf("str      : %s\n", tData.InputStr);
			}
	 }
		else{
			printf("GetInputEvent err!\n");
			return -1;
		}
	}

	return 0;
}

