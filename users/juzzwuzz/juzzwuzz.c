#include <lib/lib8tion/lib8tion.h>
#include <math.h>
#include "juzzwuzz.h"


// ----------------------------------------
// Constants
// ----------------------------------------

// # Handle default macro initialisation
#ifndef ANIMATION_TIME
#   define ANIMATION_TIME ANIMATION_TIME_DEFAULT
#endif
#ifndef ANIMATION_EFFECTS_PER_BOARD
#   define ANIMATION_EFFECTS_PER_BOARD ANIMATION_EFFECTS_PER_BOARD_DEFAULT
#endif
#ifndef ANIMATION_ANGLE
#   define ANIMATION_ANGLE ANIMATION_ANGLE_DEFAULT
#endif

const led_point_t   c_rgb_matrix_center     = RGB_MATRIX_CENTER;
const uint8_t       c_minimum_brightness    = 10;
const uint8_t       c_rgb_matrix_cols       = MATRIX_COLS;
const uint8_t       c_rgb_matrix_led_count  = RGB_MATRIX_LED_COUNT;

const uint16_t      c_animation_time        = ANIMATION_TIME;
const double        c_effects_per_board     = ANIMATION_EFFECTS_PER_BOARD;
const uint8_t       c_effect_angle          = (ANIMATION_ANGLE * 255) / 360;


// ----------------------------------------
// Effect Definitions
// ----------------------------------------


// --------------------
// Animation controls

bool                g_enabled               = false;
int8_t              g_cos                   = 0;
int8_t              g_sin                   = 0;
double              g_effect_offset         = 0.0;
bool                g_pause                 = false;

// --------------------
// Timers
uint32_t            g_timer                 = 0;
uint8_t             g_dt                    = 0;


// --------------------
// Effects

