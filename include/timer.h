/*
 *文件名: timer.h.
 *作者: 李程鹏 on 18-2-13.
 *描述: 时钟相关的中断
*/

#ifndef OS_TIMER_H
#define OS_TIMER_H

#include "idt.h"
#include "console.h"
#include "types.h"

// 处理时钟中断的回调函数
void timer_call(regs* registers);

// 初始化时钟中断函数
void init_timer(uint32_t frequency);

// 获取当前虚拟时间
uint32_t get_vitual_timer();

#endif //OS_TIMER_H
