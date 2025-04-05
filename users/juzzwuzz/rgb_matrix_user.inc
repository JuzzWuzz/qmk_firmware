RGB_MATRIX_EFFECT(JUZZ_WUZZ)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "juzzwuzz.h"

// Definition needed for QMK
bool JUZZ_WUZZ(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    render_effect(led_min, led_max, params);

    return rgb_matrix_check_finished_leds(led_max);
}

#endif
