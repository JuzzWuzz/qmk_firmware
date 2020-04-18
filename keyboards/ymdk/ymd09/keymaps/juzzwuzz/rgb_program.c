#include "rgb_program.h"

// Key controls
static uint16_t g_held_alt_tab;



#ifdef RGB_MATRIX_CUSTOM_USER

// Constant variables
const point_t g_keyboard_center = { 112, 32 };

// Animation variables
static uint32_t g_render_timer;
static float g_dt;
static float g_cos;
static float g_sin;
static float g_effect_offset;

float thing(float dt) {
	if (dt < 0.0f) {
		dt = fabs(dt);
		dt = 1.0f - (dt - (int)dt);
	} else if (dt >= 1.0f) {
		dt = dt - (int)dt;
	}

	return dt;
}

// Init Animation
void init_animation(void) {
	g_render_timer	= timer_read32();
	g_dt			= 0.0f;
	g_cos			= cos(g_animations[g_animation_index]->effect_angle);
	g_sin			= sin(g_animations[g_animation_index]->effect_angle);
	g_effect_offset	= g_animations[g_animation_index]->effects_per_board / MATRIX_COLS;
}

// This is the renderer from the custom effect we added
void render_effect(uint8_t led_min, uint8_t led_max, effect_params_t* params) {
	if (params->init) {
		init_animation();
	}

	// Update global dt value
	g_dt = fmod(g_dt + (timer_elapsed32(g_render_timer) / g_animations[g_animation_index]->effect_time), 1.0);
	g_render_timer = timer_read32();

	// Do the effect calculations
	for (uint8_t i = led_min; i < led_max; i++) {
		int16_t dx  = (g_led_config.point[i].x - g_keyboard_center.x) / g_keyboard_center.x;
		int16_t dy  = (g_led_config.point[i].y - g_keyboard_center.y) / g_keyboard_center.y;

		float dt = thing(g_dt + (g_effect_offset * (dx * g_cos - dy * g_sin)));

		RGB rgb = get_rgb_scaled_to_brightness(get_rgb_for_effect_and_time(g_animations[g_animation_index]->effects, dt), g_brightness[g_brightness_index]);

		rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
	}
}

#endif



// Triggered when the system is powered down
void suspend_power_down_user(void) {
	rgb_matrix_driver.set_color_all(0, 0, 0);
	rgb_matrix_driver.flush();
}

// Triggered when the system is resumed from standby
void suspend_wakeup_init_user(void) {
	rgb_matrix_enable_noeeprom();
}

// Fired once all keyboard init functions have been completed
void keyboard_post_init_user(void) {
	g_held_alt_tab = 0;
}

// Fired at the end of the Matrix init function
void matrix_init_user(void) {
#ifdef RGB_MATRIX_CUSTOM_USER
	rgb_matrix_mode(RGB_MATRIX_CUSTOM_JUZZ_WUZZ);
#endif
}

// Runs constantly in the background (looping)
void matrix_scan_user(void) {
	if (g_held_alt_tab != 0 && timer_elapsed(g_held_alt_tab) > 500) {
		unregister_code(KC_LALT);
		g_held_alt_tab = 0;
	}
}

// Do things when switching between layers
layer_state_t layer_state_set_user(layer_state_t state) {
	switch (get_highest_layer(state)) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	}
	return state;
}

// Process user keystrokes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	static uint32_t key_timer;

	switch (keycode) {
		// RGB Keys
		case RGB_BRI_I:
			if (record->event.pressed) {
				g_brightness_index = MIN(g_brightness_index + 1, g_brightness_count - 1);
			}
			return false;
		case RGB_BRI_D:
			if (record->event.pressed) {
				g_brightness_index = MAX(g_brightness_index - 1, 0);
			}
			return false;
		case RESET:
			if (record->event.pressed) {
				key_timer = timer_read32();
			} else {
				if (timer_elapsed32(key_timer) >= 500) {
					reset_keyboard();
				}
			}
			return false;

		// Special Keys
		case ALT_TAB:
			if (record->event.pressed) {
				if (g_held_alt_tab == 0) {
					register_code(KC_LALT);
				}
				g_held_alt_tab = timer_read();
				register_code(KC_TAB);
			} else {
				unregister_code(KC_TAB);
			}
			return false;

		// ??? Keys
		case RGB_J1:
			if (record->event.pressed) {
				g_dt = 0.0;
			}
			return false;
		case RGB_J2:
			if (record->event.pressed) {
				g_animation_index = (g_animation_index + 1) % g_animation_count;
				init_animation();
			}
			return false;
		case RGB_J3:
			if (record->event.pressed) {
			}
			return false;
		default:
			//Process all other keycodes normally
			return true;
	}
}
