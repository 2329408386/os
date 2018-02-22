/*
 *文件名: idt.c.
 *作者: 李程鹏 on 18-2-12.
 *描述: 实现一些关于中断向量表的函数.
*/

#include "idt.h"
#include "types.h"
#include "string.h"
#include "debug.h"
#include "console.h"
#include "common.h"


// 存放中断描述符表
static idt idt_entries[256];

// 中段描述符寄存器
static idtr my_idtr;

// 存放中断处理函数的指针数组
static interrupt_handler interrupt_handlers[256];

/**
 * 设置中断描述符(具体一点就是设置基地址, 段选择符和属性信息).
 * @param num :该中断门所处的位置
 * @param base :入口函数地址的偏移量
 * @param selector :段选择符
 * @param property :中断描述符的属性.
 */
static void idt_set_gate(uint32_t num,uint32_t base,uint16_t selector,uint16_t property){
    idt_entries[num].base_low=base&0x0000ffff;
    idt_entries[num].base_high=(base>>16)&0x0000ffff;
    idt_entries[num].selector=selector;
    idt_entries[num].property=property;
}

//调用中断处理函数.
void isr_handler(regs* registers){
    if(interrupt_handlers[registers->int_no]){
        interrupt_handlers[registers->int_no](registers);
    }else{
        // 这里我就是想证明一点,结构体的变量之间是连续存放的,这里的"*((uint32_t*)registers+9)"就相当于是registers->int_no.
        // 懂了这一点就知道为什么在设置结构体时顺序那么重要了,必须保持压栈的顺序才行.
        print_color(rc_red,rc_black,"NO.#d interrupt handler(CPU) hasn't been registered.\n",*((uint32_t*)registers+9));
    }
}


// IRQ 处理函数(irq: interrupt request). PS:其实就是用户定义的一些和外设打交道的函数.
void irq_handler(regs *regs){
    //发射我们的中断信号给pic芯片, 因为单个pic芯片只能处理8个中断源,
    //我们的irq请求的起始中断号为0x20,所以大于40时,需要同时使用从芯片和主芯片.
    if(regs->int_no>=40)
        outb(0xa0,0x20);    //发送设置信号给从芯片
    outb(0x20,0x20);        //发送设置信号给主芯片

    if(interrupt_handlers[regs->int_no])    //当该中断函数经过注册了
        interrupt_handlers[regs->int_no](regs);
//    else
//        print_color(rc_red,rc_black,"No.#d interrupt handler(User) hasn't registered. \n",regs->int_no);

}

//注册一个中断处理函数(n表示中断号,handler表示一个指向上面定义类型的函数.
void register_interrupt_handler(uint8_t n,interrupt_handler handler){
    interrupt_handlers[n]=handler;
}

// 对8259A PIC芯片进行初始化
// 8259A PIC芯片是2级联级的,分为主芯片和从芯片. 主芯片的端口为 0x20(代码),0x21(数据),从芯片的端口为0xA0(代码),0xA1(数据)
static void pic_init(){

    // 初始化主片、从片
    // 0001 0001(7~5:中断向量地址的A7~A5,4:1,3:边缘触发输入,2:调用间隔为8,1:联级方式,0:是否写iwc4.)
    // iwc(initialization command word).
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // 设置主片 IRQ 从 0x20(32) 号中断开始
    outb(0x21, 0x20);

    // 设置从片 IRQ 从 0x28(40) 号中断开始
    outb(0xA1, 0x28);

    // 设置主片 IR2 引脚连接从片
    outb(0x21, 0x04);

    // 告诉从片输出引脚和主片 IR2 号相连
    outb(0xA1, 0x02);

    // 设置主片和从片按照 8086 的方式工作
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // 设置主从片允许中断
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

// 初始化idt表的信息.
void init_idt(){

    pic_init();     // 初始化8259a pic芯片.

    // 对中断处理函数清零操作.
    memset_uint32((uint32_t*)interrupt_handlers,0, 256 );

    // 设置idtr寄存器.
    my_idtr.limit= sizeof(idt)*256-1;
    my_idtr.base=(uint32_t)idt_entries;

    // 对中断向量表置0操作.
    memset_uint32((uint32_t*)idt_entries,0,256);

    // 设置中断向量表的入口地址和属性.(0~31为cpu内定的,32~255可以用户定义).
    idt_set_gate(0,(uint32_t)isr0,0x8,0x8e00);
    idt_set_gate(1,(uint32_t)isr1,0x8,0x8e00);
    idt_set_gate(2,(uint32_t)isr2,0x8,0x8e00);
    idt_set_gate(3,(uint32_t)isr3,0x8,0x8e00);
    idt_set_gate(4,(uint32_t)isr4,0x8,0x8e00);
    idt_set_gate(5,(uint32_t)isr5,0x8,0x8e00);
    idt_set_gate(6,(uint32_t)isr6,0x8,0x8e00);
    idt_set_gate(7,(uint32_t)isr7,0x8,0x8e00);
    idt_set_gate(8,(uint32_t)isr8,0x8,0x8e00);
    idt_set_gate(9,(uint32_t)isr9,0x8,0x8e00);
    idt_set_gate(10,(uint32_t)isr10,0x8,0x8e00);
    idt_set_gate(11,(uint32_t)isr11,0x8,0x8e00);
    idt_set_gate(12,(uint32_t)isr12,0x8,0x8e00);
    idt_set_gate(13,(uint32_t)isr13,0x8,0x8e00);
    idt_set_gate(14,(uint32_t)isr14,0x8,0x8e00);
    idt_set_gate(15,(uint32_t)isr15,0x8,0x8e00);
    idt_set_gate(16,(uint32_t)isr16,0x8,0x8e00);
    idt_set_gate(17,(uint32_t)isr17,0x8,0x8e00);
    idt_set_gate(18,(uint32_t)isr18,0x8,0x8e00);
    idt_set_gate(19,(uint32_t)isr19,0x8,0x8e00);
    idt_set_gate(20,(uint32_t)isr20,0x8,0x8e00);
    idt_set_gate(21,(uint32_t)isr21,0x8,0x8e00);
    idt_set_gate(22,(uint32_t)isr22,0x8,0x8e00);
    idt_set_gate(23,(uint32_t)isr23,0x8,0x8e00);
    idt_set_gate(24,(uint32_t)isr24,0x8,0x8e00);
    idt_set_gate(25,(uint32_t)isr25,0x8,0x8e00);
    idt_set_gate(26,(uint32_t)isr26,0x8,0x8e00);
    idt_set_gate(27,(uint32_t)isr27,0x8,0x8e00);
    idt_set_gate(28,(uint32_t)isr28,0x8,0x8e00);
    idt_set_gate(29,(uint32_t)isr29,0x8,0x8e00);
    idt_set_gate(30,(uint32_t)isr30,0x8,0x8e00);
    idt_set_gate(31,(uint32_t)isr31,0x8,0x8e00);

    // 设置一些外部设备的入口地址.
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8e00);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8e00);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8e00);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8e00);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8e00);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8e00);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8e00);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8e00);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8e00);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8e00);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8e00);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8e00);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8e00);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8e00);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8e00);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8e00);


    idt_set_gate(255,(uint32_t)isr255,0x8,0x8e00);

    idt_flush((uint32_t)&my_idtr);
}