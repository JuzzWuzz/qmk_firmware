#include "common.h"

#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = {
	{
		// Key Matrix to LED Index
		{ 2, 1, 0 },
		{ 5, 4, 3 },
		{ 8, 7, 6 }
	},
	{
		// LED Index to Physical Position
		{ 224,   0 }, { 112,   0 }, {   0,   0 },
		{ 224,  32 }, { 112,  32 }, {   0,  32 },
		{ 224,  64 }, { 112,  64 }, {   0,  64 }
	},
	{
		// LED Index to Flag
		4, 4, 4,
		4, 4, 4,
		4, 4, 4
	}
};
#endif
