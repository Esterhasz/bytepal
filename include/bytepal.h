#pragma once

#define ANSI_RESET			 "\033[0m"
#define ANSI_CLEAR			 "\033[2J"
#define ANSI_HOME			 "\033[H"

#define ANSI_CURSOR_HIDE	 "\033[?25l"
#define ANSI_CURSOR_SHOW	 "\033[?25h"

#define ANSI_FG_BLACK        "\033[30m"
#define ANSI_FG_DARK_RED     "\033[31m"
#define ANSI_FG_DARK_GREEN   "\033[32m"
#define ANSI_FG_DARK_YELLOW  "\033[33m"
#define ANSI_FG_DARK_BLUE    "\033[34m"
#define ANSI_FG_DARK_MAGENTA "\033[35m"
#define ANSI_FG_DARK_CYAN    "\033[36m"
#define ANSI_FG_GRAY         "\033[37m"

#define ANSI_FG_DARK_GRAY    "\033[90m"
#define ANSI_FG_RED          "\033[91m"
#define ANSI_FG_GREEN        "\033[92m"
#define ANSI_FG_YELLOW       "\033[93m"
#define ANSI_FG_BLUE         "\033[94m"
#define ANSI_FG_MAGENTA      "\033[95m"
#define ANSI_FG_CYAN         "\033[96m"
#define ANSI_FG_WHITE        "\033[97m"

#define ANSI_BG_BLACK        "\033[40m"
#define ANSI_BG_DARK_RED     "\033[41m"
#define ANSI_BG_DARK_GREEN   "\033[42m"
#define ANSI_BG_DARK_YELLOW  "\033[43m"
#define ANSI_BG_DARK_BLUE    "\033[44m"
#define ANSI_BG_DARK_MAGENTA "\033[45m"
#define ANSI_BG_DARK_CYAN    "\033[46m"
#define ANSI_BG_GRAY         "\033[47m"

#define ANSI_BG_DARK_GRAY    "\033[100m"
#define ANSI_BG_RED          "\033[101m"
#define ANSI_BG_GREEN        "\033[102m"
#define ANSI_BG_YELLOW       "\033[103m"
#define ANSI_BG_BLUE         "\033[104m"
#define ANSI_BG_MAGENTA      "\033[105m"
#define ANSI_BG_CYAN         "\033[106m"
#define ANSI_BG_WHITE        "\033[107m"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

namespace bytepal {

	struct Pixel {
		const char* data;
		const char* color;

		Pixel() :
			data(""),
			color(ANSI_RESET ANSI_FG_WHITE) {
		}
		Pixel(const char* data, const char* color) : 
			data(data), 
			color(color) {
		}
	};

	class Buffer2D {

		std::unique_ptr<Pixel[]> _buffer;
		std::string _out;

		std::size_t _width;
		std::size_t _height;
		
		std::uint8_t _pixelWidth;

	public:
		Buffer2D(std::size_t width, std::size_t height, std::uint8_t pixelWidth)
		{
			_width = width;
			_height = height;
			_pixelWidth = pixelWidth;

			_buffer = std::make_unique<Pixel[]>(_width * _height);

			_out.reserve(_width * _height * 16);
		}

		void fill(const Pixel& p) {

			for (std::size_t y = 0; y < _height; y++)
			{
				for (std::size_t x = 0; x < _width; x++)
				{
					at(x, y) = p;
				}
			}
		}

		void draw(const char* pre, const char* post)
		{
			_out.clear();

			if (pre)
				_out += pre;

			for (std::size_t y = 0; y < _height; y++)
			{
				for (std::size_t x = 0; x < _width; x++)
				{
					const Pixel& p = at(x, y);

					_out += p.color ? p.color : ANSI_RESET;

					int i = 0;

					while (i < _pixelWidth) {
						if (!p.data || p.data[i] == '\0') {
							_out += ' ';
						}
						else {
							_out += p.data[i];
						}

						i++;
					}
				}
				_out += '\n';
			}

			if (post)
				_out += post;
			std::cout << _out;
		}

		void drawLine(int x1, int y1, int x2, int y2, const Pixel& p) {
			int dx = std::abs(x2 - x1);
			int dy = std::abs(y2 - y1);

			int sx = (x1 < x2) ? 1 : -1;
			int sy = (y1 < y2) ? 1 : -1;

			int err = dx - dy;

			while (true) {
				plot(x1, y1, p);

				if (x1 == x2 && y1 == y2) {
					break;
				}

				int e2 = 2 * err;

				if (e2 > -dy) {
					err -= dy;
					x1 += sx;
				}

				if (e2 < dx) {
					err += dx;
					y1 += sy;
				}
			}
		}
		void drawRect(int x, int y, int width, int height, const Pixel& p) {
			if (width <= 0 || height <= 0)
				return;

			int x2 = x + width - 1;
			int y2 = y + height - 1;

			drawLine(x, y, x2, y, p);
			drawLine(x2, y, x2, y2, p);
			drawLine(x2, y2, x, y2, p);
			drawLine(x, y2, x, y, p);
		}

		void plot(int x, int y, const Pixel& p) {
			if (x < 0 || x >= static_cast<int>(_width) ||
				y < 0 || y >= static_cast<int>(_height)) {
				return;
			}

			at(x, y) = p;
		}

		Pixel& at(std::size_t x, std::size_t y) {
			return _buffer[y * _width + x];
		}
		const Pixel& at(std::size_t x, std::size_t y) const {
			return _buffer[y * _width + x];
		}

		std::size_t width() const {
			return _width;
		}
		std::size_t height() const {
			return _height;
		}
		std::uint8_t pixelWidth() const {
			return _pixelWidth;
		}
	};
}