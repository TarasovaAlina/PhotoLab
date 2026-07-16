#ifndef CNNCLASSES_H
#define CNNCLASSES_H

#include "struct.h"

#include <cstdint>
#include <vector>

/**
 * @file CNN_kernel.h
 * @brief В этом файле находятся классы сверточного слоя (CNN) и ядра фильтра (kernel filter)
 * @author Tarasova Alina
 */

#define STANDART_KERNEL_SIZE 3

/**
 * @enum CONVOLUTION_FILTER
 * @brief Фильтры для обработки изображения
 */
enum CONVOLUTION_FILTER {
    Emboss, ///< Барельеф 
    Sharpen, ///< Повышение резкости
    BoxBlur, ///< Блочное размытие
    GaussianBlur, ///< Гауссово размытие
    LaplacianFilter, ///< Фильтр Лапласа
    PrewittFilter, ///< Фильтр Прюитта
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
     * @brief функция, создающая ЧБ изображение
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    void makeBW(const std::vector<Rgba>& data_, int height, int width) noexcept;

    /**
     * @brief функция, создающая изображение с одним выбранным цветным каналом
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    void keepCertainChannel(const std::vector<Rgba>& data_, int height, int width, const Rgba& color) noexcept;

    /**
     * @brief функция, создающая негативное изображение
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    void makeNegative(const std::vector<Rgba>& data_, int height, int width) noexcept;

    /**
     * @brief обработка изображения с применением выбранного фильтра
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    void proccesingImage(const std::vector<Rgba>& data_, int length, int width) noexcept;

    /**
     * @brief обработка изображения с применением выбранного фильтра
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     * @param filter выбранный фильтер (перегрузка для фильтра Прюитта)
     */
    void proccesingImage(const std::vector<Rgba>& data_, int length, int width,  CONVOLUTION_FILTER filter) noexcept;

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
    void setLaplacianFilter() noexcept;

    /**
     * @brief установка фильтра Прюитта по горизонтали
     */
    void setPrewittFilterX() noexcept;

    /**
     * @brief установка фильтра Прюитта по вертикали
     */
    void setPrewittFilterY() noexcept;

    /**
     * @brief геттер для выходных данных (выходного изображения после обработки фильтром)
     * @return вектор выходных значений в формате Rgba
     */
    const std::vector<Rgba>& getOutputData() const noexcept;

    /**
     * @brief установка нового размера ядра с пересчетом отступов
     * @param size размер ядра
     */
    void setKernelSize(int size) noexcept;

    /**
     * @brief установка вектора данных ядра фильтра
     * @param kernel данные фильтра
     */
    void setKernel(const std::vector<float>& kernel) noexcept;

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

    /**
     * @brief обработка отдельного участка изображения 
     * с применением выбранного фильтра
     * @param data_ имя файла
     * @param output выходной массив
     * @param h индекс по высоте
     * @param w индекс по ширине
     * @param width ширина входного изображения
     */
    void proccesingParticularSpace(const std::vector<Rgba>& data_, std::vector<Rgba>& output, int h, int w, int width) noexcept;
};

#endif