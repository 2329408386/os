/**
* 文件名: login.c
* 作者: 李程鹏 on Mar 9,2018
* 描述: 用户登录的一些实现
*/

#include "login.h"
#include "print.h"
#include "types.h"
#include "string.h"
#include "keyboard.h"
#include "user.h"

// 输出欢迎信息
void welcome(){
	print_color(rc_green,rc_black,"#31Welcome to happyOS\n");
	print_color(rc_green,rc_black,"#26Last updated on Mar 10, 2018\n");
	print_color(rc_green,rc_black,"#26Author: LiChengpeng from CQU\n");
	print_color(rc_green,rc_black,"#10There is only one user now(happyboy), ");
	print_color(rc_red,rc_black,"password is '123456'.");
	print_color(rc_green,rc_black," To see the functions I've written to play, please type ");
	print_color(rc_red,rc_black,"'help'!\n");
	print_color(rc_green,rc_black,"#25Anyway, thank you to use my OS!\n\n\n");
}

// 用户登录
void login(){
	username:print("Please enter username: ");
	char* name=scanf();
	if(is_contain(name)==0){
		print_color(rc_red,rc_black,"\nThere's no such user!\n");
		goto username;
	}
	password:print("\nPlease enter password: ");
	char* passwd=scanf();
	if(is_map(name,passwd)==0){
		print_color(rc_red,rc_black,"\nPassword is wrong!\n");
		goto password;
	}

	// 改变当前用户
	user_change(name);	

	print_color(rc_green,rc_black,"\n#36Welcome!\n");
}