#include "reader_class.h"

#pragma once

/**
 * @file process.h
 * @brief В этом файле находится функции для обработки изображений без использования фильтров
 * @author Tarasova Alina
 */

/**
 * @brief функция, создающая ЧБ изображение
 * @param bmp объект класс Bmp, содержащий считанное изображение
 */
void makeBW(Bmp<Rgba>& bmp) noexcept;

/**
 * @brief функция, создающая изображение с одним выбранным цветным каналом
 * @param bmp объект класс Bmp, содержащий считанное изображение
 */
void keepGreenChannel(Bmp<Rgba>& bmp) noexcept;

/**
 * @brief функция, создающая негативное изображение
 * @param bmp объект класс Bmp, содержащий считанное изображение
 */
void makeNegative(Bmp<Rgba>& bmp) noexcept;