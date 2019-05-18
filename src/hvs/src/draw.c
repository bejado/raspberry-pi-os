#include "draw.h"

static void scanline(hvs_plane plane, uint16_t color, int y, int left, int right)
{
    uint16_t* pixels = (uint16_t*) plane.framebuffer;
    for (int c = left; c <= right; c++) {
        if (c < 0 || c >= plane.width) continue;
        pixels[y * plane.width + c] = color;
    }
}

static void mirrored_scanlines(hvs_plane plane, uint16_t color, int xc, int yc, int x, int y)
{
    scanline(plane, color, yc + y, xc - x, xc + x);
    scanline(plane, color, yc - y, xc - x, xc + x);
}

// An implementation of Bresenham's circle drawing algorithm that draws each scanline once.
void draw_circle_16(hvs_plane plane, uint16_t color, int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    scanline(plane, color, yc + x, xc - y, xc + y);

    while (y >= x) {
        x++;

        if (d > 0) {
            mirrored_scanlines(plane, color, xc, yc, x - 1, y);
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        if (y > x) {
            mirrored_scanlines(plane, color, xc, yc, y, x);
        }
    }
}

void draw_rectangle_16(hvs_plane plane, uint16_t color, rect r)
{
    if (r.top > r.bottom || r.left > r.right) {
        return;
    }
    uint16_t* pixels = (uint16_t*) plane.framebuffer;
    for (uint16_t row = r.top; row < r.bottom; row++) {
        for (uint16_t col = r.left; col < r.right; col++) {
            pixels[row * plane.width + col] = color;
        }
    }
}

void draw_rectangle_32(hvs_plane plane, uint32_t color, rect r)
{
    if (r.top > r.bottom || r.left > r.right) {
        return;
    }
    uint32_t* pixels = (uint32_t*) plane.framebuffer;
    for (uint16_t row = r.top; row < r.bottom; row++) {
        for (uint16_t col = r.left; col < r.right; col++) {
            pixels[row * plane.width + col] = color;
        }
    }
}

void clear_plane_16(hvs_plane plane, uint16_t color)
{
    uint16_t* pixels = (uint16_t*) plane.framebuffer;
    for (int i = 0; i < plane.width * plane.height; ++i) {
        pixels[i] = color;
    }
}

void clear_plane_32(hvs_plane plane, uint32_t color)
{
    uint32_t* pixels = (uint32_t*) plane.framebuffer;
    for (int i = 0; i < plane.width * plane.height; ++i) {
        pixels[i] = color;
    }
}
