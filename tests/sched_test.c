// sched_test.c

#include "tests.h"
#include "sched.h"
#include "print.h"
#include "task.h"



int flag=0;

int thread(void* args){
	while(1){
		if(flag==0){
			print_color(rc_red,rc_black,"A");
			flag=1;
		}
	}
	return 0;
}

int thread2(void* args){
	while(1){
		if(flag==1){
			print_color(rc_brown,rc_black,"B");
			flag=2;
		}
	}
	return 0;
}

int thread3(void* args){
	while(1){
		if(flag==2){
			print_color(rc_green,rc_black,"C");
			flag=0;
		}
	}
	return 0;
}
void sched_test()
{
	kernel_thread(thread,NULL);
	kernel_thread(thread2,NULL);
	kernel_thread(thread3,NULL);
}