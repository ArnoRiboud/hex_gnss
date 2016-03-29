#include "profiLED.h"
#include <common/timing.h>
#include "spi.h"

static struct profiLED_gen_color_s led_colors[NUM_profiLEDS];

static void led_spi_send_byte(uint8_t byte) {
    while (!(SPI_SR(SPI3) & SPI_SR_TXE)); // wait for bus to become available
    SPI_DR8(SPI3) = byte;
}

void profiLED_set_color_rgb(uint32_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (idx >= NUM_profiLEDS) {
        return;
    }

    profiLED_gen_make_brg_color_rgb(r,g,b,&led_colors[idx]);
}

void profiLED_set_color_all_rgb(uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t i=0; i<NUM_profiLEDS; i++) {
        profiLED_set_color_rgb(i,r,g,b);
    }
}

void profiLED_set_color_all_hex(uint32_t color_hex) {
    profiLED_set_color_all_rgb((uint8_t)(color_hex>>16), (uint8_t)(color_hex>>8), (uint8_t)color_hex);
}

void profiLED_set_color_hex(uint32_t idx, uint32_t color_hex) {
    profiLED_set_color_rgb(idx, (uint8_t)(color_hex>>16), (uint8_t)(color_hex>>8), (uint8_t)color_hex);
}

void profiLED_update(void) {
    static uint32_t last_update_us;
    uint32_t tnow_us = micros();
    if (tnow_us-last_update_us < 8333) {
        return;
    }
    last_update_us = tnow_us;

    spi_begin(SPI_DEVICE_UBLED);
    for(uint8_t i=0;i<30;i++) __asm__("nop");
    profiLED_gen_write(NUM_profiLEDS, led_colors, led_spi_send_byte);
    led_spi_send_byte(0); // seems to be required for last LED to set color
    for(uint8_t i=0;i<30;i++) __asm__("nop");

    spi_end();
}
