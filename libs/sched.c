/*
 *文件名: sched.c.
 *作者: 李程鹏 on 18-3-6.
 *描述: 
*/

#include "sched.h"
#include "heap.h"
#include "task.h"

pcb* running_proc=NULL;     //可调度的进程
pcb* waiting_proc=NULL;     //处于等待的进程
pcb* current=NULL;      //当前运行的任务

extern uint32_t kern_stack_top;

// 全局pid(可以用map把pid和对应的pcb模块关联起来).
extern pid_t now_pid;

// 初始化任务调度
void init_sched(){
    // 将当前执行流的地址设置为栈的最低处
    current=(pcb*)(kern_stack_top-STACK_SIZE);

    // 设置当前流的属性
    current->state=TASK_RUNNING;
    current->pid=now_pid++;
    current->stack=current;
    current->mm=NULL;

    // 将该线程队列设置为单向循环列表
    current->next=current;

    // 让可调度进程指向当前进程位置.
    running_proc=current;
}

// 任务调度
void schedule(){
    // 调度函数就是直接调用当前线程的下一个线程
    if(current)
        change_task_to(current->next);
}

// 任务切换准备
void change_task_to(pcb* next){
    if(current!=next){
        pcb* prev=current;
        current=next;
        switch_to(&(prev->context),&(current->context));
    }
}