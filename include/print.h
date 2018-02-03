/*
 *文件名: print.h.
 *作者: 李程鹏 on 18-2-3.
 *描述: 包含内核级打印函数的声明.
*/

#ifndef OS_PRINT_H
#define OS_PRINT_H

#include "console.h"

// 根据给出的字符串的格式和可变参数列表以及颜色,打印带颜色的字符串
void printk_color(real_color_t fore,real_color_t back,char* format,...);

#endif //OS_PRINT_H
