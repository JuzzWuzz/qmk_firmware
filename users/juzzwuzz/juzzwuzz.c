#include <lib/lib8tion/lib8tion.h>
#include <math.h>
#include "juzzwuzz.h"


// ----------------------------------------
// Constants
// ----------------------------------------

// # Handle default macro initialisation
#ifndef EFFECT_TIME
#   define EFFECT_TIME EFFECT_TIME_DEFAULT
#endif
#ifndef EFFECT_TILING_COUNT
#   define EFFECT_TILING_COUNT EFFECT_TILING_COUNT_DEFAULT
#endif
#ifndef EFFECT_TILING_FRACTIONAL
#  define EFFECT_TILING_FRACTIONAL EFFECT_TILING_FRACTIONAL_DEFAULT
#endif
#ifndef EFFECT_ANGLE
#   define EFFECT_ANGLE EFFECT_ANGLE_DEFAULT
#endif
#ifndef ALLOW_EFFECT_ROTATION
#   define ALLOW_EFFECT_ROTATION ALLOW_EFFECT_ROTATION_DEFAULT
#endif

const led_point_t   c_rgb_matrix_center             = RGB_MATRIX_CENTER;
const uint8_t       c_minimum_brightness            = 10;
const uint8_t       c_rgb_matrix_led_count          = RGB_MATRIX_LED_COUNT;

const uint16_t      c_effect_time                   = EFFECT_TIME;
const uint8_t       c_effect_tiling_count           = EFFECT_TILING_COUNT;
const bool          c_effect_tiling_fractional      = EFFECT_TILING_FRACTIONAL;
const uint8_t       c_effect_angles_count           = EFFECT_ANGLES_COUNT;
const bool          c_allow_effect_rotation         = ALLOW_EFFECT_ROTATION;
const double        c_cos_sin_value                 = 0.7071067812; // cos(45 degrees) = sin(45 degrees) = 0.7071067812


// ----------------------------------------
// Externs
// ----------------------------------------


effect_step_t *g_layer_effect = NULL;


// ----------------------------------------
// Effect Definitions
// ----------------------------------------


// --------------------
// Animation Controls

bool                g_enabled                       = false;
uint8_t             g_effect_angle                  = EFFECT_ANGLE;
uint8_t             g_max_dx                        = 0;
uint8_t             g_max_dy                        = 0;

// --------------------
// Timers
uint32_t            g_timer                         = 0;
uint16_t            g_dt                            = 0;


// --------------------
// Effects: Base versions should none be provided by a keyboard directly

