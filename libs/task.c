/*
 *文件名: task.c.
 *作者: 李程鹏 on 18-3-7.
 *描述: 
*/

#include "vmm.h"
#include "pmm.h"
#include "heap.h"
#include "task.h"
#include "sched.h"
#include "string.h"
#include "print.h"

// 定义全局pid值
pid_t now_pid=0;

extern pcb* current;

// 内核线程的创建
uint32_t kernel_thread(int (*fn)(void* ) ,void* arg){

    // 未新任务分配栈空间
    pcb* new_task=(pcb*)kmalloc(STACK_SIZE);
    if(new_task==NULL) print("What a pity, there is something wrong when mallocing memory.");

    // 将分配的任务的低位设置为0.
    memset_uint8((uint8_t*)new_task,0, sizeof(pcb));

    new_task->state=TASK_RUNNING;
    new_task->pid=now_pid++;
    new_task->stack=current;    //把新分配任务的栈设置为当前进程
    new_task->mm=NULL;

    // 获取栈顶位置
    uint32_t *stack_top = (uint32_t *)((uint32_t)new_task + STACK_SIZE);

    // 存放线程参数列表
    *(--stack_top) = (uint32_t)arg;

    // 存放线程退出地址
    *(--stack_top) = (uint32_t)kthread_exit;

    // 存放函数地址
    *(--stack_top) = (uint32_t)fn;

    // 设置新任务的栈顶.
    new_task->context.esp=(uint32_t)new_task+STACK_SIZE- sizeof(uint32_t)*3;

    // 设置新任务的标志寄存器未屏蔽中断
    new_task->context.eflags=0x200;
    new_task->next=running_proc;

    // 找到当前任务队列,插入到末尾.
    pcb* tail=running_proc;
    if(tail==NULL) print_color(rc_red,rc_black,"You must initialize tail!");

    while(tail->next!=running_proc)
        tail=tail->next;

    tail->next=new_task;
    return new_task->pid;
}

// 线程退出函数(指的就是退出当前线程)
void kthread_exit(){
    register uint32_t val asm("eax");
    print_color(rc_red,rc_black,"Thread exit with value #d\n",val);
    while(1)
        ;
}