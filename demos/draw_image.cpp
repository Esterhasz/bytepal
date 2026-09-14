#include "draw_image.h"
#include "bytepal.h"
#include "ansi.h"
#include "stb_image.h"

void fn::draw_image(Image& image, bytepal::Buffer2D& buf) {
    auto bufW = buf.width();
    auto bufH = buf.height();

    float imageAspect = (float)image.width / (float)image.height;
    float bufferAspect = (float)bufW / (float)bufH;

    int renderW = bufW;
    int renderH = bufH;

    if (imageAspect > bufferAspect) {
        renderH = static_cast<int>(bufW / imageAspect);
    }
    else {
        renderW = static_cast<int>(bufH * imageAspect);
    }

    int offsetX = (bufW - renderW) / 2;
    int offsetY = (bufH - renderH) / 2;

    for (int y = 0; y < renderH; ++y) {
        int sourceY = y * image.height / renderH;
        int targetY = y;

        for (int x = 0; x < renderW; ++x) {
            int sourceX = x * image.width / renderW;
            int targetX = x;

            int index = (sourceY * image.width + sourceX) * image.channels;

            unsigned char r = image.image[index + 0];
            unsigned char g = image.image[index + 1];
            unsigned char b = image.image[index + 2];
            unsigned char a = image.image[index + 3];

            bytepal::Pixel finalPixel(nullptr, get_closest_ansi(r, g, b, a));

            buf.plot(targetX, targetY, finalPixel);
        }
    }
}

void fn::draw_image(const char* sourcePath, bytepal::Buffer2D& buf) {
    int width, height, channels;
    auto rawData = stbi_load(sourcePath, &width, &height, &channels, 4);

    int frameSizeBytes = width * height * 4;
    unsigned char* frameData = new unsigned char[frameSizeBytes];
    std::memcpy(frameData, rawData, frameSizeBytes);
    stbi_image_free(rawData);

    draw_image(Image(frameData, width, height, 4), buf);
    buf.draw(nullptr, nullptr);
    std::cout << ANSI_RESET;
}