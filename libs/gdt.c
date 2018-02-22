/*
 *文件名: gdt.c.
 *作者: 李程鹏 on 18-2-8.
 *描述: 对gdt初始化.
*/

#include "types.h"
#include "gdt.h"


//定义全局描述符表的长度
#define GDT_LENGTH 5

//存放全局描述符表(尽量少的设置全局变量吧, 不然重构代码的时候真的很头疼.)
static gdt_entry gdt_entries[GDT_LENGTH];

//存放gdtr(这个结构体体应该只允许有一个? 因为只有一个gdtr寄存器啊).
static gdtr my_gdtr;

/**
 * 设置gdt表的某一个项
 * @param num :对应的项
 * @param base :该项的段基址
 * @param limit :该项的限长
 * @param property :对应的属性信息
 */
static void set_gdt_gate(uint32_t num,uint32_t base,uint16_t limit_low,uint16_t property)
{
    gdt_entries[num].base_low=base&0xffff;
    gdt_entries[num].base_mid=base&0xff0000;
    gdt_entries[num].base_high=base&0xff000000;

    gdt_entries[num].limit_low=limit_low;
    gdt_entries[num].property=property;
}



void init_gdt()
{
    my_gdtr.base=(uint32_t )&gdt_entries;   //将全局描述表的开始地址放到gdtr结构体中.
    // (这里&gdt_entries和gdt_entries,&gdt_entries[0]是一样的,都是获取gdt_entries数组的首地址.
    my_gdtr.limit=sizeof(gdt_entry)*GDT_LENGTH-1;   //将全局描述符表的长度信息放到gdtr结构体中

    set_gdt_gate(0,0,0,0);      // 全局描述符表的第一个描述符必须为0
    set_gdt_gate(1,0,0xffff,0xcf9a);     //  权限为0的代码段.
    set_gdt_gate(2,0,0xffff,0xcf92);     //  权限为0的数据段
    set_gdt_gate(3,0,0xffff,0xcffa);     //  权限为3的代码段
    set_gdt_gate(4,0,0xffff,0xcff2);     //  权限为3的数据段

    gdt_flush((uint32_t)&my_gdtr);
}
