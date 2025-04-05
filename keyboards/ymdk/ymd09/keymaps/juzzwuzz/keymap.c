#include "juzzwuzz.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        MO(1),          KC_MUTE,        KC_MPLY,
        KC_NO,          KC_VOLD,        KC_VOLU,
        KC_NO,          KC_NO,          KC_F13
    ),
    [1] = LAYOUT(
        KC_TRNS,        RM_VALD,        RM_VALU,
        RGB_J3,        KC_JUZZ_PREV,   KC_JUZZ_NEXT,
        QK_BOOT,        RGB_J1,         RGB_J2
    )
};
