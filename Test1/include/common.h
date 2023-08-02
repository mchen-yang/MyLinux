

#ifndef _COMMON_H
#define _COMMON_H

#ifndef NULL
#define NULL (void *)0
#endif

typedef struct Region {
	int LeftUpX;
	int LeftUpY;
	int Width;
	int Heigh;
}Region, *PRegion;

#endif

