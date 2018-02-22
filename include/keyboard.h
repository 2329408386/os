/*
 *文件名: keyboard.h.
 *作者: 李程鹏 on 18-2-16.
 *描述: 
*/

#ifndef OS_KEYBOARD_H
#define OS_KEYBOARD_H

#include "types.h"
#include "idt.h"

// 键盘输入回调函数
void keyboard_call(regs* registers);

// 初始化键盘输入函数.
void init_keyboard();

// 输入一个字符串
char* scanf();

#endif //OS_KEYBOARD_H
