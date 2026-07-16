#include "CNN/CNN_kernel.h"

CNN::CNN() noexcept : kernel_{} {}

CNN::CNN(int size, std::vector<float> kernel_data) noexcept : 
    kernel_{size, kernel_data} 
    {}

void CNN::makeBW(const std::vector<Rgba>& data_, int height, int width) noexcept {

    output_data_ = data_;
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
}

void CNN::makeNegative(const std::vector<Rgba>& data_, int height, int width) noexcept {

    output_data_ = data_;
    for (auto& pixel : output_data_) {
        pixel.red = 255 - pixel.red ;
        pixel.green = 255 - pixel.green;
        pixel.blue = 255 - pixel.blue;
    }
}

void CNN::keepCertainChannel(const std::vector<Rgba>& data,
                             int height,
                             int width,
                             const Rgba& color) noexcept
{
    output_data_ = data;

    const float alpha = 0.5f;

    for (auto& pixel : output_data_) {
        pixel.red = static_cast<uint8_t>(
            pixel.red * (1.0f - alpha) + color.red * alpha);

        pixel.green = static_cast<uint8_t>(
            pixel.green * (1.0f - alpha) + color.green * alpha);

        pixel.blue = static_cast<uint8_t>(
            pixel.blue * (1.0f - alpha) + color.blue * alpha);
    }
}

void CNN::proccesingImage(const std::vector<Rgba>& data_, int height, int width) noexcept {

    int n_h{height}, n_w{width};

    output_data_.resize(data_.size());
    for (auto& pixel : output_data_) {
        pixel = Rgba{0, 0, 0, 255};
    }

    for (int i{0}; i < n_h - kernel_.p_h + 1; ++i) {
        for (int j{0}; j < n_w - kernel_.p_w + 1; ++j) {
            proccesingParticularSpace(data_, i, j, n_w);
        }
    }
}

void CNN::proccesingParticularSpace(const std::vector<Rgba>& data_, int h, int w, int width) noexcept{

    int new_height = h + kernel_.p_h / 2;
    int new_width = w + kernel_.p_w / 2;

    temp_Rgba rgba{};

    for (int i{}; i < kernel_.k_size; ++i) {
        for (int j{}; j < kernel_.k_size; ++j) {
            rgba += data_[(h + i) * width + (w + j)] * kernel_.kernel_data_[i * kernel_.k_size + j];
        }
    }

    output_data_[new_height * width + new_width] = rgba;
}

const std::vector<Rgba>& CNN::getOutputData() const noexcept {
    return output_data_;
}

void CNN::setKernelSize(int size) noexcept {
    kernel_.k_size = size;
    kernel_.p_h = kernel_.p_w = size - 1;
    kernel_.kernel_data_.resize(size * size);
}

void CNN::setKernel(const std::vector<float>& kernel) noexcept {
    kernel_.kernel_data_ = kernel;
}

void CNN::setEmbossFilter() noexcept {
    kernel_.kernel_data_ = { -2.f, -1.f, 0.f, -1.f, 1.f, 1.f, 0.f, 1.f, 2.f };
}

void CNN::setSharpenFilter() noexcept {
    kernel_.kernel_data_ = { -0.f, -1.f, 0.f, -1.f, 5.f, -1.f, 0.f, -1.f, 0.f };
}

void CNN::setBoxBlurFilter()  noexcept {
    for (auto& data : kernel_.kernel_data_) {
        data = 1.f / (kernel_.k_size * kernel_.k_size);
    }
}

void CNN::setGaussianBlurFilter() noexcept {
    kernel_.kernel_data_ = { 0.0625f, 0.125f, 0.0625f, 0.125f, 0.25f, 0.125f, 0.0625f, 0.125f, 0.0625f };
}

void CNN::setLaplacianFilter()  noexcept {
    for (auto& data : kernel_.kernel_data_) {
        data = 1.f;
    }
    kernel_.kernel_data_[4] = -8.f;
}