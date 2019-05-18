#include "debug.h"

#include "utils.h"
#include "printf.h"

#include "peripherals/hvs.h"

#include <stdint.h>

#define PRINT_REGISTER(name) \
    (printf("%s: 0x%08X\r\n", #name, get32(name)))

void debug_print_registers() {
    PRINT_REGISTER(SCALER_DISPLIST1);
    printf("\r\n");
}

void debug_print_memory(void* start, unsigned int size_bytes) {
    uint32_t* p = (uint32_t*) start;
    while (p < (uint32_t*) start + size_bytes / sizeof(uint32_t)) {
        printf("%08X %08X %08X %08X     0x%08X\r\n", *(p + 3), *(p + 2), *(p + 1), *p, p);
        p += 4;
    }
}
