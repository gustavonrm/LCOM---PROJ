#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <lcom/lcf.h>
#include <stdlib.h>
#include <i8042.h>
#include <stdio.h>
#include "game.h"

/**
 * @brief Subscribes Keyboard interrupts
 * @param bit_no Used to figure out which interrupts belong to the keybaord
 * @return returns 0 upos successfully subscribing keybaord
 */
int subscribe_kbd(uint8_t *bit_no);
/**
 * @brief Unsubscrbies Keybaord interrupts
 * @return returns 0 if OK
 */
int unsubscribe_keyboard();
/**
 * @brief Handles the Keyboardinterruption to get a correct Make or Break value
 * @return Returns the Value for the pressed key
 */
uint16_t kbd_ih();

/**
 * @brief writes the key to the words string
 * @param key key pressed
 */
void write_key(uint16_t key);
/**
 * @brief Draws the words string on the indicated location
 * @param x x position
 * @param y y position
 */
void Draw_string(int x, int y);
/**
 * @brief Checks what action to proceed with depending on the input and game state
 * @param key Key pressed
 */
void keyboard_utilities(uint16_t key);
//player name stuff
/**
 * @brief Gets the player name string at the biggining of the game
 * @param key key pressed
 */
void GetPlayerName(uint16_t key);
/**
 * @brief Draws the player name as it is being written
 */
void DrawPlayerName();
/**
 * @brief Gets the appropriate spell depending on the input and timer states
 * @return returns true of a spell was casted
 */
bool getSpell();


#endif
