#ifndef	_HVS_H
#define	_HVS_H

#include <stdint.h>

typedef enum {
	/* 8bpp */
	HVS_PIXEL_FORMAT_RGB332 = 0,

	/* 16bpp */
	HVS_PIXEL_FORMAT_RGBA4444 = 1,
	HVS_PIXEL_FORMAT_RGB555 = 2,
	HVS_PIXEL_FORMAT_RGBA5551 = 3,
	HVS_PIXEL_FORMAT_RGB565 = 4,

	/* 24bpp */
	HVS_PIXEL_FORMAT_RGB888 = 5,
	HVS_PIXEL_FORMAT_RGBA6666 = 6,

	/* 32bpp */
	HVS_PIXEL_FORMAT_RGBA8888 = 7,
} hvs_pixel_format;

// todo: maybe just assume this?
typedef enum {
    HVS_PIXEL_ORDER_RGBA = 0,
    HVS_PIXEL_ORDER_BGRA = 1,
    HVS_PIXEL_ORDER_ARGB = 2,
    HVS_PIXEL_ORDER_ABGR = 3
} hvs_pixel_order;

typedef struct {
    hvs_pixel_format format;            // format of the pixels in the plane
    hvs_pixel_order pixel_order;        // order of the components in each pixel
    uint16_t start_x;                   // x position of the left of the plane
    uint16_t start_y;                   // y position of the top of the plane
    uint16_t height;                    // height of the plane, in pixels
    uint16_t width;                     // width of the plane, in pixels
    uint16_t pitch;                     // number of bytes between the start of each scanline
    void* framebuffer;                  // pointer to the pixels in memory
} hvs_plane;

void write_display_list(hvs_plane planes[], uint8_t count);

#endif  /*_HVS_H */
