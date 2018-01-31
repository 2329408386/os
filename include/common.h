/**
*包含基本的输入输出接口
*/

#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H 

#include "types.h"

// Write a byte to port
void outb(uint16_t port,uint8_t value);

// Read a byte from port
uint8_t inb(uint16_t port);

// Read a word from port
uint16_t inw(uint16_t port);

#endif
