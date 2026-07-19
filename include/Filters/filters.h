#ifndef FILTERS_H
#define FILTERS_H

#include "CNN/struct.h"

#include <cstdint>
#include <vector>

/**
 * @file filters.h
 * @brief В этом файле находятся фильтры, которые применяются без использования слоев и фильтров ядра
 * @author Tarasova Alina
 */

/**
 * @enum Filter
 * @brief Методы для обработки изображения
 */

class Filter {
public:
        /**
     * @brief функция, создающая ЧБ изображение
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    std::vector<Rgba> makeBW(const std::vector<Rgba>& data_, int height, int width) noexcept;

    /**
     * @brief функция, создающая изображение с одним выбранным цветным каналом
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    std::vector<Rgba> keepCertainChannel(const std::vector<Rgba>& data_, int height, int width, const Rgba& color) noexcept;

    /**
     * @brief функция, создающая негативное изображение
     * @param data_ вектор пикселей
     * @param length длина входного изображения
     * @param width ширина входного изображения
     */
    std::vector<Rgba> makeNegative(const std::vector<Rgba>& data_, int height, int width) noexcept;
};

#endif