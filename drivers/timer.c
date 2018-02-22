/*
 *文件名: times.c.
 *作者: 李程鹏 on 18-2-13.
 *描述: 对时钟中断的一些定义
*/

#include "timer.h"
#include "console.h"
#include "idt.h"
#include "common.h"

// 处理时钟中断的回调函数
void timer_call(regs* registers){
    static int tick=0;
    print("Now is:#d\n",tick++);
}

// 初始化时钟中断函数
void init_timer(uint32_t frequency){
    // 注册时钟中断函数.
    register_interrupt_handler(IRQ0,timer_call);

    asm volatile("sti");    //打开中断

    // Intel 8253/8254 PIT芯片 I/O端口地址范围是40h~43h
    // 输入频率为 1193180，frequency 即每秒中断次数
    uint32_t divisor = 1193180 / frequency;

    // D7 D6 D5 D4 D3 D2 D1 D0
    // 0  0  1  1  0  1  1  0
    // (D7~D6: 00表示设置的是counter0, D5~D4:11表示可读可写, D3~D1:011表示在模式3下工作,D0:0表示计时采用16bits).
    // 设置 8253/8254 芯片工作在模式 3 下
    outb(0x43, 0x36);

    // 拆分低字节和高字节
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    // 分别写入低字节和高字节
    outb(0x40, low);
    outb(0x40, high);
}