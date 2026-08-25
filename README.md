# bytepal
### Turn your console into a GUI!
A single-header C++ library for rendering colorful 2D graphics directly in your terminal.
The project uses ANSI escape sequences and a 2D buffer to turn terminal characters into simple graphical pixels.
Combine different colors and primitives to build simple interfaces, visualizations, games, and other terminal graphics!

## Features

* Single `bytepal.h` header
* No external dependencies
* ANSI foreground and background colors
* 2D pixel buffer
* Color-filled pixels
* Lines and rectangles
* Direct pixel access
* Customizable terminal output
* Lightweight API

## This is console output!
![Demo](media/gifPlayerHeart.gif)

## Quick Start

```cpp
#include "bytepal.h"

int main()
{
    using namespace bytepal;

    Buffer2D buffer(40, 20, 2); // <-- 2 is the width of each pixel in glyphs

    Pixel background(" ", ANSI_BG_BLACK);
    Pixel red(" ", ANSI_BG_RED);
    Pixel blue(" ", ANSI_BG_BLUE);

    buffer.fill(background);

    buffer.drawRect(5, 3, 30, 14, blue);
    buffer.plot(20, 10, red);

    buffer.draw(
        ANSI_CLEAR ANSI_HOME,
        ANSI_RESET
    );
}
```

The result is rendered directly in the terminal.

## Pixels

A `Pixel` consists of character data and an ANSI color:

```cpp
Pixel(const char* data, const char* color);
```

For rendering, a space combined with a background color can be used as a solid colored pixel:

```cpp
Pixel red(" ", ANSI_BG_RED);
```

You can use any single-byte characters, not only spaces:

```cpp
Pixel red("X", ANSI_BG_RED);
```

Each logical pixel can occupy multiple terminal characters horizontally. This is controlled by `pixelWidth`:

```cpp
Buffer2D buffer(40, 20, 2);
//                      ^ each pixel 2 console positions wide
```

A width of `2` makes pixels look closer to square on most terminals. Use `1` if wider pixels aren't needed.

Pixel data is trimmed or padded with spaces to fit `pixelWidth`.

## Drawing

bytepal provides a few basic drawing primitives.


```cpp
// a certain pixel
buffer.plot(x, y, pixel);

// line
buffer.drawLine(
    x1, y1,
    x2, y2,
    pixel
);

// hollow rectangle
buffer.drawRect(
    x, y,
    width, height,
    pixel
);

// fills the entire buffer
buffer.fill(pixel);
```

## Rendering

The entire buffer is rendered using a single output string.

```cpp
buffer.draw(pre, post);
```

`pre` is written before the buffer and `post` is written after it.

Both can be `nullptr`.

This leaves terminal control up to the application:

```cpp
buffer.draw(
    ANSI_HOME ANSI_CURSOR_HIDE,
    ANSI_CURSOR_SHOW
);
```

For example, a simple render loop can look like:

```cpp
while (running)
{
    // update

    buffer.draw(
        ANSI_HOME,
        nullptr
    );
}
```

## ANSI Colors

bytepal provides the standard 16 ANSI foreground and background colors.

```cpp

// foreground
ANSI_FG_BLACK
ANSI_FG_DARK_RED
ANSI_FG_DARK_GREEN
ANSI_FG_DARK_YELLOW
ANSI_FG_DARK_BLUE
ANSI_FG_DARK_MAGENTA
ANSI_FG_DARK_CYAN
ANSI_FG_GRAY

ANSI_FG_DARK_GRAY
ANSI_FG_RED
ANSI_FG_GREEN
ANSI_FG_YELLOW
ANSI_FG_BLUE
ANSI_FG_MAGENTA
ANSI_FG_CYAN
ANSI_FG_WHITE

// background
ANSI_BG_BLACK
ANSI_BG_DARK_RED
ANSI_BG_DARK_GREEN
ANSI_BG_DARK_YELLOW
ANSI_BG_DARK_BLUE
ANSI_BG_DARK_MAGENTA
ANSI_BG_DARK_CYAN
ANSI_BG_GRAY

ANSI_BG_DARK_GRAY
ANSI_BG_RED
ANSI_BG_GREEN
ANSI_BG_YELLOW
ANSI_BG_BLUE
ANSI_BG_MAGENTA
ANSI_BG_CYAN
ANSI_BG_WHITE

// other
ANSI_RESET
ANSI_CLEAR
ANSI_HOME
ANSI_CURSOR_HIDE
ANSI_CURSOR_SHOW
```

## Installation
bytepal is header-only.

1. Copy `bytepal.h` into your project. 
2. Add `#include "bytepal.h"`
3. You're done!

## Requirements
* C++11 or newer
* An ANSI-compatible terminal

## Notes
The project is intended for terminals that support ANSI escape sequences.

`Pixel::data` is treated as a sequence of single-byte characters. UTF-8 character handling is not provided.
`Buffer2D::at()` performs no bounds checking. Use `plot()` when coordinates may be outside the buffer.
