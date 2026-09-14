#pragma once
#include "bytepal.h"

struct Color {
	unsigned char r, g, b, a;
	const char* ansi;
};

const Color ansi_palette[] = {
	{ 0,   0,   0,   255, ANSI_BG_BLACK			},
	{ 128, 0,   0,   255, ANSI_BG_DARK_RED		},
	{ 0,   128, 0,   255, ANSI_BG_DARK_GREEN	},
	{ 128, 128, 0,   255, ANSI_BG_DARK_YELLOW	},
	{ 0,   0,   128, 255, ANSI_BG_DARK_BLUE		},
	{ 128, 0,   128, 255, ANSI_BG_DARK_MAGENTA	},
	{ 0,   128, 128, 255, ANSI_BG_DARK_CYAN		},
	{ 192, 192, 192, 255, ANSI_BG_GRAY			},
	{ 128, 128, 128, 255, ANSI_BG_DARK_GRAY		},
	{ 255, 0,   0,   255, ANSI_BG_RED			},
	{ 0,   255, 0,   255, ANSI_BG_GREEN			},
	{ 255, 255, 0,   255, ANSI_BG_YELLOW		},
	{ 0,   0,   255, 255, ANSI_BG_BLUE			},
	{ 255, 0,   255, 255, ANSI_BG_MAGENTA		},
	{ 0,   255, 255, 255, ANSI_BG_CYAN			},
	{ 255, 255, 255, 255, ANSI_BG_WHITE			},
};

const char* get_closest_ansi(
	unsigned char r, 
	unsigned char g, 
	unsigned char b, 
	unsigned char a, 
	unsigned char alphaTreshold = 12) {

	if (a < alphaTreshold)
		return ANSI_RESET;

	const char* best_ansi = ansi_palette[0].ansi;
	double min_dist = std::numeric_limits<double>::max();

	for (const auto& color : ansi_palette) {
		double dr = (double)r - color.r;
		double dg = (double)g - color.g;
		double db = (double)b - color.b;

		double dist = dr * dr + dg * dg + db * db;

		if (dist < min_dist) {
			min_dist = dist;
			best_ansi = color.ansi;
		}
	}
	return best_ansi;
}