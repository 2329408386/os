/*
*文件名:user.c
* 作者: 李程鹏 on Mar 9, 2018
* 描述: 关于用户操作的实现
*/

#include "types.h"
#include "user.h"
#include "string.h"
#include "print.h"
#include "keyboard.h"
#include "heap.h"

static user* head;
static user* current;
static uint32_t user_id=1;

// 打印出当前用户的信息
static void print_cur_user(user* user){
	print_color(rc_green,rc_black,"Current user\tuser id: #d\tuser name: #s\tpassword: #s\n",user->id,user->name,user->password);
}

// 打印出非当前用户的信息
static void print_other_user(user* user){
	print_color(rc_green,rc_black,"user id: #d\t user name: #s\tpassword: unknown\n",user->id,user->name,user->password);
}

// 统计当前user的数量
static uint32_t count_user(){
	int number=0;
	user* tranverse=head;
	while(tranverse!=NULL){
		number++;
		tranverse=tranverse->next;
	}
	return number;
}

// 判断当前用户中是否存在名字为name的用户
uint32_t is_contain(char* name){
	user* tranverse=head;
	while(tranverse!=NULL){
		if(strcmp(tranverse->name,name)==0)
			return 1;
		tranverse=tranverse->next;
	}
	return 0;
}

// 判断名字为name的用户密码是否匹配
uint32_t is_map(char* name, char* passwd){
	user* tranverse=head;
	while(tranverse!=NULL){
		if(strcmp(tranverse->name,name)==0){
			if(strcmp(tranverse->password,passwd)==0)
				return 1;
			else return 0;
		}
		tranverse=tranverse->next;
	}
	return 0;
}

// 初始化用户模块
void init_user(){
	head=(user*)kmalloc(sizeof(user));
	current=head;
	strcpy(current->name,"happyboy");
	current->id=user_id++;
	strcpy(current->password,"123456");
	current->next=NULL;
}

// 预览用户信息
void user_view(){
	user* tranverse=head;
	while(tranverse!=NULL){
		if(tranverse==current)
			print_cur_user(tranverse);
		else
			print_other_user(tranverse);
		tranverse=tranverse->next;
	}
	print_color(rc_green,rc_black,"There are total #d users now\n",count_user());
}

// 添加一个用户
void user_add(char* name){

	// 判断是否满足用户数加入条件
	if(count_user()>=10){
		print_color(rc_red,rc_black,"\nSorry, you should only at most 10 users!\n");
		return;
	}

	// 判断名字长度是否满足条件
	if(strlen(name)>=19){
		print_color(rc_red,rc_black,"\nThe length of user name should be less than 20!\n");
		return;	
	}

	// 判断是否存在同名用户
	if(is_contain(name)){
		print_color(rc_red,rc_black,"\nThe user has been in the database.\n");
		return;	
	}

	// 设置用户密码
	password:print("\nPlease enter password: ");
	char* passwd=scanf();
	if(strlen(passwd)>=19){
		print_color(rc_red,rc_black,"\nThe length of password should be less than 20!\n");
		return;
	}
	print("\nPlease enter password again: ");
	char* passwd2=scanf();
	if(strcmp(passwd,passwd2)!=0){
		print_color(rc_red,rc_black,"\nTwo times passwords should be equal!\n");
		goto password;
	}

	// 添加新的用户(这里当然需要为新用户分配内存空间啦!)
	user* newUser=(user*)kmalloc(sizeof(user));
	strcpy(newUser->name,name);
	strcpy(newUser->password,passwd);
	newUser->id=user_id++;

	newUser->next=current->next;
	current->next=newUser;
	print("\nAdd user successfully!\n");
}

// 切换用户
void user_change(char* name){
	user* tranverse=head;
	while(tranverse!=NULL){
		if(strcmp(tranverse->name,name)==0){
			current=tranverse;
			return;
		}
		tranverse=tranverse->next;
	}
	print_color(rc_red,rc_black,"\nThere no such user!\n");
}

// 删除一个用户
void user_del(char* name){
	if(strcmp(name,"happyboy")==0){
		print_color(rc_red,rc_black,"\nYou cannot delete super user!\n");
		return;
	}

	user* tranverse=head;
	while(tranverse!=NULL){
		if(strcmp(tranverse->name,name)==0){
			if(tranverse==current){
				print_color(rc_red,rc_black,"\nYou cannot delete current user!\n");
				return ;
			}


			user* tranverse2=head;
			while(tranverse2!=tranverse){
				if(tranverse2->next==tranverse){
					tranverse2->next=tranverse->next;
					return;
				}
				tranverse2=tranverse2->next;
			}			
		}
		tranverse=tranverse->next;
	}
	print_color(rc_red,rc_black,"\nThere no such user!\n");
}