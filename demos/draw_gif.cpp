#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <cstring>
#include <regex>
#include "bytepal.h"
#include "stb_image.h"
#include "draw_gif.h"
#include "Image.h"
#include "draw_image.h"

using namespace bytepal;
namespace fs = std::filesystem;

static void free_frames(std::vector<Image>& frames) {
    for (auto& f : frames) {
        if (f.image) {
            delete[] f.image;
        }
    }
    frames.clear();
}

static std::vector<Image> load_gif_frames(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
        return {};

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    int width, height, frameCount, channels;
    int* delays = nullptr;

    unsigned char* rawData = stbi_load_gif_from_memory(
        (const stbi_uc*)buffer.data(),
        (int)buffer.size(),
        &delays,
        &width,
        &height,
        &frameCount,
        &channels,
        4
    );

    if (!rawData) 
        return {};

    std::vector<Image> frames;
    int frameSizeBytes = width * height * 4;

    for (int i = 0; i < frameCount; ++i) {
        unsigned char* frameData = new unsigned char[frameSizeBytes];
        std::memcpy(frameData, rawData + (i * frameSizeBytes), frameSizeBytes);
        frames.push_back(Image(frameData, width, height, 4));
    }

    STBI_FREE(delays);
    stbi_image_free(rawData);

    return frames;
}

static int frame_number(const fs::path& path) {
    std::string filename = path.filename().string();
    std::smatch match;

    if (std::regex_search(filename, match, std::regex(R"(\d+)"))) {
        return std::stoi(match.str());
    }
    return 0;
}

static std::vector<Image> load_framebyframe_video(const std::string& dirPath) {
    std::vector<fs::path> paths;
    std::vector<Image> frames;

    if (!fs::exists(dirPath)) 
        return {};

    for (auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            paths.push_back(entry.path());
        }
    }

    std::sort(paths.begin(), paths.end(), [](const fs::path& a, const fs::path& b) {
        return frame_number(a) < frame_number(b);
        });

    for (fs::path& p : paths) {
        int width, height, channels;
        auto rawData = stbi_load(p.string().c_str(), &width, &height, &channels, 4);

        if (!rawData) 
            continue;

        int frameSizeBytes = width * height * 4;
        unsigned char* frameData = new unsigned char[frameSizeBytes];
        std::memcpy(frameData, rawData, frameSizeBytes);
        stbi_image_free(rawData);

        frames.push_back(Image(frameData, width, height, 4));
    }

    return frames;
}

void fn::draw_gif(const char* sourcePath, int targetFps, Buffer2D& buf) {
    fs::path path(sourcePath);
    std::vector<Image> frames;

    if (fs::is_directory(path)) {
        frames = load_framebyframe_video(sourcePath);
    }
    else if (fs::is_regular_file(path) && path.extension() == ".gif") {
        frames = load_gif_frames(sourcePath);
    }
    else {
        std::cerr << "Error: Invalid path or unsupported format: " << sourcePath << std::endl;
        return;
    }

    if (frames.empty()) {
        std::cerr << "Error: No frames loaded from " << sourcePath << std::endl;
        return;
    }

    std::cout
        << ANSI_CLEAR
        << ANSI_HOME
        << ANSI_CURSOR_HIDE;

    std::size_t frameNum = 0;
    const auto frameDuration = std::chrono::milliseconds(1000 / targetFps);

    Pixel background = Pixel(nullptr, nullptr);

    while (true) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        buf.fill(background);

        Image& f = frames[frameNum % frames.size()];

        fn::draw_image(f, buf);

        buf.draw(ANSI_HOME, nullptr);
        frameNum++;

        auto frameTime = std::chrono::high_resolution_clock::now() - frameStart;
        if (frameTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - frameTime);
        }
    }

    free_frames(frames);
    std::cout << ANSI_CURSOR_SHOW << ANSI_RESET;
}