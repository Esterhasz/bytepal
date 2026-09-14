#include "draw_gif.h"
#include "draw_image.h"

const int OutputWidth	= 64;
const int OutputHeight	= 64;

const int PixelWidth	= 2;

const char* SourcePath	= "content/heart_transparent.gif";

int main() {
	std::string str;
	std::cin >> str;
	std::cout << ANSI_HOME;

	bytepal::Buffer2D buffer(OutputWidth, OutputHeight, PixelWidth);

	fn::draw_gif(SourcePath, 15, buffer);

	return 0;
}