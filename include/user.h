/**
*文件名: user.h
* 作者: 李程鹏 on Mar 3, 2019
* 描述: 关于用户的一些操作
*/

#ifndef USER_H
#define USER_H

#include "types.h"

// 用户模块
typedef struct user{
	char name[20];
	uint32_t id;
	char password[20];
	struct user* next;
}user;

// 初始化用户模块
void init_user();

// 添加一个用户
void user_add(char* name);

// 删除一个用户
void user_del(char* name);

// 预览用户信息
void user_view();

// 切换用户
void user_change(char* name);

// 判断当前用户中是否存在名字为name的用户
uint32_t is_contain(char* name);

// 判断名字为name的用户密码是否匹配
uint32_t is_map(char* name, char* passwd);

#endif

