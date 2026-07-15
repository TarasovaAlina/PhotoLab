#ifndef CNNCLASSES_H
#define CNNCLASSES_H

#include "struct.h"

#include <cstdint>
#include <vector>

/**
 * @file CNN_kernel.h
 * @brief В этом файле находится классы сверточного слоя (CNN) и ядра фильтра (kernel filter)
 * @author Tarasova Alina
 */

#define MAX_KERNEL_SIZE 16
#define STANDART_KERNEL_SIZE 3
#define STRIDE 1

/**
 * @enum CONVOLUTION_FILTER
 * @brief Перечисленные фильтров для обработки изображения
 */
typedef enum CONVOLUTION_FILTER {
    Emboss, ///< Барельеф 
    Sharpen, ///< Повышение резкости
    BoxBlur, ///< Блочное размытие
    GaussianBlur, ///< Гауссово размытие
    LaplacianFilter, ///< Фильтр Лапласа
    PrewittFilter, ///< Фильтр Прюитта
    SobelFilter ///< Фильтр Собеля
};

/**
 * @class Kernel
 * @brief Класс ядра, который используется сетью для обработки изображения
 * Стандартно фильтр задается размером 3x3 с несколькими вариантами для обработки
 * При установлении собственного размера дополнительные методы для
 * установки значений данных ядра не предусмотрены
 */
class Kernel {
public:

    Kernel() noexcept;
    explicit Kernel(int size, std::vector<float> kernek_data) noexcept;

    int k_size; ///< размер ядра
    int p_w, p_h; ///< высота и ширина отступов
    std::vector<float> kernel_data_{}; ///< вектор данных фильтра

};

/**
 * @class CNN
 * @brief Класс сверточного слоя, который используется
 * для применения фильтров в обработки изображения
 */
class CNN {
public:

    explicit CNN() noexcept;
    explicit CNN(int size, std::vector<float> kernel_data) noexcept;

    /**
     * @brief обработка изображения с применением выбранного фильтра
     * @param data_ имя файла
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    void proccesingImage(const std::vector<Rgba>& data_, int length, int width) noexcept;

    /**
     * @brief установка фильтра Барельефа
     */
    void setEmbossFilter() noexcept;

    /**
     * @brief установка фильтра Повышения резкости
     */
    void setSharpenFilter() noexcept;

    /**
     * @brief установка фильтра Блочного размытия
     */
    void setBoxBlurFilter()  noexcept;

    /**
     * @brief установка фильтра Гауссового размытия
     */
    void setGaussianBlurFilter() noexcept;

    /**
     * @brief установка фильтра Лапласа
     */
    void LaplacianFilter() noexcept;

    /**
     * @brief геттер для выходных данных (выходного изображения после обработки фильтром)
     * @return вектор выходных значений в формате Rgba
     */
    const std::vector<Rgba>& getOutputData() const noexcept;

private:

    std::vector<Rgba> output_data_{1}; ///< вектор данных выходного слоя после обработки фильтром

    Kernel kernel_; ///< фильтр ядра

    /**
     * @brief обработка отдельного участка изображения 
     * с применением выбранного фильтра
     * @param data_ имя файла
     * @param h индекс по высоте
     * @param w индекс по ширине
     * @param width ширина входного изображения
     */
    void proccesingParticularSpace(const std::vector<Rgba>& data_, int h, int w, int width) noexcept;
};

#endif