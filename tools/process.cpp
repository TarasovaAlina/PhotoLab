#include "../include/process.h"

void makeBW(Bmp<Rgba>& bmp) noexcept {
    for (uint32_t i{}, height{bmp.getHeight()}; i < height; ++i) {
        for (uint32_t j{}, width{ bmp.getWidth() }; j < width; ++j) {
            auto& pixel{ bmp(i, j) };
            const auto average { static_cast<uint8_t>(
                  static_cast<uint8_t>(pixel.red)
                + static_cast<uint8_t>(pixel.green)
                + static_cast<uint8_t>(pixel.blue)
                / 3)
            };
            pixel.red = average;
            pixel.green = average;
            pixel.blue = average;
        }
    }
}

void makeNegative(Bmp<Rgba>& bmp) noexcept {
    for (uint32_t i{}, height{bmp.getHeight()}; i < height; ++i) {
        for (uint32_t j{}, width{ bmp.getWidth() }; j < width; ++j) {
            auto& pixel{ bmp(i, j) };

            pixel.red = 255 - pixel.red ;
            pixel.green = 255 - pixel.green;
            pixel.blue = 255 - pixel.blue;
            // pixel.alpha = average;
        }
    }
}

void keepGreenChannel(Bmp<Rgba>& bmp) noexcept {
    for (uint32_t i{}, height{bmp.getHeight()}; i < height; ++i) {
        for (uint32_t j{}, width{ bmp.getWidth() }; j < width; ++j) {
            auto& pixel{ bmp(i, j) };
            pixel.red = 0;
            pixel.green = 0;
            // pixel.blue = ;
        }
    }
}