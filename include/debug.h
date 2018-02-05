/*
 *文件名: debug.h.
 *作者: 李程鹏 on 18-2-5.
 *描述: 内核级测试函数
*/

#ifndef OS_DEBUG_H
#define OS_DEBUG_H

// 初始化 Debug 信息
void init_debug();

// 打印当前的段存器值
void print_cur_status();

// 打印当前的函数调用栈信息
void panic(const char *msg);

#endif //OS_DEBUG_H
