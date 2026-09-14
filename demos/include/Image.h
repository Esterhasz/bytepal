#pragma once

struct Image {
    int width;
    int height;
    int channels;
    unsigned char* image;

    Image(unsigned char* img, int w, int h, int c) :
        image(img), width(w), height(h), channels(c) {}
};