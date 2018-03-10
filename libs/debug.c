/*
 *文件名: debug.c.
 *作者: 李程鹏 on 18-2-5.
 *描述: 实现几个内核级测试函数
*/

#include "debug.h"
#include "multiboot.h"
#include "elf.h"
#include "types.h"
#include "print.h"

static elf_t kernel_elf;

static void print_track_trace();

// 初始化 Debug 信息
void init_debug()
{
    kernel_elf=elf_from_multiboot(glb_mboot_ptr);   //通过grub规范从multiboot中获取内核的elf信息
}

// 打印当前的段存器值
void print_cur_status()
{
    static int level=0;     //存放当前的运行级别
    uint16_t cs=0,ds=0,es=0,ss=0;

    // 提取出cs,ds,es和ss的内容(memory)
    asm volatile("mov %%cs,%0;mov %%ds,%1;mov %%es,%2;mov %%ss,%3":"=m"(cs),"=m"(ds),"=m"(es),"=m"(ss));

    print("\nCurrent Level:#d @ring #d\ncs=#d\nds=#d\nes=#d\nss=#d\n",level,cs&0x3,cs,ds,es,ss);
    level++;
}

// 打印当前的函数调用栈信息
void panic(const char *msg)
{
    print("---------#s\nTrack path is as follow:\n",msg);
    print_track_trace();
    print("That's end---------\n");

    // 当发生错误之后,就停留在这个地方了吧!
    while(1)
        ;
}

static void print_track_trace()
{
    uint32_t *eip,*ebp;

    // 提取ebp的值(value itself, not the memory it point).
    asm volatile("mov %%ebp,%0":"=r"(ebp));

    // 因为ebp的值在没有被改变的时候,会放在栈底的位置,即0,所以一直打印从当前函数处,到root函数,一直沿着向上的内容
    while(ebp){
        eip=ebp+1;  // 获得调用该函数的函数的地址.
        print("Address:#x\tName:#s\n",*eip,elf_lookup_symbol(*eip,&kernel_elf));  //打印出eip指向的函数的名称
        ebp=(uint32_t *)(*ebp);  //获取更上一级ebp的值.
    }
}