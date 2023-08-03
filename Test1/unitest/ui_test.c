#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>


#include <font_manager.h>
#include <disp_manager.h>
#include <ui.h>


int main(int argc, char **argv)
{
	PDispBuff ptBuffer;
	int error;
	Button tButton;
	Region tRegion;



	if (argc != 5)
	{
		printf("Usage: %s <font_size>\n", argv[0]);
		return -1;
	}

	DisplaySystemRegister();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	ptBuffer = GetDispBuff();
	
	FontSystemRegister();
	
	error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

	tRegion.LeftUpX = 200;
	tRegion.LeftUpY = 200;
	tRegion.Width   = 300;
	tRegion.Heigh   = 100;

	InitButton(&tButton, "testButton", &tRegion, NULL, NULL);

	tButton.OnDraw(&tButton, ptBuffer);
	while (1)
	{
		tButton.OnPressed(&tButton, ptBuffer, NULL);
		sleep(2);
	}
	return 0;
}

