#include <gtest/gtest.h>
#include <type_traits>

#include "CNN/CNN_kernel.h"
#include "Filters/filters.h"
#include "Reader/reader_class.h"
#include <iostream>

TEST(CNNClasses, test_1) {
    std::unique_ptr<IFilterStrategy> test_filer_ = FilterFactory::create(CONVOLUTION_FILTER::Emboss);
    EXPECT_EQ(typeid(*(test_filer_.get())), typeid(ConvolutionFilter));

    test_filer_ = FilterFactory::create(CONVOLUTION_FILTER::BoxBlur);
    EXPECT_EQ(typeid(*(test_filer_.get())), typeid(ConvolutionFilter));

    test_filer_ = FilterFactory::create(CONVOLUTION_FILTER::GaussianBlur);
    EXPECT_EQ(typeid(*(test_filer_.get())), typeid(ConvolutionFilter));

    test_filer_ = FilterFactory::create(CONVOLUTION_FILTER::LaplacianFilter);
    EXPECT_EQ(typeid(*(test_filer_.get())), typeid(ConvolutionFilter));

    test_filer_ = FilterFactory::create(CONVOLUTION_FILTER::Sharpen);
    EXPECT_EQ(typeid(*(test_filer_.get())), typeid(ConvolutionFilter));

    test_filer_ = FilterFactory::create(CONVOLUTION_FILTER::PrewittFilter);
    EXPECT_EQ(typeid(*(test_filer_.get())), typeid(GradientFilter));

    test_filer_ = FilterFactory::create(static_cast<CONVOLUTION_FILTER>(7));
    EXPECT_EQ(typeid(*(test_filer_.get())), typeid(ConvolutionFilter));

    CNN cnn_, cnn_new_;
    Bmp<Rgba> test_bmp_1{};

    EXPECT_TRUE(test_bmp_1.loadFile("../../tests/test_white.bmp"));

    Bmp<Rgba> test_bmp_2 = test_bmp_1;

    cnn_.setFilter(FilterFactory::create(CONVOLUTION_FILTER::Emboss));
    cnn_.proccesingImage(test_bmp_2.data(), test_bmp_1.getHeight(), test_bmp_1.getWidth());

    test_bmp_2.setData(cnn_.getOutputData());
    EXPECT_EQ((test_bmp_2.data()).size(), (test_bmp_1.data()).size());
    EXPECT_EQ(test_bmp_2.getHeight(), test_bmp_1.getHeight());
    EXPECT_EQ(test_bmp_2.getWidth(), test_bmp_1.getWidth());

    cnn_.setFilter(FilterFactory::create(CONVOLUTION_FILTER::PrewittFilter));
    test_bmp_2 = test_bmp_1;
    cnn_.proccesingImage(test_bmp_2.data(), test_bmp_1.getHeight(), test_bmp_1.getWidth());

    test_bmp_2.setData(cnn_.getOutputData());
    EXPECT_EQ((test_bmp_2.data()).size(), (test_bmp_1.data()).size());
    EXPECT_EQ(test_bmp_2.getHeight(), test_bmp_1.getHeight());
    EXPECT_EQ(test_bmp_2.getWidth(), test_bmp_1.getWidth());

    Bmp<Rgba> test_bmp_3{"../../tests/blue-bmp-24-bit.bmp"}, 
        test_bmp_4{"../../tests/blue-bmp-24-bit.bmp"};
    std::vector<float> data{ -2.f, -1.f, 0.f, -1.f, 1.f, 1.f, 0.f, 1.f, 2.f };

    cnn_new_.setKernelSize(3);
    cnn_new_.setKernel(data);
    cnn_new_.proccesingImage(test_bmp_4.data(), test_bmp_4.getHeight(), test_bmp_4.getWidth());

    cnn_.setFilter(FilterFactory::create(CONVOLUTION_FILTER::Emboss));
    cnn_.proccesingImage(test_bmp_3.data(), test_bmp_3.getHeight(), test_bmp_3.getWidth());

    EXPECT_EQ(test_bmp_3.data(), test_bmp_4.data());
}

