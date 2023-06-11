#include "rgb_program.h"


// Memory
// typedef union {
//	uint32_t raw;
//	struct {
//		bool     rgb_layer_change :1;
//	};
// } user_config_t;

// user_config_t user_config;
static bool g_enabled;
static uint8_t g_animation_index;
static uint8_t g_brightness_index;

// Layer contorls
static effect_config *g_layer_effect;

// Key controls
static uint16_t g_held_alt_tab;

// Sleep controls
static bool g_suspended;


// Private function definitions
void init_animation(void);
void set_layer_color(void);


// ----------------------------------------
// Animation Stuff
// ----------------------------------------



#ifdef RGB_MATRIX_CUSTOM_USER

// Constant variables
const point_t g_keyboard_center = { 112, 32 };

// Animation variables
static uint32_t g_render_timer;
static float g_dt;
static float g_cos;
static float g_sin;
static float g_effect_offset;

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

	// Bail if disabled, suspended or showing layer specific colors
	if (g_suspended || g_layer_effect != NULL) {
		return;
	}

	// Update global dt value
	g_dt = fmod(g_dt + (timer_elapsed32(g_render_timer) / g_animations[g_animation_index]->effect_time), 1.0f);
	g_render_timer = timer_read32();

	// Do the effect calculations
	for (uint8_t i = led_min; i < led_max; i++) {
		float dx  = (g_led_config.point[i].x - g_keyboard_center.x) / g_keyboard_center.x;
		float dy  = (g_led_config.point[i].y - g_keyboard_center.y) / g_keyboard_center.y;

		float dt = wrap_float(g_dt + (g_effect_offset * (dx * g_cos - dy * g_sin)), 0.0f, 1.0f);

		RGB rgb = get_rgb_scaled_to_brightness(get_rgb_for_effect_and_time(g_animations[g_animation_index]->effects, dt), g_brightness[g_brightness_index]);

		rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
	}
}

#else

void init_animation(void) { }

#endif // RGB_MATRIX_CUSTOM_USER



// ----------------------------------------
// Keyboard Functions - Init
// ----------------------------------------



// Load from memory
void memory_load(void) {
	// Read the user config from EEPROM
	user_config.raw = eeconfig_read_user();

	g_enabled = true;
	g_animation_index = 0;
	g_brightness_index = g_brightness_count - 1;
}

// Save to memory
void memory_save(void) {

}

// Fired once all keyboard init functions have been completed
// @Override
void keyboard_post_init_user(void) {
	g_layer_effect = NULL;
	g_held_alt_tab = 0;
	g_suspended	= false;

	// Load data from memory
	memory_load();
}

// Fired at the end of the Matrix init function
// @Override
void matrix_init_user(void) {
#ifdef RGB_MATRIX_CUSTOM_USER
	rgb_matrix_mode(RGB_MATRIX_CUSTOM_JUZZ_WUZZ);
#endif
}



// ----------------------------------------
// Keyboard Functions - Sleep
// ----------------------------------------



// Triggered when the system is powered down
// @Override
void suspend_power_down_user(void) {
	g_suspended	= true;
	rgb_matrix_disable_noeeprom();
	rgb_matrix_driver.set_color_all(0, 0, 0);
	rgb_matrix_driver.flush();
}

// Triggered when the system is resumed from standby
// @Override
void suspend_wakeup_init_user(void) {
	g_suspended	= false;
	rgb_matrix_enable_noeeprom();
}



// ----------------------------------------
// Keyboard Functions - Looping
// ----------------------------------------




// Runs constantly in the background (looping)
// @Override
void matrix_scan_user(void) {
	// Check for Super Alt-Tab key
	if (g_held_alt_tab != 0 && timer_elapsed(g_held_alt_tab) > 750) {
		unregister_code(KC_LALT);
		g_held_alt_tab = 0;
	}
}



// ----------------------------------------
// Keyboard Functions - Layer Control
// ----------------------------------------



// Do things when switching between layers
// @Override
layer_state_t layer_state_set_user(layer_state_t state) {
	switch (get_highest_layer(state)) {
		case LYR_BASE:
			g_layer_effect = NULL;
			// Back to base so reset the timer
			//g_render_timer = timer_read32();
			break;
		case LYR_RGB:
			g_layer_effect = layer_rgb;
			break;
		case LYR_VLC:
			g_layer_effect = layer_vlc;
			break;
		case LYR_FREE:
			g_layer_effect = layer_free;
			break;
	}

	set_layer_color();

	return state;
}

// Set Layer Color
void set_layer_color(void) {
	if (g_enabled && g_layer_effect != NULL) {
		for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
			float dx = ((float)(g_led_config.point[i].x - g_keyboard_center.x) / g_keyboard_center.x) / 2.0f + 0.5f;

			RGB rgb = get_rgb_scaled_to_brightness(get_rgb_for_effect_and_time(g_layer_effect, dx), g_brightness[g_brightness_index]);

			rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
		}
	}
}



// ----------------------------------------
// Keyboard Functions - Key Processing
// ----------------------------------------



// Process user keystrokes
// @Override
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	static uint16_t key_timer;

	switch (keycode) {
		// RGB Keys
		case RGB_BRI_I:
			if (record->event.pressed) {
				if (g_enabled) {
					g_brightness_index = MIN(g_brightness_index + 1, g_brightness_count - 1);
					set_layer_color();
				}
			}
			return false;
		case RGB_BRI_D:
			if (record->event.pressed) {
				if (g_enabled) {
					g_brightness_index = MAX(g_brightness_index - 1, 0);
					set_layer_color();
				}
			}
			return false;
		case RGB_TOG:
			if (record->event.pressed) {
				rgblight_toggle();
				set_layer_color();
			}
			return false;
		case RESET:
			if (record->event.pressed) {
				key_timer = timer_read();
			} else {
				if (timer_elapsed(key_timer) >= 500) {
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
