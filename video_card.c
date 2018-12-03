#include <lcom/lcf.h>
#include "video_card.h"
#include "macros.h"
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

uint16_t getXRes(){
  return X_Res;
}

uint16_t getYRes(){
  return Y_Res;
}

uint8_t getByts_per_pixel(){
  return B_per_pixel;
}

uint8_t getMemoryModel(){
  return mem_model;
}

uint8_t getRedMaskSize(){
  return red_mask_size;
}

uint8_t getGreenMaskSize(){
  return green_mask_size;
}

uint8_t getBlueMaskSize(){
  return blue_mask_size;
}

uint8_t getRedFieldPosition(){
  return RedFieldPosition;
}

uint8_t getGreenFieldPosition(){
  return GreenFieldPosition;
}

uint8_t getBlueFieldPosition(){
  return BlueFieldPosition;
}

uint32_t direct_color(uint16_t row, uint16_t col, uint32_t first, uint8_t step){
  uint32_t Rmask = ((1 << getRedMaskSize()) - 1) << getRedFieldPosition();
  unsigned Rvalue = (Rmask & first) >> getRedFieldPosition();
    uint32_t R = (Rvalue + col * step) % (1 << getRedMaskSize());
    R = R << getRedFieldPosition();
  uint32_t Gmask = ((1 << getGreenMaskSize()) - 1) << getGreenFieldPosition();
  unsigned Gvalue = (Gmask & first) >> getGreenFieldPosition();
	  uint32_t G = (Gvalue + row * step) % (1 << getGreenMaskSize());
    G = G << getGreenFieldPosition();
  uint32_t Bmask = ((1 << getBlueMaskSize()) - 1) << getBlueFieldPosition();
  unsigned Bvalue = (Bmask & first) >> getBlueFieldPosition();
	  uint32_t B = (Bvalue + (col + row) * step) % (1 << getBlueMaskSize());
    B = B << getBlueFieldPosition();

  uint32_t color = R | G | B;
  return color;
}

void* (vg_init)(uint16_t mode){
  struct reg86u reg;

  vbe_mode_info_t info;

  memset(&reg, 0, sizeof(reg));	/* zero the structure */

  lm_init(true);

  if(new_vbe_get_mode_info(mode, &info) != 0) return NULL;
  phys_bytes buf = info.PhysBasePtr;

  unsigned int bytes_per_pixel = ceil(info.BitsPerPixel/8.0);

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
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_base = info.PhysBasePtr;  /* VRAM’s physical addresss */
  unsigned int vram_size = bytes_per_pixel*(X_Res*Y_Res);  /* VRAM’s size, but you can use the frame-buffer size, instead */
    /* frame-buffer VM address */
    /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) 
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED) panic("couldn’t map video memory");

  if(video_mem == NULL) {
		printf("Failed to map memory\n");
		return NULL;
	}

  reg.u.b.intno = BIOS_VIDEO;  //BIOS video services
  reg.u.w.ax = SET_VBE_MODE;  //Set video mode function
  reg.u.w.bx = LINEAR_MODE|mode;  //video mode
  reg.u.w.es = PB2BASE(buf);    /* set a segment base */
  reg.u.w.di = PB2OFF(buf);     /* set the offset accordingly */
  reg.u.w.cx = mode;
  if( sys_int86(&reg) != OK ) {
    printf("vg_exit(): sys_int86() failed \n");
    return NULL;
  }

  video_size = vram_size;
  video_buffer = (char *) malloc(vram_size);

  return video_mem; 
}

////////RECTANGLE///////
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(uint16_t i=0; i<height; i++){
    if(vg_draw_line(x, y, width, color) != 0) return 1;
    y++;
  }
  return 0;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color){
  if(color == PINK) return 0;
  char *ptr = (char*)video_buffer +(y * X_Res * B_per_pixel) +(x *B_per_pixel);
  //printf("\nPIXEL COLOR: %x",color);
  memcpy(ptr,&color,B_per_pixel); 
  return 0;
}

int vg_draw_line( uint16_t x, uint16_t y,uint16_t len,uint32_t color ){
  for( uint16_t i=0; i< len ;i++){
    if(draw_pixel(x+i,y,color) != 0) return 1;
  }
  return 0; 
}

void UpdateVideo(){
  memcpy(video_mem,video_buffer,video_size); 
}

int draw_xpm(xpm_image_t *image, uint8_t* map, uint16_t x, uint16_t y){

  int width, height;
  /*char *map;
  // get the pix map from the XPM
  map = read_xpm(xpm, &width, &height);*/
  // copy it to graphics memory

  //uint8_t* map = &image->map;
  height = image->height;
  width = image->width;

  for( int i=0; i<height; i++ ){
    for(int j=0; j<width; j++){
      uint32_t color = map[i * width + j];
     if(draw_pixel(x+j,y+i,color) != 0) return 1; 
    }
  }
  return 0; 
}


