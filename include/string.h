/*
*文件名: string.h
* 作者: 李程鹏
* 描述: 这个文件包含对字符串操作的基本定义
*/

#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H 

#include "types.h"

// 分别以32位,16位和8位的形式复制给定长度,给定值的数据到指定位置.
void memset_uint32(uint32_t* dest,uint32_t value, int len );
void memset_uint16(uint16_t* dest,uint16_t value, int len );
void memset_uint8(uint8_t* dest,uint8_t value, int len );

// 分别以32位,16位和8位的形式复制给定长度,给定位置的数据到指定位置.
void memcpy_uint32(uint32_t* dest,uint32_t* src,int len);
void memcpy_uint16(uint16_t* dest,uint16_t* src,int len);
void memcpy_uint8(uint8_t* dest,uint8_t* src,int len);

/*以下为基本的字符串操作函数*/

// 获取字符串长度
int strlen(char* str);

/*
 *比较字符串大小
 *return  0: str1==str2; 1:str1>str2; -1:str1<str2
 */
int strcmp(char* str1,char* str2);

// 复制src字符串的内容到dest字符串
void strcpy(char* dest,char* src);

// 把src字符串的内容连接到dest字符串后面
void strcat(char* dest,char* src);

#endif