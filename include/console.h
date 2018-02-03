/**
*包含对控制台的一些基本操作
*/

#ifndef INCLUDE_CONSOLE_H
#define INCLUDE_CONSOLE_H 

#include "types.h"

// 定义颜色(可用于前景色或者背景色) 
typedef enum real_color
{
	rc_black=0,
	rc_blue=1,
	rc_green=2,
	rc_cyan=3,
	rc_red=4,
	rc_magenta=5,
	rc_brown=6,
	rc_light_grey=7,
	rc_dark_grey=8,
	rc_light_blue=9,
	rc_light_green=10,
	rc_light_cyan=11,
	rc_light_red=12,
	rc_light_magenta=13,
	rc_light_brown=14,
	rc_white=15
}real_color_t;

// 清屏操作
void console_clear();

// 输出一个带颜色的字符
void console_putc_color(char c,real_color_t fore,real_color_t back);

// 输出一个以'\0'结尾,黑色背景,白色前景色的字符串
void console_write(char *cstr);

// 输出一个以'\0'结尾,自定义前景色和背景色的字符串
void console_write_color(char *cstr,real_color_t fore,real_color_t back);

// 以16进制的形式输出一个自定义前景色和背景色的无符号整数,假设该整数是符合条件的整数
void console_write_hex(uint32_t n,real_color_t fore,real_color_t back);

// 以10进制形式输出一个自定义前景色和背景色的无符号整数,假设该整数是符合条件的整数
void console_write_dec(uint32_t n,real_color_t fore,real_color_t back);

// 以10进制形式输出一个自定义前景色和背景色的有符号整数,假设该整数是符合条件的整数
void console_write_dec_sign(int32_t n,real_color_t fore,real_color_t back);

// 打印一个浮点数f,小数点后保留len位.
void console_write_float(float f,int len,real_color_t fore,real_color_t back);

#endif