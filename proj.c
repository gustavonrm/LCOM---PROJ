// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
// Any header files included below this line should have been created by you

#include "bitmap.h"
#include "video_card.h"
#include "keyboard.h"

Bitmap* background;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)() {
  vg_init(0x118);

  if((background = loadBitmap("Background.bmp")) == NULL){
    vg_exit();
    return 1;
  }
  drawBitmap(background,0,0);
  UpdateVideo();

  sleep(3);

  vg_exit(); 
  return 0;
}


//argc numero de argumentos entre ""
//argv array cujos elementos sao os argumentos transatos 
