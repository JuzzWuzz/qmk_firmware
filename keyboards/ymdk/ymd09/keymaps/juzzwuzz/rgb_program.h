#pragma once

#include "common.h"

// iCue
animation_config animation_icue = {
	.effect_time		= 2000.0,
	.effects_per_board	= 1.0 / MATRIX_COLS,
	.effects = {
		{ .es = 0.00, .ee = 0.11, .hs = 300.00, .he = 293.88, .ss =  74.90, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
		{ .es = 0.11, .ee = 0.22, .hs = 293.88, .he = 284.94, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
		{ .es = 0.22, .ee = 0.33, .hs = 284.94, .he = 255.06, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih = false },
		{ .es = 0.33, .ee = 0.55, .hs = 255.06, .he = 224.94, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
		{ .es = 0.55, .ee = 0.66, .hs = 224.94, .he = 150.12, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih = false },
		{ .es = 0.66, .ee = 0.77, .hs = 150.12, .he = 135.06, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
		{ .es = 0.77, .ee = 0.88, .hs = 135.06, .he = 120.00, .ss = 100.00, .se =  74.90, .bs = 100.00, .be = 100.00, .ih =  true },
		{ .es = 0.88, .ee = 1.00, .hs = 120.00, .he = 300.00, .ss =  74.90, .se =  74.90, .bs = 100.00, .be = 100.00, .ih = false },
		{ .ex = 1 }
	}
};
// Juzz 1
animation_config animation_juzz1 = {
	.effect_time		= 2000.0,
	.effects_per_board	= 1.0 / MATRIX_COLS,
	.effects = {
		{ .es = 0.00, .ee = 0.90, .hs = 350.00, .he = 100.00, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
		{ .es = 0.90, .ee = 1.00, .hs = 100.00, .he = 350.00, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih = false },
		{ .ex = 1 }
	}
};
// Juzz 2
animation_config animation_juzz2 = {
	.effect_time		= 2000.0,
	.effects_per_board	= 1.0 / MATRIX_COLS,
	.effects = {
		{ .es = 0.00, .ee = 1.00, .hs = 360.00, .he =   0.00, .ss = 100.00, .se = 100.00, .bs = 100.00, .be = 100.00, .ih =  true },
		{ .ex = 1 }
	}
};

// Animations list
animation_config *g_animations[] = {
	&animation_icue,
	&animation_juzz1,
	&animation_juzz2
};
const uint8_t g_animation_count = sizeof(g_animations) / sizeof(*g_animations);
static uint8_t g_animation_index = 0;

// Brightness list
float g_brightness[] = {
	0.05f,
	0.40f,
	1.00f
};
const uint8_t g_brightness_count = sizeof(g_brightness) / sizeof(float);
static uint8_t g_brightness_index = g_brightness_count - 1;
