#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <lcom/lcf.h>
#include <stdlib.h>
#include <i8042.h>
#include <stdio.h>

int subscribe_kbd(uint8_t *bit_no);
int unsubscribe_keyboard();

#endif