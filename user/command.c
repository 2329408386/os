/**
*文件名: command.c
* 作者: 李程鹏 on Mar 3, 2018
* 描述: 有关命令行的一些操作
*/

#include "print.h"
#include "command.h"
#include "user.h"
#include "string.h"
#include "check.h"
#include "game.h"

static uint32_t to_number(char* str){
    uint32_t result=0,i=0;
    for (;str[i]!=0;i++)
        result=result*10+(str[i]-'0');
    return result;
}

// 打印当前支持的一些指令
void help(){

	// User manipulation
	print("\n-----------user manipulation-----------\n");
	print("adduser (Add a user, eg. adduser username)\n");
	print("deluser (Delete a user, eg. deluser username)\n");
	print("viewusers (View a user, eg. viewusers)\n");
	print("changeuser (Change current user, eg. changeuser username)\n");
	print("-----------user manipulation end-----------\n");

	// Check manipupation
	print("\n-----------check manipulation-----------\n");
	print("check_console (Check console manipulation)\n");	
	print("check_heap (Check heap manipulation)\n");
	print("check_memory (Check memory manipulation)\n");
	print("-----------check manipulation end-----------\n");

	// Games
	print("\n-----------games-----------\n");
	print("guess (guess number game, eg. guess 10: guess a number between 1 and 10)\n");	
	print("counter (It's a thread simulation game, eg. count 4: there are 4 threads running.)\n");
	print("jump (Eee....... It's a game.......)\n");
	print("-----------games end-----------\n");

	print("help (Print all commands which have been provided now)\n");
	print("exit (exit system)\n");
}

// 根据输入的命令选择要进行的操作
void choose(char* command){
	// 提取出指令的第一部分和第二部分
	char first[20];
	char second[20];

	int pos=0,pos1=0,pos2=0;
	while(command[pos]!=0&&command[pos]!=' '){
		first[pos1++]=command[pos++];
	}
	first[pos1]=0;
	pos++;
	while(command[pos]!=0&&command[pos]!=' '){
		second[pos2++]=command[pos++];
	}
	second[pos2]=0;

	if(strcmp(first,"help")==0)
		help();
	else if(strcmp(first,"adduser")==0){
		user_add(second);		
	}
	else if(strcmp(first,"viewusers")==0){
		print("\n");
		user_view();
	}
	else if(strcmp(first,"changeuser")==0){
		print("\n");
		user_change(second);
	}
	else if(strcmp(first,"deluser")==0){
		print("\n");
		user_del(second);
	}
	else if(strcmp(first,"check_console")==0){
		check_console();
	}
	else if(strcmp(first,"check_heap")==0){
		check_heap();
	}
	else if(strcmp(first,"check_memory")==0){
		check_memory();
	}
	else if(strcmp(first,"guess")==0){
		guess(to_number(second));
	}
	else if(strcmp(first,"counter")==0){
		counter(to_number(second));
	}
	else if(strcmp(first,"jump")==0){
		jump(to_number(second));
	}
	else{
		print_color(rc_red,rc_black,"\nNo such command!\n");
	}

}