#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"


// ----------------------------------------
// Macros

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))
#define DECREMENT_WRAP(val, max) ((val) == 0 ? (max) - 1 : (val) - 1)
#define INCREMENT_WRAP(val, max) (((val) + 1) % (max))


// ----------------------------------------
// Enums

enum effect_angles {
    EFFECT_DIRECTION_N = 0,
    EFFECT_DIRECTION_NE,
    EFFECT_DIRECTION_E,
    EFFECT_DIRECTION_SE,
    EFFECT_DIRECTION_S,
    EFFECT_DIRECTION_SW,
    EFFECT_DIRECTION_W,
    EFFECT_DIRECTION_NW,
    EFFECT_ANGLES_COUNT
};

enum my_keycodes {
    J_EFF_P = QK_KB_0,
    J_EFF_N,
    J_ROT_D,
    J_ROT_I,
    J_RESET,
    RGB_J1,
    RGB_J2,
    RGB_J3
};


// ----------------------------------------
// Types
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


// ----------------------------------------
// Externs

extern effect_step_t *g_layer_effect;
extern effect_t **g_effects;
extern uint8_t g_effect_count;


// ----------------------------------------
// Functions

void render_effect(uint8_t led_min, uint8_t led_max, effect_params_t* params);
void set_layer_color(void);
// Allows for keyboard specific keycode handling
__attribute__((weak)) bool process_keycode_juzz(uint16_t keycode);
