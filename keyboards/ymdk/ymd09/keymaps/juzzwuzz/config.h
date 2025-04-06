#pragma once


// --------------------
// Keyboard specific configuration

#ifdef PRODUCT
#	undef PRODUCT
#endif
#define PRODUCT "Juzz Macropad"

#define EFFECT_TILING_COUNT 3                               // Show 3 instances of the effect
#define EFFECT_TILING_FRACTIONAL true                       // Swapping this to be fractional instances
#define EFFECT_ANGLE EFFECT_DIRECTION_NE                    // Bottom-left to top-right
#define ALLOW_EFFECT_ROTATION true                          // Allow the effect to rotate

#define DYNAMIC_KEYMAP_LAYER_COUNT 3
