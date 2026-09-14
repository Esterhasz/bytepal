#pragma once
#include "bytepal.h"

namespace fn {
	void draw_gif(const char* sourcePath, int targetFps, bytepal::Buffer2D& buf);
}