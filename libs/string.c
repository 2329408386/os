/*
*文件名:string.c
*作者: 李程鹏
*描述: 包含对字符串操作函数的基本实现.
*/

#include "string.h"
#include "types.h"
#include "console.h"

//下面3个函数分别以32位,16位和8位的形式复制给定长度,给定值的数据到指定位置.
void memset_uint32(uint32_t* dest,uint32_t value, int len ){
	int i=0;
	for(;i<len;i++)
		dest[i]=value;
}
void memset_uint16(uint16_t* dest,uint16_t value, int len ){
	int i=0;
	for(;i<len;i++)
		dest[i]=value;
}
void memset_uint8(uint8_t* dest,uint8_t value, int len ){
	int i=0;
	for(;i<len;i++)
		dest[i]=value;
}


//下面3个函数分别以32位,16位和8位的形式复制给定长度,给定位置的数据到指定位置.
void memcpy_uint32(uint32_t* dest,uint32_t* src,int len){
	int i=0;
	for(;i<len;i++)
		dest[i]=src[i];
}
void memcpy_uint16(uint16_t* dest,uint16_t* src,int len){
	int i=0;
	for(;i<len;i++)
		dest[i]=src[i];
}
void memcpy_uint8(uint8_t* dest,uint8_t* src,int len){
	int i=0;
	for(;i<len;i++)
		dest[i]=src[i];
}


// 获取字符串长度
int strlen(char* str){
	int len=0;
	while(str[len]!=0)
		len++;
	return len;
}


//比较字符串大小
int strcmp(const char* str1,const char* str2)
{	
	int pos=0;
	while(str1[pos]!=0&&str2[pos]!=0)
	{
		if(str1[pos]>str2[pos])
			return 1;
		else if(str1[pos]<str2[pos])
			return -1;
		else
			pos++;
	}
	
	// console_write_dec(pos,rc_blue,rc_black);console_write("  ");
	// console_write_dec(str1[pos],rc_blue,rc_black);console_write("  ");
	// console_write_dec(str2[pos],rc_blue,rc_black);console_write("  ");;

	// 判断2个字符串到达尾部的情况.
	// if(str1[pos]==0&&str2[pos]==0) return 0;
	// if(str1[pos]==0) return -1;
	// if(str2[pos]==0) return 1;

	// 我觉得这种判断更好一点, 因为只要到达末尾了, 就是0, 那就是ascii码里面最小的
	if(str1[pos]==str2[pos])
		return 0;
	else if(str1[pos]>str2[pos])
		return 1;
	else
		return -1;
}

// 复制src字符串的内容到dest字符串
void strcpy(char* dest,char* src)
{
	int len=strlen(src);
	int i=0;
	for(;i<=len;i++)
		dest[i]=src[i];
}

// 把src字符串的内容连接到dest字符串后面
void strcat(char* dest,char* src)
{
	int lenDest=strlen(dest);
	int lenSrc=strlen(src);
	int i=0;
	for(;i<lenSrc;i++)
		dest[lenDest+i]=src[i];
}