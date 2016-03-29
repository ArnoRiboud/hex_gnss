#pragma once

#include <common/profiLED_gen.h>
#include <stdint.h>

#ifndef NUM_profiLEDS
#define NUM_profiLEDS 4
#endif


void profiLED_set_color_all_rgb(uint8_t r, uint8_t g, uint8_t b);
void profiLED_set_color_rgb(uint32_t idx, uint8_t r, uint8_t g, uint8_t b);
void profiLED_set_color_all_hex(uint32_t color_hex);
void profiLED_set_color_hex(uint32_t idx, uint32_t color_hex);

void profiLED_update(void);
