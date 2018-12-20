#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <lcom/lcf.h>
#include <stdlib.h>
#include <i8042.h>
#include <stdio.h>
#include "game.h"

int subscribe_kbd(uint8_t *bit_no);
int unsubscribe_keyboard();
uint16_t kbd_ih();

void write_key(uint16_t key);
void Draw_string(); 
void keyboard_utilities(uint16_t key);

<<<<<<< HEAD
bool getSpell();
=======
//void getSpell();
>>>>>>> 6496d7626e210a09f5b8132f90e24bbc6900ce19


#endif
