#include "controller/controller.h"

Controller::Controller() noexcept: 
    original_{0, 0},  processed_{0,0} , cnn_{} {}

bool Controller::loadFile(const std::string& filename) noexcept(false) {
    original_.loadFile(filename);
    processed_ = original_;
    return true;
}

bool Controller::saveFile(const std::string& filename) noexcept(false) {
    processed_.saveFile(filename);
    return true;
}

void Controller::setFilter(CONVOLUTION_FILTER filter) noexcept
{
    cnn_.setKernelSize(STANDART_KERNEL_SIZE);

    switch (filter) {
    case Emboss:
        cnn_.setEmbossFilter();
        break;
    case Sharpen:
        cnn_.setSharpenFilter();
        break;
    case BoxBlur:
        cnn_.setBoxBlurFilter();
        break;
    case GaussianBlur:
        cnn_.setGaussianBlurFilter();
        break;
    case LaplacianFilter:
        cnn_.setLaplacianFilter();
        break;
    }
}

void Controller::setKernelSize(int size) noexcept {
    cnn_.setKernelSize(size);
}

void Controller::setKernel(const std::vector<float>& kernel) noexcept {
    cnn_.setKernel(kernel);
}

void Controller::processImage() {

    cnn_.proccesingImage(processed_.data(), processed_.getHeight(), processed_.getWidth());

    processed_.setData(cnn_.getOutputData());
}

void Controller::processImage(bool filter) {
    cnn_.proccesingImage(processed_.data(), processed_.getHeight(), processed_.getWidth(), PrewittFilter);

    processed_.setData(cnn_.getOutputData());
}

void Controller::clearImage() {
    processed_.setData(original_.data());
}

void Controller::makeBWFunction() noexcept {

    cnn_.makeBW(processed_.data(), processed_.getHeight(), processed_.getWidth());

    processed_.setData(cnn_.getOutputData());
}

void Controller::keepCertainChannelFunction(const Rgba& color) noexcept {

    cnn_.keepCertainChannel(processed_.data(), processed_.getHeight(), processed_.getWidth(), color);

    processed_.setData(cnn_.getOutputData());
}

void Controller::makeNegativeFunction() noexcept {

    cnn_.makeNegative(processed_.data(), processed_.getHeight(), processed_.getWidth());

    processed_.setData(cnn_.getOutputData());
}

uint32_t Controller::width() const noexcept {
    return original_.getHeight();
}

uint32_t Controller::height() const noexcept {
    return original_.getWidth();
}

bool Controller::hasImage() const noexcept {
    return (original_.getHeight() != 0);
}

const Bmp<Rgba>& Controller::originalImage() const {
    return original_;
}

const Bmp<Rgba>& Controller::processedImage() const {
    return processed_;
}