effect_t effect_icue = {
    { .es = 0,   .ee = 28,  .hs = 212, .he = 208, .ss = 191, .se = 255, .bs = 255, .be = 255, .ih =  true }, // FF40FF -> E500FF
    { .es = 28,  .ee = 56,  .hs = 208, .he = 202, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // E500FF -> BF00FF
    { .es = 56,  .ee = 84,  .hs = 202, .he = 180, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // BF00FF -> 4000FF
    { .es = 84,  .ee = 140, .hs = 180, .he = 159, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // 4000FF -> 0040FF
    { .es = 140, .ee = 168, .hs = 159, .he = 106, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = false }, // 0040FF -> 00FF80
    { .es = 168, .ee = 196, .hs = 106, .he = 95,  .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // 00FF80 -> 00FF40
    { .es = 196, .ee = 224, .hs = 95,  .he = 85,  .ss = 255, .se = 191, .bs = 255, .be = 255, .ih =  true }, // 00FF40 -> 40FF40
    { .es = 224, .ee = 255, .hs = 85,  .he = 212, .ss = 191, .se = 191, .bs = 255, .be = 255, .ih = false }, // 40FF40 -> FF40FF
    { .ex = 1 }
};
effect_t effect_icue2 = {
    { .es = 0,   .ee = 28,  .hs = 212, .he = 208, .ss = 191, .se = 255, .bs = 255, .be = 255, .ih =  true }, // FF40FF -> E500FF
    { .es = 28,  .ee = 56,  .hs = 208, .he = 202, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // E500FF -> BF00FF
    { .es = 56,  .ee = 84,  .hs = 202, .he = 180, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // BF00FF -> 4000FF
    { .es = 84,  .ee = 140, .hs = 180, .he = 159, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // 4000FF -> 0040FF
    { .es = 140, .ee = 168, .hs = 159, .he = 106, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // 0040FF -> 00FF80
    { .es = 168, .ee = 196, .hs = 106, .he = 95,  .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // 00FF80 -> 00FF40
    { .es = 196, .ee = 224, .hs = 95,  .he = 85,  .ss = 255, .se = 191, .bs = 255, .be = 255, .ih =  true }, // 00FF40 -> 40FF40
    { .es = 224, .ee = 255, .hs = 85,  .he = 212, .ss = 191, .se = 191, .bs = 255, .be = 255, .ih =  true }, // 40FF40 -> FF40FF
    { .ex = 1 }
};
effect_t effect_juzz1 = {
    { .es = 0,   .ee = 230, .hs = 248, .he = 71,  .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true },
    { .es = 230, .ee = 255, .hs = 71,  .he = 248, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = false },
    { .ex = 1 }
};
effect_t effect_juzz2 = {
    { .es = 0,   .ee = 84,  .hs = 0,   .he = 0,   .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true },
    { .es = 84,  .ee = 168, .hs = 167, .he = 167, .ss = 200, .se = 200, .bs = 255, .be = 255, .ih =  true },
    { .es = 168, .ee = 255, .hs = 78,  .he = 78,  .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true },
    { .ex = 1 }
};
effect_t *g_effects[] = {
    &effect_icue,
    &effect_icue2,
    &effect_juzz1,
    &effect_juzz2
};
const uint8_t g_effect_count = sizeof(g_effects) / sizeof(*g_effects);
uint8_t g_effect_index = 0;


// --------------------
// Layers

// effect_step_t layer_rgb[] = {
// 	{ .es = 0.00, .ee = 1.00, .hs =   0.00, .he = 240.00, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
// 	{ .ex = 1 }
// };
// effect_step_t layer_vlc[] = {
// 	{ .es = 0.00, .ee = 1.00, .hs =  11.29, .he =  11.29, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
// 	{ .ex = 1 }
// };
// effect_step_t layer_free[] = {
// 	{ .es = 0.00, .ee = 1.00, .hs = 352.47, .he =  18.82, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true, .iv =  true },
// 	{ .ex = 1 }
// };
effect_step_t *g_layer_effect = NULL;


// ----------------------------------------
// Animation Functions
// ----------------------------------------


rgb_t get_rgb_for_effect_and_time(const effect_step_t *effect, uint8_t time) {
	for (uint8_t i = 0; effect[i].ex != 1; i++) {
		effect_step_t cur = effect[i];

		if (time >= cur.es && time <= cur.ee) {
            fract8 t256 = scale8(255, (time - cur.es) * 255 / (cur.ee - cur.es));

			hsv_t bgn = { cur.hs, cur.ss, cur.bs };
			hsv_t end = { cur.he, cur.se, cur.be };

            rgb_t rgb;

            if (cur.ih) {
                // Interpolate Hue (circular interpolation)
                int16_t range = (int16_t)end.h - (int16_t)bgn.h;
                if (range > 127) {
                    range -= 256; // Wrap around backward
                } else if (range < -127) {
                    range += 256; // Wrap around forward
                }

                rgb = hsv_to_rgb((hsv_t){
                    .h = bgn.h + (range * t256 / 256),
                    .s = lerp8by8(bgn.s, end.s, t256),
                    .v = lerp8by8(bgn.v, end.v, t256)
                });
            } else {
                rgb_t rgb1 = hsv_to_rgb(bgn);
                rgb_t rgb2 = hsv_to_rgb(end);

                rgb.r = lerp8by8(rgb1.r, rgb2.r, t256);
                rgb.g = lerp8by8(rgb1.g, rgb2.g, t256);
                rgb.b = lerp8by8(rgb1.b, rgb2.b, t256);
            }

            // Scale to brightness
            uint8_t scale = rgb_matrix_get_val();
            if (scale < c_minimum_brightness) {
                scale = c_minimum_brightness;
            }
            return (rgb_t){
                .r = scale8(rgb.r, scale),
                .g = scale8(rgb.g, scale),
                .b = scale8(rgb.b, scale)
            };
		}
	}

	return (rgb_t){ .r = 0, .g = 0, .b = 0 };
}

void init_animation(void) {
    g_dt            = 0;
    g_cos           = cos8(c_effect_angle) - 128;
    g_sin           = sin8(c_effect_angle) - 128;
    g_effect_offset = c_effects_per_board / c_rgb_matrix_cols;
}

void set_layer_color(void) {
    if (g_enabled && g_layer_effect != NULL) {
        for (uint8_t i = 0; i < c_rgb_matrix_led_count; i++) {
            float dx = ((float)(g_led_config.point[i].x - c_rgb_matrix_center.x) / c_rgb_matrix_center.x) / 2.0f + 0.5f;

            rgb_t rgb = get_rgb_for_effect_and_time(g_layer_effect, dx);

            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
}

void render_effect(uint8_t led_min, uint8_t led_max, effect_params_t* params) {
    if (params->init) {
        init_animation();
    }

    // Bail if disabled, suspended or showing layer specific colors
    if (g_layer_effect != NULL) {
        return;
    }

    if (!g_pause) {
        g_dt = g_dt + scale16by8(sync_timer_elapsed32(g_timer), scale8(rgb_matrix_config.speed, 32)) + 1;
    }
    g_timer = sync_timer_read32();

    // Do the effect calculations
    for (uint8_t i = led_min; i < led_max; i++) {
        // Calculate the offset for the current LED
        int8_t dx = g_led_config.point[i].x - c_rgb_matrix_center.x;
        int8_t dy = g_led_config.point[i].y - c_rgb_matrix_center.y;
        int8_t offset = (dx * g_cos + dy * g_sin) >> 7;
        uint8_t dt = g_dt + offset;

        rgb_t rgb = get_rgb_for_effect_and_time((*g_effects[g_effect_index]), dt);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
}


// ----------------------------------------
// Keyboard Functions - Init
// ----------------------------------------


// Struct for user config
typedef union {
    uint32_t raw;
    struct {
        uint8_t effect_index :8;
    };
} user_config_t;
user_config_t user_config;

// EEPROM is getting reset! Store default values
// @Override
void eeconfig_init_user(void) {  // EEPROM is getting reset!
    user_config.raw = 0;
    user_config.effect_index = 0; // Set default effect index to 0
    eeconfig_update_user(user_config.raw); // Write default value to EEPROM now
}

// Fired once all keyboard init functions have been completed
// @Override
void keyboard_post_init_user(void) {
    // Read the user config from EEPROM
    user_config.raw = eeconfig_read_user();

	g_effect_index = user_config.effect_index;
	g_layer_effect = NULL;
    g_enabled = true;

#ifdef RGB_MATRIX_CUSTOM_USER
	rgb_matrix_mode(RGB_MATRIX_CUSTOM_JUZZ_WUZZ);
#endif
}


// ----------------------------------------
// Keyboard Functions - Key Processing
// ----------------------------------------


// Set the RGB effect index and save it to EEPROM
void set_rgb_effect(uint8_t effect_index) {
    // Set the effect index and save it to EEPROM
    g_effect_index = effect_index;
    user_config.effect_index = effect_index;
    eeconfig_update_user(user_config.raw);

    // Init the animation
    init_animation();
}

// Process user keystrokes
// @Override
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_JUZZ_PREV:
                // Decrement g_effect_index and wrap around if it goes below 0
                set_rgb_effect(submod8(g_effect_index, 1, g_effect_count));
                return false;

            case KC_JUZZ_NEXT:
                // Increment g_effect_index and wrap around if it exceeds g_effect_count - 1
                set_rgb_effect(addmod8(g_effect_index, 1, g_effect_count));
                return false;

            case RGB_J1:
                g_effect_index = 0;
                return false;

            case RGB_J2:
                g_effect_index = 1;
                return false;

            case RGB_J3:
                g_pause = !g_pause;
                return false;
        }
    }
    return true;
}


// ----------------------------------------
// Keyboard Functions - Layer Control
// ----------------------------------------


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
