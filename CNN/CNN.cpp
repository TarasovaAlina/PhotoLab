#include "CNN/CNN_kernel.h"
#include <cmath>

ConvolutionFilter::ConvolutionFilter() noexcept: kernel_() {}

ConvolutionFilter::ConvolutionFilter(int size, std::vector<float> kernel_data) noexcept : 
    kernel_{size, kernel_data} 
    {}
    
std::vector<Rgba> ConvolutionFilter::apply(const std::vector<Rgba>& data, int height, int width) const {
    int n_h{height}, n_w{width};
    std::vector<Rgba> output_data_;
    output_data_.resize(data.size());

    for (auto& pixel : output_data_) {
        pixel = Rgba{0, 0, 0, 255};
    }

    for (int i{0}; i < n_h - kernel_.p_h + 1; ++i) {
        for (int j{0}; j < n_w - kernel_.p_w + 1; ++j) {
            proccesingParticularSpace(output_data_, data, i, j, n_w);
        }
    }
    return output_data_;
}

void ConvolutionFilter::proccesingParticularSpace(std::vector<Rgba>& output_data_, const std::vector<Rgba>& data_, int h, int w, int width) const noexcept{

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

void ConvolutionFilter::setKernelSize(int size) noexcept {
    kernel_.k_size = size;
    kernel_.p_w = kernel_.p_h = size - 1;
    kernel_.kernel_data_.resize(size * size);
}
void ConvolutionFilter::setKernel(const std::vector<float>& kernel) noexcept {
    kernel_.k_size = sqrt(kernel.size());
    kernel_.p_w = kernel_.p_h = kernel_.k_size - 1;
    kernel_.kernel_data_.resize(kernel.size() * kernel.size());
    kernel_.kernel_data_ = kernel;
}

GradientFilter::GradientFilter() : 
    gx_{3, std::vector<float>{ -1.f, -1.f, -1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f }}, 
    gy_(3, std::vector<float>{ -1.f, 0.f, 1.f, -1.f, 0.f, 1.f, -1.f, 0.f, 1.f })
    {}

std::vector<Rgba> GradientFilter::apply(const std::vector<Rgba>& data_, int height, int width) const {

    int n_h{height}, n_w{width};
    std::vector<Rgba> output_data_;
    output_data_.resize(data_.size());
    std::vector<Rgba> temp_1;
    std::vector<Rgba> temp_2;

    temp_1.resize(data_.size());
    temp_2.resize(data_.size());
    output_data_.resize(data_.size());

    for (auto& pixel : temp_1) {
        pixel = Rgba{0, 0, 0, 255};
    }

    for (auto& pixel : temp_2) {
        pixel = Rgba{0, 0, 0, 255};
    }

    for (auto& pixel : output_data_) {
        pixel = Rgba{0, 0, 0, 255};
    }

    for (int i{0}; i < n_h - gx_.p_h; ++i) {
        for (int j{0}; j < n_w - gx_.p_w + 1; ++j) {
            proccesingParticularSpace(temp_1, data_, i, j, n_w, 0);
        }
    }

    for (int i{0}; i < n_h - gy_.p_h + 1; ++i) {
        for (int j{0}; j < n_w - gy_.p_w + 1; ++j) {
            proccesingParticularSpace(temp_2, data_, i, j, n_w, 1);
        }
    }

    for (int i = 0; i < output_data_.size(); ++i) {
        temp_Rgba temp{};
        temp.blue = std::abs(temp_1[i].blue) + std::abs(temp_2[i].blue);
        temp.green = std::abs(temp_1[i].green) + std::abs(temp_2[i].green);
        temp.red = std::abs(temp_1[i].red) + std::abs(temp_2[i].red);
 
        const uint8_t originalAlpha = output_data_[i].alpha;
        output_data_[i] = temp;
        output_data_[i].alpha = originalAlpha;
    }

    return output_data_;
}

void GradientFilter::proccesingParticularSpace(std::vector<Rgba>& output_data_, const std::vector<Rgba>& data_, int h, int w, int width, int type) const noexcept{

    const Kernel& kernel_ = (type == 0) ? gx_ : gy_ ; 
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

void GradientFilter::setKernelSize(int size) noexcept {}

void GradientFilter::setKernel(const std::vector<float>& kernel) noexcept {}

void CNN::setFilter(std::unique_ptr<IFilterStrategy> filter) { 
    filter_ = std::move(filter); 
}

void CNN::proccesingImage(const std::vector<Rgba>& data_, int height, int width) noexcept {
    if (!filter_) return;
    output_data_ = filter_->apply(data_, height, width);
}

const std::vector<Rgba>& CNN::getOutputData() const noexcept { 
    return output_data_; 
}

void CNN::setKernelSize(int size) noexcept {
    if (!filter_) return;
    filter_->setKernelSize(size);
}
void CNN::setKernel(const std::vector<float>& kernel) noexcept {
    if (!filter_) return;
    filter_->setKernel(kernel);
}

std::unique_ptr<IFilterStrategy> FilterFactory::create(CONVOLUTION_FILTER type, int size) {
    if (type == Emboss) 
        return std::make_unique<ConvolutionFilter>(size, std::vector<float>{{ -2.f, -1.f, 0.f, -1.f, 1.f, 1.f, 0.f, 1.f, 2.f }});
    else if (type == Sharpen) 
        return std::make_unique<ConvolutionFilter>(size, std::vector<float>{{ -0.f, -1.f, 0.f, -1.f, 5.f, -1.f, 0.f, -1.f, 0.f }});
    else if (type == BoxBlur) {
        const int n = size * size;
        std::vector<float> data(static_cast<size_t>(n), 1.0f / static_cast<float>(n));
        return std::make_unique<ConvolutionFilter>(size, std::move(data));
    }
    else if (type == GaussianBlur) 
        return std::make_unique<ConvolutionFilter>(size, std::vector<float>{{ 0.0625f, 0.125f, 0.0625f, 0.125f, 0.25f, 0.125f, 0.0625f, 0.125f, 0.0625f }});
    else if (type == LaplacianFilter) 
        return std::make_unique<ConvolutionFilter>(size, std::vector<float>{{ 0.f, 1.f, 0.f, 1.f, -4.f, 1.f, 0.f, 1.f, 0.f }});
    else if (type == PrewittFilter) 
        return std::make_unique<GradientFilter>();
    else return std::make_unique<ConvolutionFilter>();
}