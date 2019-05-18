#include "debug.h"
#include "draw.h"
#include "hvs.h"
#include "mini_uart.h"
#include "mm.h"
#include "printf.h"
#include "utils.h"

#include <stdint.h>

void kernel_main(void)
{
	uart_init();
    init_printf(0, putc);

    debug_print_registers();

    /* "Allocate" 4 framebuffers in memory. Each is 1MiB in size, which is plenty for our purposes. */
    uint16_t* const fb_one     = (uint16_t*)(0x10000000);   // the first 3 will use 16-bit pixels.
    uint16_t* const fb_two     = (uint16_t*)(0x10100000);
    uint16_t* const fb_three   = (uint16_t*)(0x10200000);
    uint32_t* const fb_four    = (uint32_t*)(0x10300000);   // this one will use a 32-bit pixel format.

    /* Each framebuffer will be a quarter of the screen (960 x 540) in dimensions. */
    const uint16_t screen_width = 1920, screen_height = 1080;
    const uint16_t fb_width = screen_width / 2, fb_height = screen_height / 2;
    const uint16_t fb_center_x = fb_width / 2;
    const uint16_t fb_center_y = fb_height / 2;

    /* Set up initial display list - a single plane centered on the screen. */
    hvs_plane plane = {
        .format = HVS_PIXEL_FORMAT_RGB565,
        .pixel_order = HVS_PIXEL_ORDER_ARGB,
        .start_x = (screen_width - fb_width) / 2,
        .start_y = (screen_height - fb_height) / 2,
        .height = fb_height,
        .width = fb_width,
        .pitch = fb_width * sizeof(uint16_t),
        .framebuffer = fb_one
    };
    write_display_list(&plane, 1);

    /* Pause for a few seconds... */
    delay(9000000);

    /* We'll use this rect to draw rectangles. */
    rect r  = {
        .left = fb_center_x - 100,
        .top = fb_center_y - 100,
        .right = fb_center_x + 100,
        .bottom = fb_center_y + 100
    };

    /* Clear the framebuffer and do some drawing... */
    clear_plane_16(plane, GREEN_16);
    draw_rectangle_16(plane, WHITE_16, r);

    /* Pause for a few seconds... */
    delay(9000000);

    /* Move the plane across the screen. */
    for (int i = 0; i < 10; i++) {
        plane.start_x = i * ((screen_width - fb_width) / 10);
        plane.start_y = i * ((screen_height - fb_height) / 10);
        write_display_list(&plane, 1);
        delay(9000000 * 0.25);
    }

    /* Set up 4 planes. */
    hvs_plane planes[4];
    int i = 0;
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            planes[i].format = HVS_PIXEL_FORMAT_RGB565,
            planes[i].pixel_order = HVS_PIXEL_ORDER_ARGB,
            planes[i].start_x = fb_width * x;
            planes[i].start_y = fb_height * y;
            planes[i].height = fb_height,
            planes[i].width = fb_width,
            planes[i].pitch = fb_width * 2,
            i++;
        }
    }
    planes[0].framebuffer = fb_one;
    planes[1].framebuffer = fb_two;
    planes[2].framebuffer = fb_three;
    planes[3].framebuffer = fb_four;

    /* We'll make the fourth framebuffer a 32-bit pixel format, just for demonstrations. */
    planes[3].format = HVS_PIXEL_FORMAT_RGBA8888;
    planes[3].pitch = fb_width * sizeof(uint32_t);

    write_display_list(planes, 4);

    /* Clear the 3 new framebuffers. */

    clear_plane_16(planes[1], BLUE_16);
    clear_plane_16(planes[2], WHITE_16);
    clear_plane_32(planes[3], RED_32);

    /* Draw some additional shapes. */
    draw_rectangle_16(planes[0], WHITE_16, r);
    draw_circle_16(planes[1], WHITE_16, fb_center_x, fb_center_y, 100);
    draw_circle_16(planes[2], BLACK_16, fb_center_x, fb_center_y, 100);
    draw_rectangle_32(planes[3], YELLOW_32, r);

    delay(9000000);

    /* Move the fourth plane to the center of the screen. */
    planes[3].start_x = (screen_width - fb_width) / 2;
    planes[3].start_y = (screen_height - fb_height) / 2;

    write_display_list(planes, 4);

    delay(9000000);

    /* Make the fourth plane partially-transparent with a yellow square. */
    uint32_t half_trasparent_red = 0x7F0000FF;
    clear_plane_32(planes[3], half_trasparent_red);
    draw_rectangle_32(planes[3], YELLOW_32, r);

    delay(9000000);

    int first_plane = 0;
    while (1) {
        /* Rotate the planes every so often... */
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                int plane_index = (y * 2 + x + first_plane) % 4;
                planes[plane_index].start_x = fb_width * x;
                planes[plane_index].start_y = fb_height * y;
            }
        }

        write_display_list(planes, 4);
        delay(9000000 * 1);

        first_plane = (first_plane + 1) % 4;
    }
}
