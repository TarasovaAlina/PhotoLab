#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 * @file Controller.h
 * @brief В этом файле находится класс контроллера, осуществляющий передачу данных между qt и CNN.
 * @author Tarasova Alina
 */

#include "reader_class.h"
#include "CNN/CNN_kernel.h"

#include <iostream>

class Controller {
public:

    Controller() noexcept;

    /**
     * @brief Загружает BMP-изображение.
     * @param filename путь к файлу.
     * @return true, если файл успешно загружен.
     */
    bool loadFile(const std::string& filename) noexcept(false);

    /**
     * @brief Сохраняет обработанное изображение в BMP-файл.
     * @param filename путь к файлу.
     * @return true, если файл успешно сохранён.
     */
    bool saveFile(const std::string& filename) noexcept(false);

    /**
     * @brief Устанавливает фильтр свёртки.
     * @param filter тип фильтра.
     */
    void setFilter(CONVOLUTION_FILTER filter) noexcept;

    /**
     * @brief Устанавливает размер ядра свёртки.
     * @param size размер стороны ядра.
     */
    void setKernelSize(int size) noexcept;

    /**
     * @brief Устанавливает пользовательское ядро свёртки.
     * @param kernel одномерный массив коэффициентов ядра.
     */
    void setKernel(const std::vector<float>& kernel) noexcept;

    /**
     * @brief Применяет выбранный фильтр свёртки к изображению.
     */
    void processImage();

    /**
     * @brief Применяет выбранный фильтр свёртки к изображению (перегрузка для фильтра Прюитта).
     */
    void processImage(bool filter);

    /**
     * @brief Восстанавливает обработанное изображение до исходного состояния.
     */
    void clearImage();

    /**
     * @brief Преобразует изображение в оттенки серого.
     */
    void makeBWFunction() noexcept;

    /**
     * @brief Выполняет тонирование изображения выбранным цветом.
     * @param color цвет тонирования.
     */
    void keepCertainChannelFunction(const Rgba& color) noexcept;

    /**
     * @brief Создаёт негатив изображения.
     */
    void makeNegativeFunction() noexcept;

    /**
     * @brief Возвращает исходное изображение.
     * @return Константная ссылка на исходное изображение.
     */
    const Bmp<Rgba>& originalImage() const;

    /**
     * @brief Возвращает обработанное изображение.
     * @return Константная ссылка на обработанное изображение.
     */
    const Bmp<Rgba>& processedImage() const;

    /**
     * @brief Возвращает ширину изображения.
     * @return Ширина изображения в пикселях.
     */
    uint32_t width() const noexcept;

    /**
     * @brief Возвращает высоту изображения.
     * @return Высота изображения в пикселях.
     */
    uint32_t height() const noexcept;

    /**
     * @brief Проверяет, загружено ли изображение.
     * @return true, если изображение загружено.
     */
    bool hasImage() const noexcept;

private:
    Bmp<Rgba> original_; ///< Исходное изображение.
    Bmp<Rgba> processed_; ///< Обработанное изображение.

    CNN cnn_; ///< Объект обработки изображения.
};

#endif