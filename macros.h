#ifndef _MACROS_H_
#define _MACROS_H_

#include <lcom/lcf.h>

#define BIOS_VIDEO 0x10
#define VIDEO_MODE_FUNC 0x00
#define SET_VBE_MODE 0x4F02
#define GET_VBE_MODE_INFO 0x4F01
#define GET_VBE_CTRL_INFO 0x4F00
#define TEXT_MODE 0x03
#define BIT(n) (0x01<<(n))
#define LINEAR_MODE BIT(14)
#define XPE_MODE 0x105
#define SEGMENT(n)((n) >> 16 & 0x0FFFF)
#define OFFSET(n) ((n) & 0x0FFFF)

#endif
