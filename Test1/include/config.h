#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <common.h>
#include <font_manager.h>
#include <input_manager.h>
#include <disp_manager.h>

#define ITEMCFG_MAX_NUM 30
#define CFG_FILE  "./etc/test_gui/gui.conf"
typedef struct ItemCfg{
	int index;
	char name[100];
	int bCanBeTouched;
	char command[100];
}ItemCfg, *PItemCfg;

int ParseConfigFile(void);
int GetItemCfgCount(void);
PItemCfg GetItemCfgByIndex(int index);
PItemCfg GetItemCfgByName(char *name);

#endif // !_CONFIG_H_