#include "Filters/filters.h"

std::vector<Rgba> Filter::makeBW(const std::vector<Rgba>& data_, int height, int width) noexcept {

    std::vector<Rgba> output_data_ = data_;

    for (auto& pixel : output_data_) {
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

    return output_data_;
}

std::vector<Rgba> Filter::makeNegative(const std::vector<Rgba>& data_, int height, int width) noexcept {

    std::vector<Rgba> output_data_ = data_;

    for (auto& pixel : output_data_) {
        pixel.red = 255 - pixel.red ;
        pixel.green = 255 - pixel.green;
        pixel.blue = 255 - pixel.blue;
    }

    return output_data_;
}

std::vector<Rgba> Filter::keepCertainChannel(const std::vector<Rgba>& data_, int height, int width, const Rgba& color) noexcept
{
    std::vector<Rgba> output_data_ = data_;

    const float alpha = 0.5f;

    for (auto& pixel : output_data_) {
        pixel.red = static_cast<uint8_t>(
            pixel.red * (1.0f - alpha) + color.red * alpha);

        pixel.green = static_cast<uint8_t>(
            pixel.green * (1.0f - alpha) + color.green * alpha);

        pixel.blue = static_cast<uint8_t>(
            pixel.blue * (1.0f - alpha) + color.blue * alpha);
    }

    return output_data_;
}