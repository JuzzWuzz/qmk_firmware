#include "juzzwuzz.h"


// ----------------------------------------
// Keyboard Functions - Layer Control
// ----------------------------------------

effect_step_t layer_rgb[] = {
    { .es = 0,   .ee = 84,  .hs = 0,   .he = 0,   .ss = 255, .se = 255, .bs = 128, .be = 255, .ih =  true }, // Red
    { .es = 84,  .ee = 168, .hs = 85,  .he = 85,  .ss = 255, .se = 255, .bs = 128, .be = 255, .ih =  true }, // Green
    { .es = 168, .ee = 255, .hs = 170, .he = 170, .ss = 255, .se = 255, .bs = 128, .be = 255, .ih =  true }, // Blue
	{ .ex = 1 }
};

// Do things when switching between layers
// @Override
layer_state_t layer_state_set_user(layer_state_t state) {
	switch (get_highest_layer(state)) {
		case 0:
			// g_layer_effect = NULL;
			break;
		case 1:
			// g_layer_effect = layer_rgb;
			break;
	}

	// set_layer_color();

	return state;
}
