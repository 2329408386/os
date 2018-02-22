/*
 *文件名: gdt.h.
 *作者: 李程鹏 on 18-2-7.
 *描述: 这个文件我准备先加一些理论知识(具体见Intel帮助文档):
 * GDT,LDT(Global Descriptor Table, Local Descriptor Table)结构:
 * {
 *      第7字节: 段基址31...24;
 *      第6~5字节:
 *      {
 *          第15位: G,G=0时，段限长的20位为实际段限长，最大限长为2^20=1MB; G=1时，则实际段限长为20位段限长乘以2^12=4KB，最大限长达到4GB
 *          第14位: D/B：当描述符指向的是可执行代码段时，这一位叫做D位，D=1使用32位地址和32/8位操作数，D=0使用16位地址和16/8位操作数。如果指向的是向下扩展的数据段，这一位叫做B位，B=1时段的上界为4GB，B=0时段的上界为64KB。如果指向的是堆栈段，这一位叫做B位，B=1使用32位操作数，堆栈指针用ESP，B=0时使用16位操作数，堆栈指针用SP。
 *          第13位: 0.
 *          第12位: ALV(available and reversed bit), 通常为0
 *          第11~8位: 段限长的19~16位
 *          第7位: P,存在位,P=1表示段在内存中
 *          第6~5位: DPL：特权级，0为最高特权级，3为最低，表示访问该段时CPU所需处于的最低特权级
 *
 *          第4位: S位
 *          第3~0位: TYPE位
 *          S=1表示该描述符指向的是代码段或数据段；S=0表示系统端（TSS(Task State Segment?)、LDT(Local Descriptor Table?）和门描述符
 *           S=1且TYPE<8时，为数据段描述符。数据段都是可读的，但不一定可写。
 *            S=1且TYPE>=8时，为代码段描述符。代码段都是可执行的，一定不可写，不一定可读
 *              S=0时，描述符可能为TSS、LDT和4种门描述符
 *
 *      }
 *      第4~2字节: 段基址23...0;
 *      第1~0字节: 限长15...0;
 * }
 * GDTR,LDTR:
 * {
 *      47~16位: GDT在内存中的起始位置
 *      15~0位: GDT的长度
 * }
*/

#ifndef OS_GDT_H
#define OS_GDT_H

#include "types.h"

/*我这里应该有问题, 按照规范, 汇编可不知道这么多信息,那么就应该严格按照从低位到高位存储.*/
// 全局描述符
typedef struct gdt_entry{
    uint16_t limit_low;     //限长的低16位
    uint16_t base_low;      //段基址的低16位
    uint8_t base_mid;       //段基址的中间8位
    uint16_t property;      //属性值,限长的高4位
    uint8_t base_high;      //段基址的高8位
}__attribute__((packed))gdt_entry;  //(这里不是对齐好不好的问题,是根本不能让它对齐).

/*gdtr结构体也和上面的一样,限长在低16位,段基址在高32位*/
// gdtr寄存器
typedef struct gdtr{
    uint16_t limit;     // gdt的最大长度.
    uint32_t base;   // gdt的基地址
}__attribute__((packed))gdtr;

// 初始化gdt表和gdtr寄存器(定义的结构体)
void init_gdt();

// 将gdtr结构体变量的内容加载到gdtr寄存器(真实的物理寄存器)中, 并初始化段选择器(cs,ds,es,fs,gs,ss).
void gdt_flush(uint32_t);


#endif //OS_GDT_H
