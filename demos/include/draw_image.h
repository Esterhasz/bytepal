#pragma once
#include "bytepal.h"
#include "Image.h"

namespace fn {
	void draw_image(Image& image, bytepal::Buffer2D& buf);
	void draw_image(const char* sourcePath, bytepal::Buffer2D& buf);
}