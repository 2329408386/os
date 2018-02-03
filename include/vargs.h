/*
 *文件名: vargs.h.
 *作者: 李程鹏 on 18-2-3.
 *描述: 该文件包含可变长参数获取操作的一些定义
*/

#ifndef OS_VARGS_H
#define OS_VARGS_H

typedef __builtin_va_list va_list;

#define va_start(list,start) (__builtin_va_start(list,start))       //获取可变长参数列表的起始地址
#define va_arg(list,type)   (__builtin_va_arg(list,type))       //根据当前可变长参数列表的位置和类型,获取该参数
#define va_end(list)    (list=(va_list)0)           // 将当前可变长参数地址清零

#endif //OS_VARGS_H
