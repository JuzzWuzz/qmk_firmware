#pragma once

#include QMK_KEYBOARD_H
#include <math.h>
#include "print.h"
#include <stdlib.h>
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

#define EPSILON 0.00001f

enum my_layer_names {
	LYR_BASE,
    LYR_RGB,
    LYR_VLC,
    LYR_MAP,
    LYR_OBS,
    LYR_FREE
};

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
	bool	iv; // Invert direction of Hue Interpolation
	uint8_t	ex; // Set to signal end of the setup when == `1`
} effect_config;

typedef struct animation_config {
	float			effect_time;		// Time to complete the animation
	float			effects_per_board;	// Number of times full animation shown
	float			effect_angle;		// The angle to offset the animation
	effect_config	effects[];			// Array of effects

} animation_config;

// Functions
bool compare_float_eq(float f1, float f2);
int wrap_int(int val, int const min, int const max);
float wrap_float(float val, float const min, float const max);
HSB func_interpolate_hsb(HSB bgn, HSB end, float t, bool invert);
RGB func_interpolate_rgb(RGB bgn, RGB end, float t);
RGB hsb_to_rgb(HSB hsb);
RGB get_rgb_for_effect_and_time(effect_config *effect, float time);
RGB get_rgb_scaled_to_brightness(RGB rgb, float brightness);
