#include "common.h"


// ----------------------------------------
// Hidden Functions
// ----------------------------------------



// Normal interpolation function
float func_interpolate(float x, float y, float t) {
	return (y - x) * t + x;
}

// Interpolation for values that specify directionality
float func_interpolate_hue(float x, float y, float t, bool i) {
	float range = (y - x);
	if (i) {
		if (range < 0.0f) {
			range += 360.0f;
		} else {
			range -= 360.0f;
		}
	}
	return wrap_float(range * t + x, 0.0f, 360.0f);
}



// ----------------------------------------
// Function Implementations
// ----------------------------------------



bool compare_float_eq(float f1, float f2) {
    return (fabs(f1 - f2) < EPSILON);
}

int wrap_int(int val, int const min, int const max) {
    int range = max - min;

    if (val < min) {
        val += range * (int)ceil(-(float)(val - min) / range);
    } else if (val > max) {
        val -= range * (int)floor((float)(val - min) / range);
        // Set to max value when wrapping as it sets it to min
        if (val == min) {
            val = max;
        }
    }

    return val;
}

float wrap_float(float val, float const min, float const max) {
    float range = max - min;

    if (val < min) {
        val += range * ceil(-(val - min) / range);
    } else if (val > max) {
        val -= range * floor((val - min) / range);
        // Set to max value when wrapping as it sets it to min
        if (compare_float_eq(val, min)) {
            val = max;
        }
    }

    return val;
}

