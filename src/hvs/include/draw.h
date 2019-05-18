#ifndef	_DRAW_H
#define	_DRAW_H

#include "hvs.h"

#include <stdint.h>

/* Color defines for RGB565 */
#define RED_16          0xF800
#define GREEN_16        0x07E0
#define BLUE_16         0x001F
#define YELLOW_16       0xFFC0
#define WHITE_16        0xFFFF
#define BLACK_16        0x0000

/* Color defines for RGBA8888 */
/* Even though the pixel order is ARGB, we must use ABGR here ¯\_(ツ)_/¯ */
#define RED_32         0xFF0000FF
#define GREEN_32       0xFF00FF00
#define BLUE_32        0xFFFF0000
#define YELLOW_32      0xFF00FFFF
#define WHITE_32       0xFFFFFFFF
#define BLACK_32       0xFF000000
#define TRANSPARENT_32 0x00000000

// An implementation of Bresenham's circle drawing algorithm that draws each scanline once.
void draw_circle_16(hvs_plane plane, uint16_t color, int xc, int yc, int r);

typedef struct {
    uint16_t left, top, right, bottom;
} rect;

void draw_rectangle_16(hvs_plane plane, uint16_t color, rect r);
void draw_rectangle_32(hvs_plane plane, uint32_t color, rect r);

void clear_plane_16(hvs_plane plane, uint16_t color);
void clear_plane_32(hvs_plane plane, uint32_t color);

#endif
