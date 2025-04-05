#pragma once

#include QMK_KEYBOARD_H
#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"


enum my_keycodes {
    KC_JUZZ_PREV = QK_KB_0,
    KC_JUZZ_NEXT,
    RGB_J1,
    RGB_J2,
    RGB_J3
};

typedef struct effect_step_t {
    uint8_t  es; // Effect Start        (Range 0-255)
    uint8_t  ee; // Effect End          (Range 0-255)
    uint8_t  hs; // Hue Start           (Range 0-255)
    uint8_t  he; // Hue End             (Range 0-255)
    uint8_t  ss; // Saturation Start    (Range 0-255)
    uint8_t  se; // Saturation End      (Range 0-255)
    uint8_t  bs; // Brightness Start    (Range 0-255)
    uint8_t  be; // Brightness End      (Range 0-255)
    bool     ih; // Interpolation HSV/RGB
    uint8_t  ex; // Set to signal end of the effect when == `1`
} effect_step_t;

typedef effect_step_t effect_t[];

void render_effect(uint8_t led_min, uint8_t led_max, effect_params_t* params);
