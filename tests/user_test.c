/*
* 文件名:user_test.c
* 作者: 李程鹏 on Mar 9 2018
* 描述: 对用户模块的测试
*/

#include "tests.h"
#include "user.h"

// 对用户操作的测试
void user_test(){
	user_add("name1");
	user_add("name2");
	user_view();
}