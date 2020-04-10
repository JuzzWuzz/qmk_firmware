#include "common.h"

float func_interpolate(float x, float y, float t) {
	return (y - x) * t + x;
}

HSB func_interpolate_hsb(HSB bgn, HSB end, float t) {
	HSB hsb;
	hsb.h = fmod(func_interpolate(bgn.h, end.h, t), 360.0);
	hsb.s = CLAMP(func_interpolate(bgn.s, end.s, t), 0.0, 100.0);
	hsb.b = CLAMP(func_interpolate(bgn.b, end.b, t), 0.0, 100.0);
	return hsb;
}

RGB func_interpolate_rgb(RGB bgn, RGB end, float t) {
	RGB rgb;
	rgb.r = CLAMP(func_interpolate(bgn.r, end.r, t), 0, 255);
	rgb.g = CLAMP(func_interpolate(bgn.g, end.g, t), 0, 255);
	rgb.b = CLAMP(func_interpolate(bgn.b, end.b, t), 0, 255);
	return rgb;
}

RGB hsb_to_rgb(HSB hsb) {
	RGB rgb;

	float h = fmod(hsb.h, 360.0);
	float s = hsb.s / 100.0;
	float v = hsb.b / 100.0;

	float c = v * s;
	float x = c * (1 - fabs(fmod((h / 60.0), 2) - 1));
	float m = v - c;

	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
	if (h < 60.0) {
		r = c;
		g = x;
	} else if (h < 120.0) {
		r = x;
		g = c;
	} else if (h < 180.0) {
		g = c;
		b = x;
	} else if (h < 240.0) {
		g = x;
		b = c;
	} else if (h < 300.0) {
		r = x;
		b = c;
	} else if (h < 360.0) {
		r = c;
		b = x;
	}

	rgb.r = (r + m) * 255;
	rgb.g = (g + m) * 255;
	rgb.b = (b + m) * 255;

	return rgb;
}

RGB get_rgb_for_effect_and_time(effect_config *effect, float time) {
	for (uint8_t i = 0; effect[i].ex != 1; i++) {
		effect_config cur = effect[i];
		if (time >= cur.es && time <= cur.ee) {
			float t = (time - cur.es) / (cur.ee - cur.es);
			HSB bgn = { cur.hs, cur.ss, cur.bs };
			HSB end = { cur.he, cur.se, cur.be };

			if (cur.ih) {
				return hsb_to_rgb(func_interpolate_hsb(bgn, end, t));
			} else {
				return func_interpolate_rgb(hsb_to_rgb(bgn), hsb_to_rgb(end), t);
			}
		}
	}

	return (RGB){ .r = 0, .g = 0, .b = 0 };
}

RGB get_rgb_scaled_to_brightness(RGB rgb, float brightness) {
	rgb.r = ((float)rgb.r) * brightness;
	rgb.g = ((float)rgb.g) * brightness;
	rgb.b = ((float)rgb.b) * brightness;

	return rgb;
}
