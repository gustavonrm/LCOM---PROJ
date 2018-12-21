#include <lcom/lcf.h>

#include "video_card.h"
#include "macros.h"
#include "bitmap.h"
#include "game.h"
#include "keyboard.h"
#include <math.h>

static uint16_t X_Res;
static uint16_t Y_Res;
static uint8_t B_per_pixel;
static void *video_mem;
static void *video_buffer;
static uint32_t video_size;
static uint8_t mem_model;
static uint8_t red_mask_size;
static uint8_t green_mask_size;
static uint8_t blue_mask_size;
static uint8_t RedFieldPosition;
static uint8_t GreenFieldPosition;
static uint8_t BlueFieldPosition;

extern Bitmap *background;

//TODO: remove this
extern Bitmap *Fire_0;
extern Bitmap *Earth_0;
extern Bitmap *Water_0;
extern Bitmap *Wind_0;

uint16_t getXRes()
{
  return X_Res;
}

uint16_t getYRes()
{
  return Y_Res;
}

uint8_t getByts_per_pixel()
{
  return B_per_pixel;
}

uint8_t getMemoryModel()
{
  return mem_model;
}

uint8_t getRedMaskSize()
{
  return red_mask_size;
}

uint8_t getGreenMaskSize()
{
  return green_mask_size;
}

uint8_t getBlueMaskSize()
{
  return blue_mask_size;
}

uint8_t getRedFieldPosition()
{
  return RedFieldPosition;
}

uint8_t getGreenFieldPosition()
{
  return GreenFieldPosition;
}

uint8_t getBlueFieldPosition()
{
  return BlueFieldPosition;
}

///////////////////////////////////

void *(vg_init)(uint16_t mode)
{
  struct reg86u reg;

  vbe_mode_info_t info;

  memset(&reg, 0, sizeof(reg)); /* zero the structure */

  lm_init(true);

  if (new_vbe_get_mode_info(mode, &info) != 0)
    return NULL;
  phys_bytes buf = info.PhysBasePtr;

  unsigned int bytes_per_pixel = ceil(info.BitsPerPixel / 8.0);

  X_Res = info.XResolution;
  Y_Res = info.YResolution;
  B_per_pixel = bytes_per_pixel;
  mem_model = info.MemoryModel;
  red_mask_size = info.RedMaskSize;
  green_mask_size = info.GreenMaskSize;
  blue_mask_size = info.BlueMaskSize;
  RedFieldPosition = info.RedFieldPosition;
  BlueFieldPosition = info.BlueFieldPosition;
  GreenFieldPosition = info.GreenFieldPosition;

  int r;
  struct minix_mem_range mr;                                  /* physical memory range */
  unsigned int vram_base = info.PhysBasePtr;                  /* VRAM’s physical addresss */
  unsigned int vram_size = bytes_per_pixel * (X_Res * Y_Res); /* VRAM’s size, but you can use the frame-buffer size, instead */
  /* frame-buffer VM address */
  /* Allow memory mapping */
  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  if (video_mem == NULL)
  {
    printf("Failed to map memory\n");
    return NULL;
  }

  reg.u.b.intno = BIOS_VIDEO;      //BIOS video services
  reg.u.w.ax = SET_VBE_MODE;       //Set video mode function
  reg.u.w.bx = LINEAR_MODE | mode; //video mode
  reg.u.w.es = PB2BASE(buf);       /* set a segment base */
  reg.u.w.di = PB2OFF(buf);        /* set the offset accordingly */
  reg.u.w.cx = mode;
  if (sys_int86(&reg) != OK)
  {
    printf("vg_exit(): sys_int86() failed \n");
    return NULL;
  }

  video_size = vram_size;
  video_buffer = (char *)malloc(vram_size);

  return video_mem;
}

///////////////////////////////////

////////RECTANGLE///////
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
  for (uint16_t i = 0; i < height; i++)
  {
    if (vg_draw_line(x, y, width, color) != 0)
      return 1;
    y++;
  }
  return 0;
}

///////////////////////////////////

