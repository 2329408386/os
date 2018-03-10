/*
* 文件名:  game.c
* 作者: 李程鹏 on Mar 10, 2018
* 描述:  包含一些用户可以玩的小游戏
*/

#include "common.h"
#include "game.h"
#include "types.h"
#include "timer.h"
#include "keyboard.h"
#include "print.h"
#include "string.h"
#include "task.h"
#include "sched.h"

#define N 1000000

static int count=0;
static int f1(void* nothing){
	while(1){
		int b=0;
		while(b++<N)
			;
		print_color(rc_red,rc_black,"#d",count);
		count++;
	}
	return 0;
}
static int f2(void* nothing){
	while(1){
		int b=0;
		while(b++<N)
			;
		print_color(rc_blue,rc_black,"#d",count);
		count++;
	}
	return 0;
}
static int f3(void* nothing){
	while(1){
		int b=0;
		while(b++<N)
			;
		print_color(rc_green,rc_black,"#d",count);
		count++;
	}
	return 0;
}
static int f4(void* nothing){
	while(1){
		int b=0;
		while(b++<N)
			;
		print_color(rc_brown,rc_black,"#d",count);
		count++;
	}
	return 0;
}
static int f5(void* nothing){
	while(1){
		int b=0;
		while(b++<N)
			;
		print_color(rc_white,rc_black,"#d",count);
		count++;
	}
	return 0;
}

typedef int (*f)(void*);
static f fs[6]={f1,f2,f3,f4,f5}; 

extern pcb* current;

static uint32_t to_number(char* str){
    uint32_t result=0,i=0;
    for (;str[i]!=0;i++)
        result=result*10+(str[i]-'0');
    return result;
}

// 猜数游戏, number为该数最大值
void guess(uint32_t number){
	uint32_t result=get_vitual_timer()%number+1;
	uint32_t youGuess=0;
	print("\nThe result is between 1 and #d(included), it's your turn to guess:\n",number);
	while(youGuess!=result){
		youGuess=to_number(scanf());
		if(youGuess>result)
			print("\nToo big\n");
		else if(youGuess<result)
			print("\nToo small\n");
		else{
			print_color(rc_green,rc_black,"\nCongratuations!\n");
			return;
		}
	}
}

// 一个内核线程切换的小游戏, number为线程数量, 而且不能大于5.
void counter(uint32_t number){
	if(number>5){
		print_color(rc_red,rc_black,"\nSorry, I only create 5 threads\n");
		return;
	}
	int i=0;
	print("\n");
	for(;i<number;i++){
		kernel_thread(fs[i],NULL);
	}
	
	while(1){
		if(count>500){
			current=NULL;
			print("\n");
			print("Have you find something interesting, yeah, they are completing for a variable!\n");
			break;
		}
	}
}

// 通过输入数字来决定每次跳跃的个数, 输入0时表示结束.
void jump(uint32_t steps){
	print("\n0");
	int i=0;
	for(;i<steps;i++)
		print("*");
	print("0\n");
}