#include "juzzwuzz.h"


// --------------------
// Effects: Custom for the keyboard

effect_t effect_tofu_rgb = {
    { .es = 0,   .ee = 84,  .hs = 0,   .he = 0,   .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // Red
    { .es = 84,  .ee = 168, .hs = 85,  .he = 85,  .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // Green
    { .es = 168, .ee = 255, .hs = 170, .he = 170, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // Blue
    { .ex = 1 }
};
effect_t effect_tofu_yoco= {
    { .es =   0, .ee = 128, .hs = 135, .he = 213, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = true },  // #00A9E0 → #E000A9
    { .es = 128, .ee = 255, .hs = 213, .he = 135, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = true },  // #E000A9 → #00A9E0
    { .ex = 1 }
};
effect_t *g_tofu_jr_effects[] = {
    &effect_tofu_rgb,
    &effect_tofu_yoco
};

// Indicating these are no longer weak externs
effect_t **g_effects = g_tofu_jr_effects;
uint8_t g_effect_count = ARRAY_COUNT(g_tofu_jr_effects);

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

// Handle custom keycodes for this keyboard only
bool process_keycode_juzz(uint16_t keycode) {
    if (keycode == KC_JZZ_SPECIAL) {
        // Your custom logic here
        tap_code(KC_ESC);
        return true; // Indicate that we handled it
    }
    return false; // Let it fall through
}