int draw_pixel(uint16_t x, uint16_t y, uint32_t color)
{
  if (!(color & 0xff000000) || x < 0 || y < 0 || x > X_Res || y > V_RES)
    return 0; //Only draws fully opaque pixels
  char *ptr = (char *)video_buffer + (y * X_Res * B_per_pixel) + (x * B_per_pixel);
  memcpy(ptr, &color, B_per_pixel);
  return 0;
}

///////////////////////////////////

int vg_draw_line(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
  for (uint16_t i = 0; i < len; i++)
  {
    if (draw_pixel(x + i, y, color) != 0)
      return 1;
  }
  return 0;
}

///////////////////////////////////

void UpdateVideo()
{
  memcpy(video_mem, video_buffer, video_size);
}

///////////////////////////////////

void DrawBackground()
{
  memcpy(video_buffer, background->bitmapData, background->bitmapInfoHeader.imageSize);
}

void DrawBitmap(Bitmap *bmp, int x, int y)
{
  if (bmp == NULL)
    return;

  int width = bmp->bitmapInfoHeader.width;
  int height = bmp->bitmapInfoHeader.height;

  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (x + j < 0 || x + j > H_RES || y + i < 0 || y + i > V_RES) continue;
      //Checking if it's out of bounds ^^
      uint32_t color = bmp->bitmapData[i * width + j];
      draw_pixel(x + j, y + i, color);
    }
  }
}

////////////////SPRITE///////////////////

Sprite *CreateSprite(char img_name[])
{
  Bitmap *bmp;
  if ((bmp = loadBitmap(img_name)) == NULL)
    return NULL;
  Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));

  for (int i = 1; i < 360; i++)
  {
    Bitmap *rotated = RotateImage(bmp, i);
    sprite->bitmap[i] = rotated;
  }

  sprite->bitmap[0] = bmp; //for now

  return sprite;
}

///////////////////////////////////////

Bitmap *RotateImage(Bitmap *image, float angle)
{
  angle = angle * M_PI / 180.0;
  Bitmap *bmp = (Bitmap *)malloc(sizeof(Bitmap)); //New bitmap

  double cosAngle = cos(angle);
  double sinAngle = sin(angle);

  int width = image->bitmapInfoHeader.width;
  int height = image->bitmapInfoHeader.height;

  int x0 = image->bitmapInfoHeader.width/2;  //center x
  int y0 = image->bitmapInfoHeader.height/2;  //center y0
  
  unsigned int* bmp_map = (unsigned int*) malloc(width*height*4);
  unsigned int* image_map = (unsigned int*) image->bitmapData;

  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      int x1 = (x - x0) * cosAngle + (y0 - y) * sinAngle + x0;
      int y1 = (x - x0) * sinAngle + (y - y0) * cosAngle + y0;
      unsigned int color;

      if (x1 > 0 && x1 < image->bitmapInfoHeader.width && y1 > 0 && y1 < image->bitmapInfoHeader.height) { //within bounds
        color = *(unsigned int*)(image_map + y1 * width+ x1);
      }
      else
        color = 0x00FFFFFF;

      memcpy(bmp_map + y * width + x, &color, sizeof(color));
    }
  }

  bmp->bitmapData = bmp_map;
  bmp->bitmapInfoHeader.width = width;
  bmp->bitmapInfoHeader.height = height;
  bmp->bitmapInfoHeader.imageSize = image->bitmapInfoHeader.imageSize;

  return bmp;
}

///////////////////////////////////

void destroy_sprite(Sprite *sp)
{
  if (sp == NULL)
    return;
  free(sp);
  sp = NULL;
  // XXX: pointer is passed by value
  // should do this @ the caller
}

///////////////////////////////////

void DrawSprite(Sprite *img, int center_x, int center_y, unsigned int rot, bool centered)
{
  int x, y; //drawing position
  int width, height;

  width = img->bitmap[rot]->bitmapInfoHeader.width;
  height = img->bitmap[rot]->bitmapInfoHeader.height;

  if (centered)
  {
    x = center_x - img->bitmap[rot]->bitmapInfoHeader.width/2;
    y = center_y - img->bitmap[rot]->bitmapInfoHeader.height/2;
  }
  else
  {
    x = center_x;
    y = center_y;
  }

  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (x + j < 0 || x + j > H_RES || y + i < 0 || y + i > V_RES)
        continue; //Checking if it's out of bounds
      uint32_t color = img->bitmap[rot]->bitmapData[i * width + j];
      draw_pixel(x + j, y + i, color);
    }
  }
}

