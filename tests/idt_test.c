/*
 *文件名: idt_test.c.
 *作者: 李程鹏 on 18-2-12.
 *描述: 对idt模块的测试
*/

#include "tests.h"
#include "idt.h"

// 对idt模块的测试
void idt_test(){
    init_idt();
//    asm volatile("int $0x0");
//    int a=3/0;
    asm volatile("int $0x03;");
    asm volatile("int $0x24;");
}