#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H
#include <sys/time.h>
#include <pthread.h>

/* 输入事件类别 */
#define INPUT_TYPE_STDIN        0
#define INPUT_TYPE_TOUCHSCREEN  1
#define INPUT_TYPE_NET          2

#define EVENT_MAIN_INDEX_0       0x10
#define EVENT_MAIN_INDEX_1       0x11
#define EVENT_MAIN_INDEX_2       0x12

#define EVENT_OPENJTAG_BURN         0x20
#define EVENT_OPENJTAG_TEST         0x21

#define EVENT_CAMERA_TEST         0x41

#define EVENT_KILL         0x1001
#define EVENT_EXIT         0x1000

#define MAIN_PAGE_MASK      0x10
#define OPENJTAG_PAGE_MASK  0x20
#define CAMERA_PAGE_MASK    0x40


typedef struct InputEvent {
	struct timeval tTime;   /* 发生这个输入事件时的时间 */
	int iType;  /* 类别: stdin, touchsceen */
	int iX;     /* X/Y座标 */
	int iY;
	int iKey;   /* 按键值 */
	int iPressure; /* 压力值 */
	char aStr[1024];
}T_InputEvent, *PT_InputEvent;

#define INPUT_EVENT_STR_OFFSET ((int)(&((T_InputEvent *)0)->aStr))

typedef struct InputOpr {
	char *name;          /* 输入模块的名字 */
	pthread_t tTreadID;  /* 子线程ID */
	int (*DeviceInit)(void);  /* 设备初始化函数 */
	int (*DeviceExit)(void);  /* 设备退出函数 */
	int (*GetInputEvent)(PT_InputEvent ptInputEvent);  /* 获得输入数据 */
	struct InputOpr *ptNext;
}T_InputOpr, *PT_InputOpr;


/**********************************************************************
 * 函数名称： InputInit
 * 功能描述： 调用各个输入模块的初始化函数,就是注册各个输入模块
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int InputInit(void);

/**********************************************************************
 * 函数名称： RegisterInputOpr
 * 功能描述： 注册"输入模块"
 * 输入参数： ptInputOpr - 输入模块的结构体指针
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int RegisterInputOpr(PT_InputOpr ptInputOpr);

/**********************************************************************
 * 函数名称： ShowInputOpr
 * 功能描述： 显示本程序能支持的"输入模块"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
void ShowInputOpr(void);

/**********************************************************************
 * 函数名称： AllInputDevicesInit
 * 功能描述： 调用所有"输入模块"的设备相关的初始化函数
 *            并创建用于读取输入数据的子线程
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int AllInputDevicesInit(void);

/**********************************************************************
 * 函数名称： GetInputEvent
 * 功能描述： 获得输入数据,它会使得当前线程休眠,
 *            当各输入模块的子线程读到数据后会把它唤醒
 * 输入参数： 无
 * 输出参数： ptInputEvent - 内含得到的输入数据
 * 返 回 值： 0 - 成功
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int GetInputEvent(PT_InputEvent ptInputEvent);

/**********************************************************************
 * 函数名称： StdinInit
 * 功能描述： 注册"标准输入模块"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int StdinInit(void);

/**********************************************************************
 * 函数名称： TouchScreenInit
 * 功能描述： 注册"触摸屏输入模块"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int TouchScreenInit(void);

/**********************************************************************
 * 函数名称： NetInputInit
 * 功能描述： 注册"触摸屏输入模块"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2015/04/23	     V1.0	  韦东山	      创建
 ***********************************************************************/
int NetInputInit(void);

#endif /* _INPUT_MANAGER_H */