///////////////////////////////////

int new_vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p)
{

  //phys_bytes buf;
  struct reg86u r;
  memset(&r, 0, sizeof(r)); // zero the structure
  size_t size = sizeof(vbe_mode_info_t);
  mmap_t buf;

  lm_init(true);

  lm_alloc(size, &buf);

  r.u.w.ax = GET_VBE_MODE_INFO; // VBE get mode info
  // translate the buffer linear address to a far pointer
  r.u.w.es = PB2BASE(buf.phys); // set a segment base
  r.u.w.di = PB2OFF(buf.phys);  // set the offset accordingly
  r.u.w.cx = mode;
  r.u.b.intno = BIOS_VIDEO;

  if (sys_int86(&r) != OK)
  { // call BIOS
    printf("Error in sys_int86 \n");
    lm_free(&buf);
    return 1;
  }

  *vmi_p = *(vbe_mode_info_t *)buf.virt;
  lm_free(&buf);

  return 0;
}

///////////////////////////////////

int vbe_get_ctrl_info()
{

  char VBESign[4] = "VBE2";
  vg_vbe_contr_info_t info_p;
  aux_ctrl_info_t allocator;
  memset(&info_p, 0, sizeof(info_p));       // zero the structure
  memset(&allocator, 0, sizeof(allocator)); // zero the structure
  //get the vbe2
  struct reg86u r;
  memset(&r, 0, sizeof(r)); // zero the structure
  size_t size = sizeof(vg_vbe_contr_info_t);
  mmap_t buf;

  //uint32_t* base_memory =  lm_init(true);
  uint32_t *mem_base = lm_init(false);
  lm_alloc(size, &buf);

  memcpy(buf.virt, VBESign, sizeof(VBESign));

  r.u.w.ax = GET_VBE_CTRL_INFO; // VBE get mode info
  // translate the buffer linear address to a far pointer
  r.u.w.es = PB2BASE(buf.phys); // set a segment base
  r.u.w.di = PB2OFF(buf.phys);  // set the offset accordingly
  r.u.b.intno = BIOS_VIDEO;

  if (sys_int86(&r) != OK)
  { // call BIOS
    printf("Error in sys_int86 \n");
    lm_free(&buf);
    return 1;
  }

  memcpy(&allocator, buf.virt, buf.size);
  lm_free(&buf);

  //error is that i must covert farpointers into virtual adress, how?
  strcpy(info_p.VBESignature, allocator.VBESignature);
  info_p.VBEVersion[0] = allocator.VBEVersion[0];
  info_p.VBEVersion[1] = allocator.VBEVersion[1];
  info_p.OEMString = (void *)((uint32_t)mem_base + SEGMENT(allocator.OEMString) * 0x10 + (OFFSET(allocator.OEMString)));
  info_p.VideoModeList = (void *)((uint32_t)mem_base + SEGMENT(allocator.VideoModeList) * 0x10 + (OFFSET(allocator.VideoModeList)));
  info_p.TotalMemory = allocator.TotalMemory * 64;
  info_p.OEMVendorNamePtr = (void *)((uint32_t)mem_base + SEGMENT(allocator.OEMVendorNamePtr) * 0x10 + (OFFSET(allocator.OEMVendorNamePtr)));
  info_p.OEMProductNamePtr = (void *)((uint32_t)mem_base + SEGMENT(allocator.OEMProductNamePtr) * 0x10 + (OFFSET(allocator.OEMProductNamePtr)));
  info_p.OEMProductRevPtr = (void *)((uint32_t)mem_base + SEGMENT(allocator.OEMProductRevPtr) * 0x10 + (OFFSET(allocator.OEMProductRevPtr)));

  vg_display_vbe_contr_info(&info_p);

  return 0;
}
