#include "../include/CNN/CNN_kernel.h"
#include <iostream>

CNN::CNN() noexcept : kernel_{} {}

CNN::CNN(int size, std::vector<float> kernel_data) noexcept : 
    kernel_{size, kernel_data} 
    {}

void CNN::proccesingImage(const std::vector<Rgba>& data_, int height, int width) noexcept {

    LaplacianFilter();

    int n_h{height}, n_w{width}; ///< высота и ширина изображения

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

void CNN::LaplacianFilter()  noexcept {
    for (auto& data : kernel_.kernel_data_) {
        data = 1.f;
    }
    kernel_.kernel_data_[4] = -8.f;
}