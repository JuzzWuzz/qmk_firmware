#pragma once


// --------------------
// Keyboard specific configuration (Defaults)
// Define versions without the _DEFAULT suffix to override

#define EFFECT_TIME_DEFAULT 2000                            // 2s per effect
#define EFFECT_TILING_COUNT_DEFAULT 1                       // Number of times the effect is visible over the board
#define EFFECT_TILING_FRACTIONAL_DEFAULT false              // Show the effect as a fraction of the board
#define EFFECT_ANGLE_DEFAULT EFFECT_DIRECTION_E             // Left to right
#define ALLOW_EFFECT_ROTATION_DEFAULT false                 // If the effect can be rotated by key inputs


// --------------------
// Some default RGB Matrix parameters

#ifndef RGB_MATRIX_CENTER
#   define RGB_MATRIX_CENTER {112, 32}
#endif
#define RGB_MATRIX_KEYRELEASES                              // Reactive effects respond to key releases (instead of key presses)
#define RGB_MATRIX_SLEEP                                    // Turn off effects when suspended
#define RGB_MATRIX_TIMEOUT 0                                // Number of milliseconds to wait until rgb automatically turns off. 0 to disable

#undef RGB_MATRIX_DEFAULT_MODE
#undef RGB_MATRIX_DEFAULT_ON
#undef RGB_MATRIX_LED_FLUSH_LIMIT
#undef RGB_MATRIX_LED_PROCESS_LIMIT
#undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#undef RGB_MATRIX_VAL_STEP

#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_JUZZ_WUZZ // Default RGB Matrix mode
#define RGB_MATRIX_DEFAULT_ON true                          // Sets the default enabled state
#define RGB_MATRIX_LED_FLUSH_LIMIT 10                       // Update frequency (ms). This is basically 100fps
#define RGB_MATRIX_LED_PROCESS_LIMIT RGB_MATRIX_LED_COUNT   // Number of LEDs to update per animation step
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255                   // Max brightness
#define RGB_MATRIX_VAL_STEP 51                              // Step for brightness, giving 5 steps


// --------------------
// Disable RGB Matrix Animations

#undef ENABLE_RGB_MATRIX_ALPHAS_MODS
#undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#undef ENABLE_RGB_MATRIX_BREATHING
#undef ENABLE_RGB_MATRIX_BAND_SAT
#undef ENABLE_RGB_MATRIX_BAND_VAL
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#undef ENABLE_RGB_MATRIX_CYCLE_ALL
#undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#undef ENABLE_RGB_MATRIX_DUAL_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#undef ENABLE_RGB_MATRIX_FLOWER_BLOOMING
#undef ENABLE_RGB_MATRIX_RAINDROPS
#undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#undef ENABLE_RGB_MATRIX_HUE_BREATHING
#undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#undef ENABLE_RGB_MATRIX_HUE_WAVE
#undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#undef ENABLE_RGB_MATRIX_PIXEL_RAIN
#undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#undef ENABLE_RGB_MATRIX_DIGITAL_RAIN
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#undef ENABLE_RGB_MATRIX_SPLASH
#undef ENABLE_RGB_MATRIX_MULTISPLASH
#undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#undef ENABLE_RGB_MATRIX_STARLIGHT
#undef ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE
#undef ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT
#undef ENABLE_RGB_MATRIX_RIVERFLOW
