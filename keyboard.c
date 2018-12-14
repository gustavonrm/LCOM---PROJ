#include <lcom/lcf.h>
#include <keyboard.h>
#include "video_card.h"
#include "game.h"

extern bool openTextBox;

extern Bitmap *P_Cursor;

int kbd_hook_id = 1;
//char words[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
char words[50] = ""; 
unsigned int text_index = 0;
//alphabet
extern Bitmap *Letter_A;
extern Bitmap *Letter_B;
extern Bitmap *Letter_C;
extern Bitmap *Letter_D;
extern Bitmap *Letter_E;
extern Bitmap *Letter_F;
extern Bitmap *Letter_G;
extern Bitmap *Letter_H;
extern Bitmap *Letter_I;
extern Bitmap *Letter_J;
extern Bitmap *Letter_K;
extern Bitmap *Letter_L;
extern Bitmap *Letter_M;
extern Bitmap *Letter_N;
extern Bitmap *Letter_O;
extern Bitmap *Letter_P;
extern Bitmap *Letter_Q;
extern Bitmap *Letter_R;
extern Bitmap *Letter_S;
extern Bitmap *Letter_T;
extern Bitmap *Letter_U;
extern Bitmap *Letter_V;
extern Bitmap *Letter_W;
extern Bitmap *Letter_X;
extern Bitmap *Letter_Y;
extern Bitmap *Letter_Z;

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
  // words[0] = 'B';
  //text_index = 1;
  printf("index num = %d\n", text_index);
  switch (key)
  {
  case A:
    words[text_index] = 'A';
    break;
  case B:
    words[text_index] = 'B';
    break;
  case C:
    words[text_index] = 'C';
    break;
  case D:
    words[text_index] = 'D';
    break;
  case E:
    words[text_index] = 'E';
    break;
  case F:
    words[text_index] = 'F';
    break;
  case G:
    words[text_index] = 'G';
    break;
  case H:
    words[text_index] = 'H';
    break;
  case I:
    words[text_index] = 'I';
    break;
  case J:
    words[text_index] = 'J';
    break;
  case K:
    words[text_index] = 'K';
    break;
  case L:
    words[text_index] = 'L';
    break;
  case M:
    words[text_index] = 'M';
    break;
  case N:
    words[text_index] = 'N';
    break;
  case O:
    words[text_index] = 'O';
    break;
  case P:
    words[text_index] = 'P';
    break;
  case Q:
    words[text_index] = 'Q';
    break;
  case R:
    words[text_index] = 'R';
    break;
  case S:
    words[text_index] = 'S';
    break;
  case T:
    words[text_index] = 'T';
    break;
  case U:
    words[text_index] = 'U';
    break;
  case V:
    words[text_index] = 'V';
    break;
  case W:
    words[text_index] = 'W';
    break;
  case X:
    words[text_index] = 'X';
    break;
  case Y:
    words[text_index] = 'Y';
    break;
  case Z:
    words[text_index] = 'Z';
    break;
  case SPACEBAR:
    words[text_index] = ' ';
    break;
  case BACKSPACE:
    if( words[0]== '\0'){
         // text_index--;
      return;
    }
    if( words[1]== '\0'){
      text_index--;
       words[text_index] = '\0';
      return;
    }
    words[text_index] = '\0';
    text_index--;
    text_index--; 
    break;
  case ENTER:
    text_index = 0;
    break;
  }
  if (words[text_index] != '\0')
  {
    text_index++;
    words[text_index] = '\0';
  }
}
char palavraTeste[] = "ABRAKADABRA";

void Draw_string()
{

  int x = 20, y = 605;
  for (size_t i = 0; i < text_index/*strlen(words)*/; i++)
  {
    x += 12;
    switch (words[i])
    {
    case 'A':
      DrawBitmap(Letter_A, x, y);
      break;
    case 'B':
      DrawBitmap(Letter_B, x, y);
      break;
    case 'C':
      DrawBitmap(Letter_C, x, y);
      break;
    case 'D':
      DrawBitmap(Letter_D, x, y);
      break;
    case 'E':
      DrawBitmap(Letter_E, x, y);
      break;
    case 'F':
      DrawBitmap(Letter_F, x, y);
      break;
    case 'G':
      DrawBitmap(Letter_G, x, y);
      break;
    case 'H':
      DrawBitmap(Letter_H, x, y);
      break;
    case 'I':
      DrawBitmap(Letter_I, x, y);
      break;
    case 'J':
      DrawBitmap(Letter_J, x, y);
      break;
    case 'K':
      DrawBitmap(Letter_K, x, y);
      break;
    case 'L':
      DrawBitmap(Letter_L, x, y);
      break;
    case 'M':
      DrawBitmap(Letter_M, x, y);
      break;
    case 'N':
      DrawBitmap(Letter_N, x, y);
      break;
    case 'O':
      DrawBitmap(Letter_O, x, y);
      break;
    case 'P':
      DrawBitmap(Letter_P, x, y);
      break;
    case 'Q':
      DrawBitmap(Letter_Q, x, y);
      break;
    case 'R':
      DrawBitmap(Letter_R, x, y);
      break;
    case 'S':
      DrawBitmap(Letter_S, x, y);
      break;
    case 'T':
      DrawBitmap(Letter_T, x, y);
      break;
    case 'U':
      DrawBitmap(Letter_U, x, y);
      break;
    case 'V':
      DrawBitmap(Letter_V, x, y);
      break;
    case 'W':
      DrawBitmap(Letter_W, x, y);
      break;
    case 'X':
      DrawBitmap(Letter_X, x, y);
      break;
    case 'Y':
      DrawBitmap(Letter_Y, x, y);
      break;
    case 'Z':
      DrawBitmap(Letter_Z, x, y);
      break;
    }
  }
}

void keyboard_utilities(uint16_t key)
{

  //if (key == U)
  //UpdateVideo();

  if (openTextBox == true)
  {
    printf("string= %s\n ", words);
    printf("index num = %d\n", text_index);
    write_key(key); //erro esta aqui
    UpdateVideo();
  }
  if (key == TAB)
  {
    text_index = 0;
    openTextBox = true;
    UpdateVideo();
  }
  if (key == ENTER)
  {
    //getSpell(); 
    //just in case 
    write_key(key);
    //reset
    openTextBox = false;
    text_index = 0;
    //words[0] = '\0';
    strcpy(words, "");
    UpdateVideo();
  }
  if (openTextBox == false)
  {
    //strcpy(&words,"");
    UpdateVideo();
  }
}

/*
char fire[50];
char wind[50]; 
char earth[50]; 
char water[50]

void getSpell(){
   if(words== fire){
     //condicao para fazer aparecer o bmp respetivos
     printf( "Spell fogo\n "); 
   }
    if(words== wind){
     //condicao para fazer aparecer o bmp respetivos
     printf( "Wind Spell Casted!\n "); 
   }
    if(words== earth){
     //condicao para fazer aparecer o bmp respetivos
     printf( "Earth Spell cast\n "); 
   }
    if(words== water){
     //condicao para fazer aparecer o bmp respetivos
     printf( "Water Spell Casted\n "); 
   }


}*/
