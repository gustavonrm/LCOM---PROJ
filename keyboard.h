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
void Draw_string(int x, int y); 
void keyboard_utilities(uint16_t key);
//player name stuff
void GetPlayerName(uint16_t key); 
void DrawPlayerName();

bool getSpell();


#endif
