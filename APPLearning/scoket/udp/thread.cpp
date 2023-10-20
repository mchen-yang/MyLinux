
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <iostream>


using namespace std;
int value = 0;
static inline void mythread(void){

	value++;
}


int main(int argc, char **argv)
{
	thread thread1(mythread);
	thread thread2(mythread);
	thread thread3(mythread);
	thread thread4(mythread);
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	cout<<value;

	return 0;
}


