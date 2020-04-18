#include "common.h"

// VLC Hotkeys
#define VLC_PLY MEH(KC_F1)
#define VLC_NXT MEH(KC_F2)
#define VLC_PRV MEH(KC_F3)
#define VLC_STP MEH(KC_F4)
#define VLC_SBJ MEH(KC_F5)
#define VLC_SFJ MEH(KC_F6)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(
		MO(1),		KC_MUTE,	KC_MPLY,
		MO(2),		KC_VOLD,	KC_VOLU,
		MO(3),		ALT_TAB,	KC_F13
	),
	[1] = LAYOUT(
		KC_TRNS,	RGB_BRI_D,	RGB_BRI_I,
		RGB_J1,		RGB_J2,		RGB_TOG,
		RESET,		RGB_SPD,	RGB_SPI
	),
	[2] = LAYOUT(
		KC_NO,		KC_NO,		KC_NO,
		KC_TRNS,	KC_NO,		KC_NO,
		KC_NO,		KC_NO,		KC_NO
	),
	[3] = LAYOUT(
		VLC_PRV,	VLC_PLY,	VLC_NXT,
		VLC_SBJ,	VLC_STP,	VLC_SFJ,
		KC_TRNS,	KC_NO,		KC_NO
	)
};
