#include QMK_KEYBOARD_H
#include "juzzwuzz.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        MO(1),          KC_MUTE,        KC_MPLY,
        MO(2),          KC_VOLD,        KC_VOLU,
        XXXXXXX,        XXXXXXX,        KC_F13
    ),
    [1] = LAYOUT(
        _______,        RM_VALD,        RM_VALU,
        RM_TOGG,        J_EFF_P,        J_EFF_N,
        J_RESET,        J_ROT_D,        J_ROT_I
    ),
    [2] = LAYOUT(
        XXXXXXX,        XXXXXXX,        XXXXXXX,
        _______,        XXXXXXX,        XXXXXXX,
        QK_BOOT,        XXXXXXX,        XXXXXXX
    )
};