TEST(FiltersClass, test_2) {
    Filter filter;
    Bmp<Rgba> test_bmp_1{};
    EXPECT_TRUE(test_bmp_1.loadFile("../../tests/test_white.bmp"));

    const auto& data = test_bmp_1.data();
    int width = test_bmp_1.getWidth();
    int height = test_bmp_1.getHeight();
    const Rgba color = {255, 255, 255, 255};

    auto processed_data = filter.makeBW(data, height, width);
    EXPECT_EQ(processed_data.size(), data.size());

    processed_data = filter.keepCertainChannel(data, height, width, color);
    EXPECT_EQ(processed_data.size(), data.size());

    processed_data = filter.makeNegative(data, height, width);
    EXPECT_EQ(processed_data.size(), data.size());
}

TEST(ReaderClasses, test_3) {

    Bmp<Rgba> test_bmp_1{}, test_bmp_2{}, test_bmp_3{};;

    EXPECT_TRUE(test_bmp_1.loadFile("../../tests/test_white.bmp"));
    EXPECT_EQ(test_bmp_1.getWidth(), 150);
    EXPECT_EQ(test_bmp_1.getHeight(), 100);

    for (int i = 0; i < test_bmp_1.getHeight(); ++i) {
        for (int j = 0; j < test_bmp_1.getWidth(); ++j) {
            EXPECT_EQ(static_cast<int>(test_bmp_1(i, j).red), 255);
            EXPECT_EQ(static_cast<int>(test_bmp_1(i, j).green), 255);
            EXPECT_EQ(static_cast<int>(test_bmp_1(i, j).blue), 255);
        }
    }

    EXPECT_TRUE(test_bmp_1.loadFile("../../tests/test_fbceb.bmp"));
    EXPECT_EQ(test_bmp_1.getWidth(), 150);
    EXPECT_EQ(test_bmp_1.getHeight(), 100);

    for (int i = 0; i < test_bmp_1.getHeight(); ++i) {
        for (int j = 0; j < test_bmp_1.getWidth(); ++j) {
            EXPECT_EQ(static_cast<int>(test_bmp_1(i, j).red), 251);
            EXPECT_EQ(static_cast<int>(test_bmp_1(i, j).green), 206);
            EXPECT_EQ(static_cast<int>(test_bmp_1(i, j).blue), 177);
        }
    }

    EXPECT_TRUE(test_bmp_1.saveFile("../../tests/new_image.bmp"));
    EXPECT_TRUE(test_bmp_2.loadFile("../../tests/new_image.bmp"));
    EXPECT_EQ(test_bmp_2.getWidth(), 150);
    EXPECT_EQ(test_bmp_2.getHeight(), 100);

    for (int i = 0; i < test_bmp_1.getHeight(); ++i) {
        for (int j = 0; j < test_bmp_1.getWidth(); ++j) {
            EXPECT_EQ(static_cast<int>(test_bmp_2(i, j).red), 251);
            EXPECT_EQ(static_cast<int>(test_bmp_2(i, j).green), 206);
            EXPECT_EQ(static_cast<int>(test_bmp_2(i, j).blue), 177);
        }
    }

    for (int i = 0; i < test_bmp_1.getHeight(); ++i) {
        for (int j = 0; j < test_bmp_1.getWidth(); ++j) {
            EXPECT_EQ(static_cast<int>(test_bmp_2(i, j).red), 251);
            EXPECT_EQ(static_cast<int>(test_bmp_2(i, j).green), 206);
            EXPECT_EQ(static_cast<int>(test_bmp_2(i, j).blue), 177);
        }
    }

    EXPECT_TRUE(test_bmp_3.loadFile("../../tests/blue-bmp-24-bit.bmp"));
    EXPECT_EQ(test_bmp_3.getWidth(), 512);
    EXPECT_EQ(test_bmp_3.getHeight(), 512);
}