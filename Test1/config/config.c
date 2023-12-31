
#include <config.h>
#include <stdio.h>
#include <string.h>

static ItemCfg g_atItemCfgs[ITEMCFG_MAX_NUM];
static int g_TtemCfgCount = 0;

int ParseConfigFile(void)
{
	FILE *fp;
	char buf[100];
	char *p = buf;
	/* 1. open config file */
	fp = fopen(CFG_FILE, "r");
	if (!fp)
	{
		printf("can not open cfg file %s\n", CFG_FILE);
		return -1;
	}

	while(fgets(buf, 100, fp))
	{
		/* 2.1 read each line */
		buf[99] = '\0'; 	
		/* 2.2 吃掉开头的空格或TAB */
		p = buf;
		while (*p == ' ' || *p =='\t'){			
			p++;
		}
		/* 2.3 忽略注释 */
		if (*p == '#')
			continue;	
		/* 2.4 处理 */
		g_atItemCfgs[g_TtemCfgCount].command[0] = '\0';
		g_atItemCfgs[g_TtemCfgCount].index = g_TtemCfgCount;
		sscanf(p, "%s %d %s", g_atItemCfgs[g_TtemCfgCount].name, &g_atItemCfgs[g_TtemCfgCount].bCanBeTouched, \
		                      g_atItemCfgs[g_TtemCfgCount].command);
		g_TtemCfgCount++;					
	}
	return 0;
}

int GetItemCfgCount(void)
{
	return g_TtemCfgCount;
}
PItemCfg GetItemCfgByIndex(int index)
{
	if(index < g_TtemCfgCount){
		return &g_atItemCfgs[index];
	}
	return NULL;
}
PItemCfg GetItemCfgByName(char *name)
{
	int i;
	for(i = 0; i < g_TtemCfgCount; i++){
		if(strcmp(name, g_atItemCfgs[i].name) == 0){
			return &g_atItemCfgs[i];
		}
	}

	return NULL;

}



