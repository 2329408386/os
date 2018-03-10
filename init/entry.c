/*
 * 入口函数初始化操作: 引用hurley的初始页
 */

#include "init.h"
#include "login.h"
#include "tests.h"
#include "command.h"
#include "print.h"
#include "string.h"

void kern_init()
{
	init();
	again:welcome();
	login();

	while(1){
		print("\nhappyOS: ");
		char* command=scanf();
		if(strcmp(command,"exit")==0)
			goto again;
		choose(command);
	}

	asm volatile("sti");    //打开中断
	

    // 宕机等待.
    while(1){
        asm volatile("hlt");
    }
}

