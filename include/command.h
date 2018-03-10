/*
*文件名: command.h
* 作者: 李程鹏
* 描述: 有关命令行的一些操作
*/

#ifndef COMMAND_H
#define COMMAND_H 

// 打印当前支持的一些指令
void help();

// 根据输入的命令选择要进行的操作
void choose(char* command);

#endif