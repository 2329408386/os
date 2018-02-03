/*
*文件名:string_test.c
*作者: 李程鹏
*描述: 对string模块函数的测试
*/

#include "tests.h"
#include "string.h"
#include "console.h"
#include "types.h"

#define N 10
#define M 5

// 对几个memset函数的测试
// static void memset_test()
// {
// 	console_write("When big data truncate to small data:\n");
// 	uint32_t dest[10];

// 	//这里我故意用相互错开的例子来测试内存复制的一些本质问题, 说实话,gcc编译器给的提示真好, 解决了很多问题
// 	memset_uint8(dest,0xff,40);
// 	int i=0;
// 	for(;i<10;i++){
// 		console_write_dec_sign(dest[i],rc_white,rc_black);
// 		console_write("\t");
// 	}
// 	console_write("\n\nWhen small data extend to big data:\n");

// 	uint8_t dest2[20];
// 	memset_uint32(dest2,0x41424344,5);
// 	for(i=0;i<20;i++){
// 		console_putc_color(dest2[i],rc_red,rc_black);
// 		if((i+1)%4==0)
// 			console_write("\t");
// 	}
// }

// 对几个memcpy函数的测试
// static void memcpy_test()
// {
// 	// unsigned char dest[N+1];
// 	// unsigned char src[N+1]="abcdef\101";
// 	// memcpy_uint8(dest,src,N);
// 	// console_write(dest);
// 	uint32_t dest[N];
// 	uint32_t src[N]={1,2,3,4,5};
// 	memcpy_uint32(dest,src,N);
// 	int i=0;
// 	for(;i<N;i++){
// 		console_write_dec(dest[i],rc_blue,rc_black);
// 		console_write("\t");
// 	}
// 	console_write("\n");
// }

// 对strlen函数的测试
// static void strlen_test()
// {
// 	char str[N]="abcdj";
// 	char str2[N]="";
// 	if(strlen(str)==5&&strlen(str2)==0)
// 		console_write_color("Test1: true\n",rc_green,rc_black);
// 	else
// 		console_write_color("Test1: false\n",rc_red,rc_black);

// 	// char str3[N]="abcdefghijklm";	从这里输出结果为10,可以看出, 系统把多余的部分直接truncate掉了.
// 	// console_write_dec_sign(strlen(str3),rc_white,rc_black);
// }

// 对strcmp函数的测试
// static void strcmp_test()
// {
// 	char strs1[M][N]={"","","abc","abc"};
// 	char strs2[M][N]={"","a","ab","abc"};
// 	int result[M]={0,-1,1,0};
// 	int i=0;
// 	for(;i<4;i++){

// 		// 啊啊啊, 看来我的实现一个好一点的打印函数接口, 这样调用太麻烦了吧!!!!
// 		// console_write(strs1[i]);
// 		// console_write("  ");
// 		// console_write(strs2[i]);
// 		// console_write("  ");
// 		// console_write_dec_sign(strcmp(strs1[i],strs2[i]),rc_blue,rc_black);
		
// 		if(strcmp(strs1[i],strs2[i])==result[i]){
// 			console_write_color("Test",rc_green,rc_black);
// 			console_write_dec_sign(i,rc_green,rc_black);
// 			console_write_color(": true\n",rc_green,rc_black);
// 		}
			
// 		else{
// 			console_write_color("Test",rc_red,rc_black);
// 			console_write_dec_sign(i,rc_red,rc_black);
// 			console_write_color(": false\n",rc_red,rc_black);
// 		}
// 	}
// }

// 对于strcpy函数的测试.
// static void strcpy_test()
// {
// 	char str1[M][N]={"","abc","abcde"};
// 	char str2[N]="fff";
// 	int i=0;
// 	for(;i<3;i++){
// 		strcpy(str1[i],str2);
// 		console_write_color(str1[i],rc_green,rc_black);
// 		console_write("\n");
// 	}
// }

// 对于strcat函数的测试
static void strcat_test()
{
	char str1[M][N]={"","abc","abcde"};
	char str2[N]="fff";
	int i=0;
	for(;i<3;i++){
		strcat(str1[i],str2);
		console_write_color(str1[i],rc_green,rc_black);
		console_write("\n");
	}
}

// 对string模块的测试接口
void string_test()
{
	// memcpy_test();
	// memset_test();
	// strlen_test();
	// strcmp_test();
	// strcpy_test();
	strcat_test();
}