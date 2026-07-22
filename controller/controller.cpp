#include "controller/controller.h"

Controller::Controller() noexcept: 
    original_{},  processed_{} , cnn_{}, filter_{} {}

bool Controller::loadFile(const std::string& filename) noexcept(false) {
    original_.loadFile(filename);
    processed_ = original_;
    return true;
}

bool Controller::saveFile(const std::string& filename) noexcept(false) {
    processed_.saveFile(filename);
    return true;
}

void Controller::setFilter(int filter) noexcept
{
    cnn_.setFilter(FilterFactory::create(static_cast<CONVOLUTION_FILTER>(filter - 1)));
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


void Controller::clearImage() {
    processed_.setData(original_.data());
}

void Controller::makeBWFunction() noexcept {

    processed_.setData(
        filter_.makeBW(processed_.data(), processed_.getHeight(), processed_.getWidth())
    );
}

void Controller::keepCertainChannelFunction(const Rgba& color) noexcept {

    processed_.setData(
        filter_.keepCertainChannel(processed_.data(), processed_.getHeight(), processed_.getWidth(), color)
    );
}

void Controller::makeNegativeFunction() noexcept {

    processed_.setData(
        filter_.makeNegative(processed_.data(), processed_.getHeight(), processed_.getWidth())
    );
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