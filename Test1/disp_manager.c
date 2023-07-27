#include "disp_manager.h"


static DispOper g_tFramebufferOper = {
	.name			= "fb",
	.GetBuffer		= FbGetBuffer,
	.FlushRegion	= FbFlushRegion,
};