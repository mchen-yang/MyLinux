//#include <sys/mman.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <linux/fb.h>
//#include <fcntl.h>
//#include <stdio.h>
//#include <string.h>
//#include <sys/ioctl.h>
/* My include */
#include <input_manager.h>
#include <tslib.h>
#include <stdio.h>

struct tsdev *g_ts;


static int TouchDeviceInit(void) {

	g_ts= ts_setup(NULL, 0);
	if (!g_ts)
	{
		printf("ts_setup err\n");
		return -1;
	}
	return 0;
}

static int TouchDeviceExit(void) {
	ts_close(g_ts);
	return 0;
}

static int TouchGetInput(PInputData ptInputData) {
	struct ts_sample samp;
	int ret;
	ret = ts_read(g_ts, &samp, 1);
		if (ret != 1)
			return -1;

	ptInputData->InputType = INPUT_TYPE_TOUCH;
	ptInputData->X 				= samp.x;
	ptInputData->Y 				= samp.y;
	ptInputData->Pressure = samp.pressure;
	ptInputData->tTime 		= samp.tv;

	return 0;
}


static InputOper g_tTouchScreenOper = {
	.name			= "touchscreen",
	.InputInit		= TouchDeviceInit,
	.InputExit		= TouchDeviceExit,
	.GetInput		= TouchGetInput,
};

void TouchScreenRegister(void) {
	RegisterInput(&g_tTouchScreenOper);
}

#if 0
int main(int argc, char **argv){
	InputData tInputData;

	int ret;
	g_tTouchScreenOper.InputInit();

	while(1){
		ret = g_tTouchScreenOper.GetInput(&tInputData);
		if(ret == -1){
			printf("GetInput err!\n");
			return -1;
		}
		else{
			printf("Type     = %d\n", tInputData.InputType);
			printf("X        = %d\n", tInputData.X);
			printf("Y        = %d\n", tInputData.Y);
			printf("Pressure = %d\n", tInputData.Pressure);
		}
	}
	return 0;
}
#endif
