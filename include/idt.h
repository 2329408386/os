/*
 *文件名: idt.h.
 *作者: 李程鹏 on 18-2-9.
 *描述: 中断描述符表的基本信息.
 * IDT(Interrup Descriptor Table):
 * {
 *      偏移量: 入口函数地址的偏移量(63~47:高16位地址, 15~0:低16位地址)
 *      段选择符: 入口函数所处代码段的选择符: 31~16.
 *      P:段是否在内存中的标志, 47位
 *      DPL:段描述符的特权级, 46~45位
 *      0D110: 44~40位, D是标志位,D为0,表示16位,D为1,表示32位.
 *      39~37:000
 *      36~32: 系统保留
 * }
*/

#ifndef OS_IDT_H
#define OS_IDT_H

#include "types.h"
#include "print.h"

// 定义中段描述符(顺序必须按照idt规定的顺序)
typedef struct idt{
    uint16_t base_low;      //   入口函数地址的低16位
    uint16_t selector;      //   入库函数所处代码段的段描述符
    uint16_t property;      //   idt的属性
    uint16_t base_high;       //   入口函数地址的高16位.
}__attribute__((packed)) idt;


//idtr寄存器
typedef struct idtr{
    uint16_t limit;     //限长
    uint32_t base;      //基址
}__attribute__((packed)) idtr;

//用户手动保存和cpu自动保存的寄存器
typedef struct regs{
    // 这一部分我都不删了,作为一种提示吧, 必须满足压栈的顺序才行啊, 就是这样的,这是汇编的东西,如果想按照自己定义的也可以
    // 那就不能用pusha命令,pusha顺序(eax,ecx,edx,ebx,old esp,ebp,esi,edi). (删了吧,就记住顺序必须这样就行了.)
    
    //需要用户自己保存的寄存器信息
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t int_no;    //中断号
    uint32_t error_code;    //错误码

    // 处理器自动压入的寄存器的顺序(ss,user_esp(用户esp),e_flags,cs,eip)
    uint32_t eip;
    uint32_t cs;
    uint32_t e_flags;   // 保存当前cpu状态的寄存器.
    uint32_t user_esp;
    uint32_t ss;

} regs;

// 定义一个函数指针,函数名为interrupt_handler, 参数为regs,返回值类型为void(加上typedef之后的好处就是,
// 此时的interrupt_handler相当于是一种新的类型了,以后可以像使用其它类型一样使用).
typedef void (*interrupt_handler)(regs*);

//注册一个中断处理函数(n表示中断号,handler表示一个指向上面定义类型的函数.
void register_interrupt_handler(uint8_t n,interrupt_handler handler);

//调用中断处理函数.
void isr_handler(regs*);

// 将定义的idtr结构体的内容加载到idtr寄存器.
void idt_flush(uint32_t);

// 初始化idt表的信息.
void init_idt();

// 声明中断处理函数 0-19 属于 CPU 的异常中断
// ISR:中断服务程序(interrupt service routine)
void isr0(); 		// 0 #DE 除 0 异常
void isr1(); 		// 1 #DB 调试异常
void isr2(); 		// 2 NMI
void isr3(); 		// 3 BP 断点异常
void isr4(); 		// 4 #OF 溢出
void isr5(); 		// 5 #BR 对数组的引用超出边界
void isr6(); 		// 6 #UD 无效或未定义的操作码
void isr7(); 		// 7 #NM 设备不可用(无数学协处理器)
void isr8(); 		// 8 #DF 双重故障(有错误代码)
void isr9(); 		// 9 协处理器跨段操作
void isr10(); 		// 10 #TS 无效TSS(有错误代码)
void isr11(); 		// 11 #NP 段不存在(有错误代码)
void isr12(); 		// 12 #SS 栈错误(有错误代码)
void isr13(); 		// 13 #GP 常规保护(有错误代码)
void isr14(); 		// 14 #PF 页故障(有错误代码)
void isr15(); 		// 15 CPU 保留
void isr16(); 		// 16 #MF 浮点处理单元错误
void isr17(); 		// 17 #AC 对齐检查
void isr18(); 		// 18 #MC 机器检查
void isr19(); 		// 19 #XM SIMD(单指令多数据)浮点异常

// 20-31 Intel 保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32～255 用户自定义异常
void isr255();


// 定义IRQ(interrupt request(中断请求)).
#define  IRQ0     32 	// 电脑系统计时器
#define  IRQ1     33 	// 键盘
#define  IRQ2     34 	// 与 IRQ9 相接，MPU-401 MD 使用
#define  IRQ3     35 	// 串口设备
#define  IRQ4     36 	// 串口设备
#define  IRQ5     37 	// 建议声卡使用
#define  IRQ6     38 	// 软驱传输控制使用
#define  IRQ7     39 	// 打印机传输控制使用
#define  IRQ8     40 	// 即时时钟
#define  IRQ9     41 	// 与 IRQ2 相接，可设定给其他硬件
#define  IRQ10    42 	// 建议网卡使用
#define  IRQ11    43 	// 建议 AGP 显卡使用
#define  IRQ12    44 	// 接 PS/2 鼠标，也可设定给其他硬件
#define  IRQ13    45 	// 协处理器使用
#define  IRQ14    46 	// IDE0 传输控制使用
#define  IRQ15    47 	// IDE1 传输控制使用


// 声明 IRQ 函数(具体定义在汇编中完成(libs/idt_t.s).
// IRQ:中断请求(Interrupt Request)
void irq0();		// 电脑系统计时器
void irq1(); 		// 键盘
void irq2(); 		// 与 IRQ9 相接，MPU-401 MD 使用
void irq3(); 		// 串口设备
void irq4(); 		// 串口设备
void irq5(); 		// 建议声卡使用
void irq6(); 		// 软驱传输控制使用
void irq7(); 		// 打印机传输控制使用
void irq8(); 		// 即时时钟
void irq9(); 		// 与 IRQ2 相接，可设定给其他硬件
void irq10(); 		// 建议网卡使用
void irq11(); 		// 建议 AGP 显卡使用
void irq12(); 		// 接 PS/2 鼠标，也可设定给其他硬件
void irq13(); 		// 协处理器使用
void irq14(); 		// IDE0 传输控制使用
void irq15(); 		// IDE1 传输控制使用

// IRQ 处理函数(irq: interrupt request).
void irq_handler(regs *regs);

#endif //OS_IDT_H
