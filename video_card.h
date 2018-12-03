#ifndef _VIDEO_CARD_H_
#define _VIDEO_CARD_H_

#include <lcom/lcf.h>
#include "macros.h"

uint16_t getXRes();
uint16_t getYRes();
uint8_t getByts_per_pixel();
uint8_t getMemoryModel();
uint32_t direct_color(uint16_t row, uint16_t col, uint32_t first, uint8_t step);
uint8_t getRedMaskSize();
uint8_t getGreenMaskSize();
uint8_t getBlueMaskSize();

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int vg_draw_line( uint16_t x, uint16_t y,uint16_t len,uint32_t color );
int draw_xpm(xpm_image_t *image, uint8_t* map, uint16_t x, uint16_t y);
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void UpdateVideo();


////SPRITE////

typedef struct {
      int x, y; // current position
      int xf, yf; //final position
      bool x_dir, y_dir; //true if positive direction;
      bool moving; //movement complete or not
      int width, height;  // dimensions
      int xspeed, yspeed; // current speed
      char *map;          // the pixmap
} Sprite;

Sprite *create_sprite(const char *pic[], int x, int y, int xf, int yf, int xspeed, int yspeed);
void destroy_sprite(Sprite *sp);
int draw_sprite(Sprite *sp);

int new_vbe_get_mode_info(uint16_t mode,vbe_mode_info_t * vmi_p );	
int vbe_get_ctrl_info(); 

typedef struct __attribute__((packed)){
  char VBESignature[4]; 
  BCD VBEVersion[2];
  uint32_t OEMString;
  uint8_t Capabilities[4];
  uint32_t VideoModeList;
  uint16_t TotalMemory; 
  uint16_t OEMSoftwareRevision; 
  uint32_t OEMVendorNamePtr;
  uint32_t OEMProductNamePtr;
  uint32_t OEMProductRevPtr;
  uint8_t Reserved[222];
  uint8_t OEMData[256];
} aux_ctrl_info_t;


#endif
