#include <stdio.h>
#include <stdlib.h>

static char buff[255];
static char * string;

int main(void){
	string = buff;
	printf("Please input a string:");
	gets(string);
	printf("you string is :%s\r\n", string);

	return 0;
}

