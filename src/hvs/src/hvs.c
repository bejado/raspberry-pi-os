#include "hvs.h"
#include "peripherals/hvs.h"

#include "utils.h"

#include <stdint.h>

static volatile uint32_t* dlist_memory = (uint32_t*) 0x3F402000;

/* We'll use a simple "double buffering" scheme to avoid writing out a new display list while
   one is still in-flight. */
static const uint16_t dlist_buffer_count = 2;
static const uint16_t dlist_offsets[] = { 0, 128 };
static uint16_t next_dlist_buffer = 0;

#define WRITE_WORD(word) (dlist_memory[(*offset)++] = word)

static void write_plane(uint16_t* offset, hvs_plane plane)
{
    /* Write out the words for this plane. Each word conveys some information to the HVS on how it
       should interpret this plane. */

    /* Control Word */
    const uint8_t number_of_words = 7;
    uint32_t control_word = SCALER_CTL0_VALID              |        // denotes the start of a plane
                            SCALER_CTL0_UNITY              |        // indicates no scaling
                            plane.pixel_order       << 13  |        // pixel order
                            number_of_words         << 24  |        // number of words in this plane
                            plane.format;                           // pixel format
    WRITE_WORD(control_word);

    /* Position Word 0 */
    uint32_t position_word_0 = plane.start_x        << 0   |
                               plane.start_y        << 12;
    WRITE_WORD(position_word_0);

    /* Position Word 1: scaling, only if non-unity */

    /* Position Word 2 */
    uint32_t position_word_2 = plane.width         << 0    |
                               plane.height        << 16;
    WRITE_WORD(position_word_2);

    /* Position Word 3: used by HVS */
    WRITE_WORD(0xDEADBEEF);

    /* Pointer Word */
    /* This cast is okay, because the framebuffer pointer can always be held in 4 bytes
       even though we're on a 64 bit architecture. */
    uint32_t framebuffer = (uint32_t) (intptr_t) plane.framebuffer;
    WRITE_WORD(0x80000000 | framebuffer);

    /* Pointer Context: used by HVS */
    WRITE_WORD(0xDEADBEEF);

    /* Pitch Word */
    uint32_t pitch_word = plane.pitch;
    WRITE_WORD(pitch_word);
}

void write_display_list(hvs_plane planes[], uint8_t count)
{
    uint16_t offset = dlist_offsets[next_dlist_buffer];
    const uint16_t start = offset;

    /* Write out each plane. */
    for (uint8_t p = 0; p < count; p++) {
        write_plane(&offset, planes[p]);
    }

    /* End Word */
    dlist_memory[offset] = SCALER_CTL0_END;

    /* Tell the HVS where the display list is by writing to the SCALER_DISPLIST1 register. */
    put32(SCALER_DISPLIST1, start);

    next_dlist_buffer = (next_dlist_buffer + 1) % dlist_buffer_count;
}