/** Creates a new sprite with pixmap "pic", with specified
position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(const char *pic[], int x, int y, int xf, int yf, int xspeed, int yspeed) {
  //allocate space for the "object"
  Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
  if( sp == NULL ) return NULL;
  
  // read the sprite pixmap
  sp->map = read_xpm(pic, &(sp->width), &(sp->height));
  if( sp->map == NULL ) {
    free(sp);
    return NULL;
  }

  if(xf-x > 0) sp->x_dir = true;
  else sp->x_dir = false;
  if(yf-y > 0) sp->y_dir = true;
  else sp->y_dir = false;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  sp->moving = ((x != xf || y != yf) && (xspeed != 0 || yspeed != 0));
  sp->x = x; sp->y = y;
  sp->xf = xf; sp->yf = yf;
  return sp; 
}

void destroy_sprite(Sprite *sp) {
  if( sp == NULL ) return;
  if( sp ->map ) free(sp->map);
  free(sp);
  sp = NULL;    
  // XXX: pointer is passed by value
  // should do this @ the caller
}

int draw_sprite(Sprite *sp) {
  if(sp->moving){
    draw_rectangle(sp->x, sp->y, sp->width, sp->height,0);
    sp->x = sp->x + sp->xspeed;
    sp->y = sp->y + sp->yspeed;
    if(sp->x > sp->xf && sp->x_dir) sp->x = sp->xf;
    else if(sp->x < sp->xf && !sp->x_dir) sp->x = sp->xf;
    if(sp->y > sp-> yf && sp->y_dir) sp->y = sp->yf;
    else if(sp->y < sp->yf && !sp->y_dir) sp->y = sp->yf;
  }
  for( int i=0; i<sp->height; i++ ){
    for(int j=0; j<sp->width; j++){
      uint32_t color = sp->map[i * sp->width + j];
     if(draw_pixel(sp->x+j,sp->y+i,color) != 0) return 1; 
    }
  }
  sp->moving = (sp->x != sp->xf || sp->y != sp->yf);
  return 0;
}

int new_vbe_get_mode_info(uint16_t mode,vbe_mode_info_t * vmi_p ){

  //phys_bytes buf;
  struct reg86u r;
  memset(&r, 0, sizeof(r));	// zero the structure
  size_t size = sizeof(vbe_mode_info_t); 
  mmap_t buf; 

  lm_init(true); 

  lm_alloc(size, &buf); 

  r.u.w.ax =  GET_VBE_MODE_INFO; // VBE get mode info 
  // translate the buffer linear address to a far pointer 
  r.u.w.es = PB2BASE(buf.phys); // set a segment base 
  r.u.w.di = PB2OFF(buf.phys); // set the offset accordingly 
  r.u.w.cx = mode;
  r.u.b.intno = BIOS_VIDEO;
 
  if(sys_int86(&r) != OK ) { // call BIOS 
    printf("Error in sys_int86 \n");
    lm_free(&buf); 
    return 1; 
  }

  *vmi_p =*(vbe_mode_info_t *)buf.virt; 
  lm_free(&buf);

  return 0; 
}


int vbe_get_ctrl_info(){
  
  char VBESign[4]= "VBE2"; 
  vg_vbe_contr_info_t info_p;
  aux_ctrl_info_t allocator;
  memset(&info_p, 0, sizeof(info_p));	// zero the structure
  memset(&allocator, 0, sizeof(allocator));	// zero the structure
  //get the vbe2
  struct reg86u r;
  memset(&r, 0, sizeof(r));	// zero the structure  
  size_t size = sizeof(vg_vbe_contr_info_t); 
  mmap_t buf; 

  //uint32_t* base_memory =  lm_init(true); 
  uint32_t* mem_base = lm_init(false);
  lm_alloc(size, &buf); 

  memcpy(buf.virt,VBESign,sizeof(VBESign));

  r.u.w.ax =  GET_VBE_CTRL_INFO; // VBE get mode info 
  // translate the buffer linear address to a far pointer 
  r.u.w.es = PB2BASE(buf.phys); // set a segment base 
  r.u.w.di = PB2OFF(buf.phys); // set the offset accordingly 
  r.u.b.intno = BIOS_VIDEO;
 
  if(sys_int86(&r) != OK ) { // call BIOS 
    printf("Error in sys_int86 \n");
    lm_free(&buf); 
    return 1; 
  }

  memcpy(&allocator,buf.virt,buf.size);
  lm_free(&buf);

  //error is that i must covert farpointers into virtual adress, how? 
  strcpy(info_p.VBESignature, allocator.VBESignature); 
  info_p.VBEVersion[0] = allocator.VBEVersion[0]; 
  info_p.VBEVersion[1] = allocator.VBEVersion[1]; 
  info_p.OEMString = (void*)((uint32_t) mem_base + SEGMENT(allocator.OEMString)*0x10+ (OFFSET(allocator.OEMString))); 
  info_p.VideoModeList = (void*)((uint32_t) mem_base + SEGMENT(allocator.VideoModeList) *0x10+ (OFFSET(allocator.VideoModeList)));  
  info_p.TotalMemory = allocator.TotalMemory *64; 
  info_p.OEMVendorNamePtr= (void*)((uint32_t) mem_base + SEGMENT(allocator.OEMVendorNamePtr) *0x10+ (OFFSET(allocator.OEMVendorNamePtr)));  
  info_p.OEMProductNamePtr= (void*)((uint32_t) mem_base + SEGMENT(allocator.OEMProductNamePtr) *0x10 + (OFFSET(allocator.OEMProductNamePtr)));  
  info_p.OEMProductRevPtr = (void*)((uint32_t) mem_base + SEGMENT(allocator.OEMProductRevPtr) *0x10 + (OFFSET(allocator.OEMProductRevPtr)));  
  
  vg_display_vbe_contr_info(&info_p);

  return 0; 
}

