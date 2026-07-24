#ifndef STRUCT_H
#define STRUCT_H

#include <cstdint>
#include <type_traits>

/**
 * @file struct.h
 * @brief В этом файле находится шаблонная структура для работы с пикселями изображения в формате RGBA
 * @author Tarasova Alina
 */

/**
 * @struct RGBA_pattern
 * @brief Шаблон пикселя RGBA. T=uint8_t — обычный пиксель изображения (Rgba),
 * T=float — "рабочий" пиксель для промежуточных вычислений (temp_Rgba),
 * где не нужно терять точность на каждой операции.
 */

template <typename T>
struct RGBA_pattern {
    T blue{};
    T green{};
    T red{};
    T alpha{};

    RGBA_pattern& operator=(int num);

    RGBA_pattern& operator+=(int num);

    RGBA_pattern& operator+=(RGBA_pattern temp);

    template <typename U>
    RGBA_pattern& operator=(const RGBA_pattern<U>& temp);

    RGBA_pattern<float> operator*(float num) const;

    RGBA_pattern& operator*=(float num);

    bool operator==(const RGBA_pattern& temp) const;

    /**
     * @brief устанавливает значение цветового канала в необходимом диапозоне (0-255)
     * @param val значение цветового канала
     * @return измененное значение
     */
    T setValue(float val) const;
};

template<typename T>
RGBA_pattern<T>& RGBA_pattern<T>::operator=(int num) {
        blue = static_cast<T>(num);
        green = static_cast<T>(num);
        red = static_cast<T>(num);
        alpha = static_cast<T>(num);

        return *this;
}

template<typename T>
RGBA_pattern<T>& RGBA_pattern<T>::operator+=(int num) {
    blue += static_cast<T>(num);
    green += static_cast<T>(num);
    red += static_cast<T>(num);
    alpha += static_cast<T>(num);
    return *this;
}

template<typename T>
RGBA_pattern<T>& RGBA_pattern<T>::operator+=(RGBA_pattern<T> temp) {
    blue += temp.blue;
    green += temp.green;
    red += temp.red;
    alpha += temp.alpha;
    return *this;
}

template<typename T>
template<typename U>
RGBA_pattern<T>& RGBA_pattern<T>::operator=(const RGBA_pattern<U>& temp) {
    if constexpr (std::is_integral_v<T>) {
        blue = setValue(static_cast<float>(temp.blue));
        green = setValue(static_cast<float>(temp.green));
        red = setValue(static_cast<float>(temp.red));
        alpha = setValue(static_cast<float>(temp.alpha));
    } else {
        blue = static_cast<T>(temp.blue);
        green = static_cast<T>(temp.green);
        red = static_cast<T>(temp.red);
        alpha = static_cast<T>(temp.alpha);
    }
    return *this;
}

template<typename T>
RGBA_pattern<float> RGBA_pattern<T>::operator*(float num) const {
    RGBA_pattern<float> res;
    res = *this;
    res *= num;
    return res;
}

template<typename T>
RGBA_pattern<T>& RGBA_pattern<T>::operator*=(float num) {
    blue = static_cast<float>(blue) * num;
    green = static_cast<float>(green) * num;
    red = static_cast<float>(red) * num;
    alpha = static_cast<float>(alpha) * num;
    return *this;
}

template<typename T>
T RGBA_pattern<T>::setValue(float val) const {
    if (val > 255.0f) return static_cast<T>(255);
    if (val < 0.0f) return static_cast<T>(0);
    return static_cast<T>(val);
}

template<typename T>
bool RGBA_pattern<T>::operator==(const RGBA_pattern& temp) const {
    bool res{true};
    if (blue != temp.blue ||
        green != temp.green ||
        red != temp.red ||
        alpha != temp.alpha) res = false;

    return res;
}

using Rgba = RGBA_pattern<uint8_t>;

using temp_Rgba = RGBA_pattern<float>;

#endif