HSB func_interpolate_hsb(HSB bgn, HSB end, float t, bool invert) {
	HSB hsb;
	hsb.h = func_interpolate_hue(bgn.h, end.h, t, invert);
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
				return hsb_to_rgb(func_interpolate_hsb(bgn, end, t, cur.iv));
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



// ----------------------------------------
// Test Stuff
// ----------------------------------------



#ifdef RUN_TESTS

typedef struct test_data_int {
    int val;
    int res;
    int end;
} test_data_int;

typedef struct test_data_float {
    float val;
    float res;
    float end;
} test_data_float;

void test_wrap_int(int const min, int const max, test_data_int data[]) {
    printf("Testing wrap_int(val, min, max)\n");
    bool success = true;
    for (int i = 0; data[i].end != 1; i++) {
        int val = data[i].val;
        int res = data[i].res;
        int act = wrap_int(val, min, max);
        bool pass = act == res;
        success &= pass;
        printf("    % 3d - (% 3d, % 3d) == % 3d: %s (% 3d)\n", val, min, max, res, pass ? "Pass" : "Fail", act);
    }
    if (success) {
        printf("All passed!\n\n");
    } else {
        printf("Tests failed!\n\n");
    }
}

void test_wrap_float(float const min, float const max, test_data_float data[]) {
    printf("Testing Wrap(val, min, max)\n");
    bool success = true;
    for (int i = 0; data[i].end != 1; i++) {
        float val = data[i].val;
        float res = data[i].res;
        float act = wrap_float(val, min, max);
        bool pass = compare_float_eq(act, res);
        success &= pass;
        printf("    % 8.4f - (% 8.4f, % 8.4f) == % 8.4f: %s (% 8.4f)\n", val, min, max, res, pass ? "Pass" : "Fail", act);
    }
    if (success) {
        printf("All passed!\n\n");
    } else {
        printf("Tests failed!\n\n");
    }
}

void run_tests() {
    test_wrap_int(0, 5, (test_data_int[]) {
        { .val =   0, .res = 0 }, // Min Test
        { .val =   5, .res = 5 }, // Max Test
        { .val =   3, .res = 3 }, // In-Range
        { .val =   6, .res = 1 }, // Above Max 1
        { .val =   7, .res = 2 }, // Above Max 2
        { .val =   8, .res = 3 }, // Above Max 3
        { .val =   9, .res = 4 }, // Above Max 4
        { .val =  10, .res = 5 }, // Above Max 5
        { .val =  11, .res = 1 }, // Above Max 6
        { .val =  15, .res = 5 }, // Above Max 7
        { .val =  -1, .res = 4 }, // Below Min 1
        { .val =  -2, .res = 3 }, // Below Min 2
        { .val =  -3, .res = 2 }, // Below Min 3
        { .val =  -4, .res = 1 }, // Below Min 4
        { .val =  -5, .res = 0 }, // Below Min 5
        { .val =  -6, .res = 4 }, // Below Min 6
        { .val = -15, .res = 0 }, // Below Min 7
        { .val =  11, .res = 1 }, // Above Max 8
        { .val =  12, .res = 2 }, // Above Max 9
        { .val =  13, .res = 3 }, // Above Max 10
        { .val =  14, .res = 4 }, // Above Max 11
        { .val =  15, .res = 5 }, // Above Max 12
        { .val =  16, .res = 1 }, // Above Max 13
        { .val =  17, .res = 2 }, // Above Max 14
        { .val =  18, .res = 3 }, // Above Max 15
        { .val =  19, .res = 4 }, // Above Max 16
        { .val =  20, .res = 5 }, // Above Max 17
        { .val =  21, .res = 1 }, // Above Max 18
        { .val =  22, .res = 2 }, // Above Max 19
        { .val =  23, .res = 3 }, // Above Max 20
        { .val =  24, .res = 4 }, // Above Max 21
        { .val =  25, .res = 5 }, // Above Max 22
        { .val =  26, .res = 1 }, // Above Max 23
        { .val =  27, .res = 2 }, // Above Max 24
        { .val =  28, .res = 3 }, // Above Max 25
        { .val =  29, .res = 4 }, // Above Max 26
        { .val =  30, .res = 5 }, // Above Max 27
        { .end =   1 }
    });
    test_wrap_int(2, 4, (test_data_int[]) {
        { .val =   2, .res = 2 }, // Min Test
        { .val =   4, .res = 4 }, // Max Test
        { .val =   3, .res = 3 }, // In-Range
        { .val =   5, .res = 3 }, // Above Max 1
        { .val =   6, .res = 4 }, // Above Max 2
        { .val =   7, .res = 3 }, // Above Max 3
        { .val =   8, .res = 4 }, // Above Max 4
        { .val =  15, .res = 3 }, // Above Max 5
        { .val =   1, .res = 3 }, // Below Min 1
        { .val =   0, .res = 2 }, // Below Min 2
        { .val =  -1, .res = 3 }, // Below Min 3
        { .val =  -2, .res = 2 }, // Below Min 4
        { .val = -15, .res = 3 }, // Below Min 5
        { .end =   1 }
    });
    test_wrap_int(-5, 0, (test_data_int[]) {
        { .val =  -5, .res = -5 }, // Min Test
        { .val =   0, .res =  0 }, // Max Test
        { .val =  -3, .res = -3 }, // In-Range
        { .val =   1, .res = -4 }, // Above Max 1
        { .val =   2, .res = -3 }, // Above Max 2
        { .val =   3, .res = -2 }, // Above Max 3
        { .val =   4, .res = -1 }, // Above Max 4
        { .val =   5, .res =  0 }, // Above Max 5
        { .val =   6, .res = -4 }, // Above Max 6
        { .val =  15, .res =  0 }, // Above Max 7
        { .val =  -6, .res = -1 }, // Below Min 1
        { .val =  -7, .res = -2 }, // Below Min 2
        { .val =  -8, .res = -3 }, // Below Min 3
        { .val =  -9, .res = -4 }, // Below Min 4
        { .val = -10, .res = -5 }, // Below Min 5
        { .val = -11, .res = -1 }, // Below Min 6
        { .val = -15, .res = -5 }, // Below Min 7
        { .end =   1 }
    });
    test_wrap_int(-4, 3, (test_data_int[]) {
        { .val =  -4, .res = -4 }, // Min Test
        { .val =   3, .res =  3 }, // Max Test
        { .val =  -1, .res = -1 }, // In-Range
        { .val =   4, .res = -3 }, // Above Max 1
        { .val =   5, .res = -2 }, // Above Max 2
        { .val =   6, .res = -1 }, // Above Max 3
        { .val =   7, .res =  0 }, // Above Max 4
        { .val =   8, .res =  1 }, // Above Max 5
        { .val =   9, .res =  2 }, // Above Max 6
        { .val =  10, .res =  3 }, // Above Max 7
        { .val =  11, .res = -3 }, // Above Max 8
        { .val =  15, .res =  1 }, // Above Max 9
        { .val =  -5, .res =  2 }, // Below Min 1
        { .val =  -6, .res =  1 }, // Below Min 2
        { .val =  -7, .res =  0 }, // Below Min 3
        { .val =  -8, .res = -1 }, // Below Min 4
        { .val =  -9, .res = -2 }, // Below Min 5
        { .val = -10, .res = -3 }, // Below Min 6
        { .val = -11, .res = -4 }, // Below Min 7
        { .val = -12, .res =  2 }, // Below Min 8
        { .val = -15, .res = -1 }, // Below Min 9
        { .end =   1 }
    });



    test_wrap_float(0.0, 5.0, (test_data_float[]) {
        { .val =   0.0, .res = 0.0 }, // Min Test
        { .val =   5.0, .res = 5.0 }, // Max Test
        { .val =   3.0, .res = 3.0 }, // In-Range
        { .val =   6.0, .res = 1.0 }, // Above Max 1
        { .val =   7.0, .res = 2.0 }, // Above Max 2
        { .val =   8.0, .res = 3.0 }, // Above Max 3
        { .val =   9.0, .res = 4.0 }, // Above Max 4
        { .val =  10.0, .res = 5.0 }, // Above Max 5
        { .val =  11.0, .res = 1.0 }, // Above Max 6
        { .val =  15.0, .res = 5.0 }, // Above Max 7
        { .val =  -1.0, .res = 4.0 }, // Below Min 1
        { .val =  -2.0, .res = 3.0 }, // Below Min 2
        { .val =  -3.0, .res = 2.0 }, // Below Min 3
        { .val =  -4.0, .res = 1.0 }, // Below Min 4
        { .val =  -5.0, .res = 0.0 }, // Below Min 5
        { .val =  -6.0, .res = 4.0 }, // Below Min 6
        { .val = -15.0, .res = 0.0 }, // Below Min 7
        { .end =   1 }
    });
    test_wrap_float(2.0, 4.0, (test_data_float[]) {
        { .val =   2.0, .res = 2.0 }, // Min Test
        { .val =   4.0, .res = 4.0 }, // Max Test
        { .val =   3.0, .res = 3.0 }, // In-Range
        { .val =   5.0, .res = 3.0 }, // Above Max 1
        { .val =   6.0, .res = 4.0 }, // Above Max 2
        { .val =   7.0, .res = 3.0 }, // Above Max 3
        { .val =   8.0, .res = 4.0 }, // Above Max 4
        { .val =  15.0, .res = 3.0 }, // Above Max 5
        { .val =   1.0, .res = 3.0 }, // Below Min 1
        { .val =   0.0, .res = 2.0 }, // Below Min 2
        { .val =  -1.0, .res = 3.0 }, // Below Min 3
        { .val =  -2.0, .res = 2.0 }, // Below Min 4
        { .val = -15.0, .res = 3.0 }, // Below Min 5
        { .end =   1 }
    });
    test_wrap_float(-5.0, 0.0, (test_data_float[]) {
        { .val =  -5.0, .res = -5.0 }, // Min Test
        { .val =   0.0, .res =  0.0 }, // Max Test
        { .val =  -3.0, .res = -3.0 }, // In-Range
        { .val =   1.0, .res = -4.0 }, // Above Max 1
        { .val =   2.0, .res = -3.0 }, // Above Max 2
        { .val =   3.0, .res = -2.0 }, // Above Max 3
        { .val =   4.0, .res = -1.0 }, // Above Max 4
        { .val =   5.0, .res =  0.0 }, // Above Max 5
        { .val =   6.0, .res = -4.0 }, // Above Max 6
        { .val =  15.0, .res =  0.0 }, // Above Max 7
        { .val =  -6.0, .res = -1.0 }, // Below Min 1
        { .val =  -7.0, .res = -2.0 }, // Below Min 2
        { .val =  -8.0, .res = -3.0 }, // Below Min 3
        { .val =  -9.0, .res = -4.0 }, // Below Min 4
        { .val = -10.0, .res = -5.0 }, // Below Min 5
        { .val = -11.0, .res = -1.0 }, // Below Min 6
        { .val = -15.0, .res = -5.0 }, // Below Min 7
        { .end =   1 }
    });
    test_wrap_float(-4.0, 3.0, (test_data_float[]) {
        { .val =  -4.0, .res = -4.0 }, // Min Test
        { .val =   3.0, .res =  3.0 }, // Max Test
        { .val =  -1.0, .res = -1.0 }, // In-Range
        { .val =   4.0, .res = -3.0 }, // Above Max 1
        { .val =   5.0, .res = -2.0 }, // Above Max 2
        { .val =   6.0, .res = -1.0 }, // Above Max 3
        { .val =   7.0, .res =  0.0 }, // Above Max 4
        { .val =   8.0, .res =  1.0 }, // Above Max 5
        { .val =   9.0, .res =  2.0 }, // Above Max 6
        { .val =  10.0, .res =  3.0 }, // Above Max 7
        { .val =  11.0, .res = -3.0 }, // Above Max 8
        { .val =  15.0, .res =  1.0 }, // Above Max 9
        { .val =  -5.0, .res =  2.0 }, // Below Min 1
        { .val =  -6.0, .res =  1.0 }, // Below Min 2
        { .val =  -7.0, .res =  0.0 }, // Below Min 3
        { .val =  -8.0, .res = -1.0 }, // Below Min 4
        { .val =  -9.0, .res = -2.0 }, // Below Min 5
        { .val = -10.0, .res = -3.0 }, // Below Min 6
        { .val = -11.0, .res = -4.0 }, // Below Min 7
        { .val = -12.0, .res =  2.0 }, // Below Min 8
        { .val = -15.0, .res = -1.0 }, // Below Min 9
        { .end =   1 }
    });
    test_wrap_float(0.5, 3.75, (test_data_float[]) {
        { .val =   0.50, .res = 0.50 }, // Min Test
        { .val =   3.75, .res = 3.75 }, // Max Test
        { .val =   1.37, .res = 1.37 }, // In-Range
        { .val =   4.75, .res = 1.50 }, // Above Max 1
        { .val =   8.00, .res = 1.50 }, // Above Max 2
        { .val =  -1.00, .res = 2.25 }, // Below Min 1
        { .val =  -6.13, .res = 3.62 }, // Below Min 2
        { .val = -15.00, .res = 1.25 }, // Below Min 3
        { .end =   1 }
    });
    test_wrap_float(-3.25, 1.75, (test_data_float[]) {
        { .val =  -3.25, .res = -3.25 }, // Min Test
        { .val =   1.75, .res =  1.75 }, // Max Test
        { .val =  -0.75, .res = -0.75 }, // In-Range
        { .val =   2.00, .res = -3.00 }, // Above Max 1
        { .val =   2.50, .res = -2.50 }, // Above Max 2
        { .val =   3.00, .res = -2.00 }, // Above Max 3
        { .val =   3.50, .res = -1.50 }, // Above Max 4
        { .val =   4.00, .res = -1.00 }, // Above Max 5
        { .val =   4.50, .res = -0.50 }, // Above Max 6
        { .val =   5.00, .res =  0.00 }, // Above Max 7
        { .val =   5.50, .res =  0.50 }, // Above Max 8
        { .val =   6.00, .res =  1.00 }, // Above Max 9
        { .val =   6.50, .res =  1.50 }, // Above Max 10
        
        { .val =  -4.00, .res =  1.00 }, // Below Min 1
        { .val =  -9.92, .res =  0.08 }, // Below Min 2
        { .val = -15.00, .res =  0.00 }, // Below Min 3
        { .end =   1 }
    });
}

#endif // RUN_TESTS