effect_t effect_base_icue = {
    { .es = 0,   .ee = 28,  .hs = 212, .he = 208, .ss = 191, .se = 255, .bs = 255, .be = 255, .ih = true },  // #FF40FF -> #E500FF
    { .es = 28,  .ee = 56,  .hs = 208, .he = 202, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = true },  // #E500FF -> #BF00FF
    { .es = 56,  .ee = 84,  .hs = 202, .he = 180, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = true },  // #BF00FF -> #4000FF
    { .es = 84,  .ee = 120, .hs = 180, .he = 159, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = true },  // #4000FF -> #0040FF
    { .es = 120, .ee = 156, .hs = 159, .he = 106, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = true },  // #0040FF -> #00FF80
    { .es = 156, .ee = 184, .hs = 106, .he = 95,  .ss = 255, .se = 255, .bs = 255, .be = 255, .ih = true },  // #00FF80 -> #00FF40
    { .es = 184, .ee = 212, .hs = 95,  .he = 85,  .ss = 255, .se = 191, .bs = 255, .be = 255, .ih = true },  // #00FF40 -> #40FF40
    { .es = 212, .ee = 234, .hs = 85,  .he = 170, .ss = 191, .se = 200, .bs = 255, .be = 255, .ih = true },  // #40FF40 -> #3838FF
    { .es = 234, .ee = 255, .hs = 170, .he = 212, .ss = 200, .se = 191, .bs = 255, .be = 255, .ih = true },  // #3838FF -> #FF40FF
    { .ex = 1 }
};
effect_t effect_base_rgb = {
    { .es = 0,   .ee = 84,  .hs = 0,   .he = 0,   .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // Red
    { .es = 84,  .ee = 168, .hs = 85,  .he = 85,  .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // Green
    { .es = 168, .ee = 255, .hs = 170, .he = 170, .ss = 255, .se = 255, .bs = 255, .be = 255, .ih =  true }, // Blue
    { .ex = 1 }
};
effect_t *g_base_effects[] = {
    &effect_base_icue,
    &effect_base_rgb
};

// Handling extern and defining base effects as default case
__attribute__((weak)) effect_t **g_effects = g_base_effects;
__attribute__((weak)) uint8_t g_effect_count = ARRAY_COUNT(g_base_effects);
uint8_t g_effect_index = 0;


// ----------------------------------------
// Animation Functions
// ----------------------------------------


rgb_t get_rgb_for_effect_and_time(const effect_step_t *effect, uint8_t time) {
	for (uint8_t i = 0; effect[i].ex != 1; i++) {
		effect_step_t cur = effect[i];

		if (time >= cur.es && time <= cur.ee) {
            fract8 t256 = scale8(UINT8_MAX, (time - cur.es) * UINT8_MAX / (cur.ee - cur.es));

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
    g_dt = 0;
}

void set_layer_color(void) {
    if (g_enabled && g_layer_effect != NULL) {
        for (uint8_t i = 0; i < c_rgb_matrix_led_count; i++) {
            uint8_t dx = g_led_config.point[i].x * UINT8_MAX / g_max_dx;

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

    // Calculate the speed scaled to a uint16_t
    uint16_t speed = scale16by8(UINT16_MAX,rgb_matrix_config.speed);
    // The time to add is scaled to the speed divided by the animation time
    g_dt += sync_timer_elapsed32(g_timer) * speed / c_effect_time;
    g_timer = sync_timer_read32();

    // Do the effect calculations
    for (uint8_t i = led_min; i < led_max; i++) {
        // Calculate the offset for the current LED, applying the rotation if necessary
        int8_t dx = g_led_config.point[i].x - c_rgb_matrix_center.x;
        int8_t dy = g_led_config.point[i].y - c_rgb_matrix_center.y;
        int8_t offset = 0;
        switch (g_effect_angle) {
            case EFFECT_DIRECTION_N:
                offset = dy;
                break;
            case EFFECT_DIRECTION_NE:
                offset = -dx * c_cos_sin_value + dy * c_cos_sin_value;
                break;
            case EFFECT_DIRECTION_E:
                offset = -dx;
                break;
            case EFFECT_DIRECTION_SE:
                offset = -dx * c_cos_sin_value - dy * c_cos_sin_value;
                break;
            case EFFECT_DIRECTION_S:
                offset = -dy;
                break;
            case EFFECT_DIRECTION_SW:
                offset = dx * c_cos_sin_value - dy * c_cos_sin_value;
                break;
            case EFFECT_DIRECTION_W:
                offset = dx;
                break;
            case EFFECT_DIRECTION_NW:
                offset = dx * c_cos_sin_value + dy * c_cos_sin_value;
                break;
        }

        // Account for any tiling
        if (c_effect_tiling_fractional) {
            offset /= c_effect_tiling_count;
        } else {
            offset *= c_effect_tiling_count;
        }

        uint8_t dt = (g_dt / UINT8_MAX) + offset;

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
        uint8_t effect_angle :8;
        uint8_t effect_index :8;
    };
} user_config_t;
user_config_t user_config;

// EEPROM is getting reset! Store default values
// @Override
void eeconfig_init_user(void) {  // EEPROM is getting reset!
    user_config.raw = 0;
    user_config.effect_angle = EFFECT_ANGLE;
    user_config.effect_index = 0;
    eeconfig_update_user(user_config.raw); // Write default value to EEPROM now
}

// Save the user config to EEPROM
void eeconfig_save(void) {
    eeconfig_update_user(user_config.raw);
}

// Fired once all keyboard init functions have been completed
// @Override
void keyboard_post_init_user(void) {
    // Read the user config from EEPROM
    user_config.raw = eeconfig_read_user();

    // Loop over LEDs to get boundaries
    for (uint8_t i = 0; i < c_rgb_matrix_led_count; i++) {
        g_max_dx = MAX(g_max_dx, g_led_config.point[i].x);
        g_max_dy = MAX(g_max_dy, g_led_config.point[i].y);
    }

    // Set the global properties
	g_effect_angle = user_config.effect_angle;
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
void set_effect_index(uint8_t effect_index) {
    // Set the effect index and save it to EEPROM
    g_effect_index = effect_index;
    user_config.effect_index = effect_index;
    eeconfig_save();

    // Init the animation
    init_animation();
}

void set_effect_angle(uint8_t effect_angle) {
    if (c_allow_effect_rotation) {
        // Set the effect angle and save it to EEPROM
        g_effect_angle = effect_angle;
        user_config.effect_angle = effect_angle;
        eeconfig_save();

        // Init the animation
        init_animation();
    }
}

// Handling base functionality that indicates no processing handled at a base level
__attribute__((weak)) bool process_keycode_juzz(uint16_t keycode) {
    return false;
}

// Process user keystrokes
// @Override
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Process keycodes in keyboard overrides
        if (process_keycode_juzz(keycode)) {
            return false;
        }

        switch (keycode) {
            case J_EFF_P:
                // Decrement g_effect_index and wrap around if necessary
                set_effect_index(DECREMENT_WRAP(g_effect_index,g_effect_count));
                return false;

            case J_EFF_N:
                // Increment g_effect_index and wrap around if necessary
                set_effect_index(INCREMENT_WRAP(g_effect_index, g_effect_count));
                return false;

            case J_ROT_D:
                // Decrement g_effect_angle and wrap around if necessary
                set_effect_angle(DECREMENT_WRAP(g_effect_angle, c_effect_angles_count));
                return false;

            case J_ROT_I:
                // Increment g_effect_angle and wrap around if necessary
                set_effect_angle(INCREMENT_WRAP(g_effect_angle, c_effect_angles_count));
                return false;

            case J_RESET:
                // Reset the eeprom data and reload the default values
                eeconfig_init_user();

                // Reintialise the keyboard
                keyboard_post_init_user();

                // Reset the brightness
                rgb_matrix_sethsv(rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, UINT8_MAX);

                // Reinitialise the animation
                init_animation();
                return false;

            case RGB_J1:
                return false;

            case RGB_J2:
                return false;

            case RGB_J3:
                return false;
        }
    }
    return true;
}
