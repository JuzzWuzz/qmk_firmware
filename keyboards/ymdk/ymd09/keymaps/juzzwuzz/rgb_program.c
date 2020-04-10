#include "rgb_program.h"

// Key controls
static uint16_t g_held_alt_tab;



#ifdef RGB_MATRIX_CUSTOM_USER

static float g_dt;
static uint32_t g_render_timer;

// This is the renderer from the custom effect we added
void render_effect(uint8_t led_min, uint8_t led_max, effect_params_t* params) {
	if (params->init) {
		g_dt = 0.0;
		g_render_timer = timer_read32();
	}

	// Calculated new dt
	float dt = timer_elapsed32(g_render_timer) / g_animations[g_animation_index]->effect_time;
	g_render_timer = timer_read32();
	g_dt = fmod(g_dt+dt, 1.0);

	// Do the effect calculations
	for (uint8_t column = 0; column < MATRIX_COLS; column++) {
		float time = fmod(g_dt + ((MATRIX_COLS - column) * (g_animations[g_animation_index]->effects_per_board / MATRIX_COLS)), 1.0);
		RGB rgb = get_rgb_scaled_to_brightness(get_rgb_for_effect_and_time(g_animations[g_animation_index]->effects, time), g_brightness[g_brightness_index]);
		for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
			uint8_t led_index = g_led_config.matrix_co[row][column];
			if (led_index != NO_LED) {
				rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
			}
		}
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
	debug_enable = true;

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
	if (g_held_alt_tab != 0 && timer_elapsed(g_held_alt_tab) > 1000) {
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
				xprintf("\n");
				eeconfig_debug_rgb_matrix();
				xprintf("g_animation_index: %d\n", g_animation_index);
				xprintf("g_animation_count: %d\n", g_animation_count);
				xprintf("\n");
			}
			return false;
		case RGB_J2:
			if (record->event.pressed) {
				g_dt = 0.0;
				xprintf("Resetting effect\n");
			}
			return false;
		case RGB_J3:
			if (record->event.pressed) {
				g_dt = 0.0;
				g_animation_index = (g_animation_index + 1) % g_animation_count;
				xprintf("Program: %d/%d\n", g_animation_index + 1, g_animation_count);
			}
			return false;
		default:
			//Process all other keycodes normally
			return true;
	}
}
