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
#include <input_manager.h>
#include <page_manager.h>

#include <ui.h>


int main(int argc, char **argv)
{
	int error;

	if (argc != 2)
	{
		printf("Usage: %s <font_File>\n", argv[0]);
		return -1;
	}
	/* init display */
	DisplayInit();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	/* init input */
	InputInit();

	InitInputDevice();

	/* init font */
	
	FontInit();

	error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}
	/* init page */
	PagesRegister();

		/* run main_page */
	Page("main")->Run(NULL);



	return 0;
}

