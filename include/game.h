/*
*文件名:game.h
* 作者: 李程鹏 on Mar 10, 2018
* 描述: 包含一些用户可以玩的小游戏
*/

#ifndef GAME_H
#define GAME_H 

#include "types.h"

// 猜数游戏, number为该数最大值
void guess(uint32_t number);

// 一个内核线程切换的小游戏, number为线程数量, 而且不能大于5.
void counter(uint32_t number);

// 通过输入数字来决定每次跳跃的个数, 输入0时表示结束.
void jump(uint32_t steps);

#endif