#pragma once

#include QMK_KEYBOARD_H
#include "print.h"
#include "quantum.h"

#ifdef MAX
#	undef MAX
#endif
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef MIN
#	undef MIN
#endif
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifdef CLAMP
#	undef CLAMP
#endif
#define CLAMP(a, x, y) ((a) < (x) ? (x) : ((a) > (y) ? (y) : (a)))

enum my_keycodes {
	RGB_BRI_I = SAFE_RANGE,
	RGB_BRI_D,
	RGB_J1,
	RGB_J2,
	RGB_J3,
	ALT_TAB
};

typedef struct HSB {
    float h;
    float s;
    float b;
} HSB;

typedef struct effect_config {
	float	es; // Effect Start
	float	ee; // Effect End
	float	hs; // Hue Start
	float	he; // Hue End
	float	ss; // Saturation Start
	float	se; // Saturation End
	float	bs; // Brightness Start
	float	be; // Brightness End
	bool	ih;	// Interpolation HSV/RGB
	uint8_t	ex; // Set to signal end of the setup when == `1`
} effect_config;

typedef struct animation_config {
	float 			effect_time;
	float			effects_per_board;
	effect_config	effects[];

} animation_config;

// Functions
float func_interpolate(float x, float y, float t);
HSB func_interpolate_hsb(HSB bgn, HSB end, float t);
RGB func_interpolate_rgb(RGB bgn, RGB end, float t);
RGB hsb_to_rgb(HSB hsb);
RGB get_rgb_for_effect_and_time(effect_config *effect, float time);
RGB get_rgb_scaled_to_brightness(RGB rgb, float brightness);
