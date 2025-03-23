RGB_MATRIX_EFFECT(JUZZ_WUZZ)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

__attribute__((weak)) void render_effect(uint8_t led_min, uint8_t led_max, effect_params_t* params) {}

bool JUZZ_WUZZ(effect_params_t* params) {
	RGB_MATRIX_USE_LIMITS(led_min, led_max);

	render_effect(led_min, led_max, params);

	return led_max < DRIVER_LED_TOTAL;
}

#endif
