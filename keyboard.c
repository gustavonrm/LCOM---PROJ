#include <lcom/lcf.h>
#include <keyboard.h>
#include "video_card.h"

int kbd_hook_id = 1;
char words[] = "";
//alphabet
extern Bitmap *Letter_A;

int subscribe_kbd(uint8_t *kbd_bit_no)
{
  *kbd_bit_no = kbd_hook_id;
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != OK)
    return 1; //sets exclusive interrupts to kbd_hook_id
  return 0;
}

int unsubscribe_keyboard()
{
  if (sys_irqrmpolicy(&kbd_hook_id) != OK)
    return 1;
  else
    return 0;
}

uint16_t kbd_ih()
{
  bool make = false;
  uint8_t scan_bytes[2];
  uint8_t scancode = 0;
  uint32_t stat, data;
  if (sys_inb(STAT_REG, &stat) != OK)
    return 1;
  uint8_t st = (uint8_t)stat;
  if (stat & OBF)
  {
    if (sys_inb(OUT_BUF, &data) != OK)
      return 1;
    if ((st & (PAR_ERR | TO_ERR)) == 0)
    {
      scancode = (uint8_t)data;
    }
    else
    {
    } //in case there was an error simply discard the output buffer value
  }

  if (scancode == TWO_BYTES)
  {
    scan_bytes[0] = scancode;
    make = false;
  }
  else if (scan_bytes[0] == TWO_BYTES)
  {
    scan_bytes[1] = scancode;
    make = !((scancode & MAKE_TYPE) >> 7);                  //because it's make if it's set to 0
    return ((scan_bytes[0] << 8) | (scan_bytes[1] & 0xff)); //returns double byte key
  }
  else
  {
    scan_bytes[0] = scancode;
    make = !((scancode & MAKE_TYPE) >> 7);
    return scan_bytes[0]; //returns single byte key
  }

  return 1;
}

void write_key(uint16_t key)
{
  //words = "";
  char src[1];
  switch (key)
  {
  //letters
  case A:
    src[0] = 'A';
    strcat(words, src);
  case B:
    src[0] = 'B';
    strcat(words, src);
  case C:
    src[0] = 'C';
    strcat(words, src);
  case D:
    src[0] = 'D';
    strcat(words, src);
  case E:
    src[0] = 'E';
    strcat(words, src);
  case F:
    src[0] = 'F';
    strcat(words, src);
  case G:
    src[0] = 'G';
    strcat(words, src);
  case H:
    src[0] = 'H';
    strcat(words, src);
  case I:
    src[0] = 'I';
    strcat(words, src);
  case J:
    src[0] = 'J';
    strcat(words, src);
  case K:
    src[0] = 'K';
    strcat(words, src);
  case L:
    src[0] = 'L';
    strcat(words, src);
  case M:
    src[0] = 'M';
    strcat(words, src);
  case N:
    src[0] = 'N';
    strcat(words, src);
  case O:
    src[0] = 'O';
    strcat(words, src);
  case P:
    src[0] = 'P';
    strcat(words, src);
  case Q:
    src[0] = 'Q';
    strcat(words, src);
  case R:
    src[0] = 'R';
    strcat(words, src);
  case S:
    src[0] = 'S';
    strcat(words, src);
  case T:
    src[0] = 'T';
    strcat(words, src);
  case U:
    src[0] = 'U';
    strcat(words, src);
  case V:
    src[0] = 'V';
    strcat(words, src);
  case W:
    src[0] = 'W';
    strcat(words, src);
  case X:
    src[0] = 'X';
    strcat(words, src);
  case Y:
    src[0] = 'Y';
    strcat(words, src);
  case Z:
    src[0] = 'Z';
    strcat(words, src);
  //operations
  case ENTER:
    break;
  case BACKSPACE:

  case SPACEBAR:
    strcat(words, src);
  }
}

char palavra[] = "ABRAKADABRA";

void Draw_string()
{

  int x = 5, y = 650;
  for (size_t i = 0; i < strlen(palavra); i++)
  {
    x += 10;
    switch (palavra[i])
    {
    case 'A':
      DrawBitmap(Letter_A, x, y);
    case 'B':
      DrawBitmap(Letter_A, x, y);
    case 'C':
      DrawBitmap(Letter_A, x, y);
    case 'D':
      DrawBitmap(Letter_A, x, y);
    case 'E':
      DrawBitmap(Letter_A, x, y);
    case 'F':
      DrawBitmap(Letter_A, x, y);
    case 'G':
      DrawBitmap(Letter_A, x, y);
    case 'H':
      DrawBitmap(Letter_A, x, y);
    case 'I':
      DrawBitmap(Letter_A, x, y);
    case 'J':
      DrawBitmap(Letter_A, x, y);
    case 'K':
      DrawBitmap(Letter_A, x, y);
    case 'L':
      DrawBitmap(Letter_A, x, y);
    case 'M':
      DrawBitmap(Letter_A, x, y);
    case 'N':
      DrawBitmap(Letter_A, x, y);
    case 'O':
      DrawBitmap(Letter_A, x, y);
    case 'P':
      DrawBitmap(Letter_A, x, y);
    case 'Q':
      DrawBitmap(Letter_A, x, y);
    case 'R':
      DrawBitmap(Letter_A, x, y);
    case 'S':
      DrawBitmap(Letter_A, x, y);
    case 'T':
      DrawBitmap(Letter_A, x, y);
    case 'U':
      DrawBitmap(Letter_A, x, y);
    case 'V':
      DrawBitmap(Letter_A, x, y);
    case 'W':
      DrawBitmap(Letter_A, x, y);
    case 'X':
      DrawBitmap(Letter_A, x, y);
    case 'Y':
      DrawBitmap(Letter_A, x, y);
    case 'Z':
      DrawBitmap(Letter_A, x, y);
    }
  }
}
