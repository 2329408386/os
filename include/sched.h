/*
 *文件名: sched.h.
 *作者: 李程鹏 on 18-3-6.
 *描述: 对调度机制的一些定义
*/

#ifndef OS_SCHED_H
#define OS_SCHED_H

#include "task.h"

extern pcb* running_proc;   //可调度进程列表
extern pcb* waiting_proc;   //等待进程列表
extern pcb* current;        //当前正在运行的任务

// 初始化任务调度
void init_sched();

// 任务调度
void schedule();

// 任务切换准备
void change_task_to(pcb* next);

// 任务切换
void switch_to(context* prev,context* next);

#endif //OS_SCHED_H
