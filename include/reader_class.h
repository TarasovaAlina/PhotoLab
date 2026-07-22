#ifndef READERCLASS_H
#define READERCLASS_H

#include "CNN/struct.h"
#include "CNN/CNN_kernel.h"

#include <vector>
#include <climits>
#include <cstdint>
#include <stdexcept>
#include <fstream>

/**
 * @file reader_class.h
 * @brief В этом файле находится шаблонный класс для считывания и загрузки файлов в формате bpm (сейчас поддерживается 32-битная версия)
 * @author Tarasova Alina
 */

/**
 * @class Bmp
 * @brief Шаблонный класс, описывающий основные методы
 * по загрузке и сохранению .bmp файла
 */
template <typename Pixel = Rgba>
class Bmp {
public:

    using PixelType = Pixel; 

    Bmp() = default;
    Bmp(const std::string& filename) noexcept(false);

    /**
     * @brief загрузка файла .bmp 
     * @param filename имя файла (в данный момент считывается из командной строки)
     * @return флаг успешного считывания/открытия файла 
     */
    bool loadFile(const std::string& filename) noexcept(false);

    /**
     * @brief скачивание/создание файла .bmp 
     * @param filename имя файла (в данный момент считывается из командной строки)
     * @return флаг успешного считывания/открытия файла 
     */
    bool saveFile(const std::string& filename) noexcept(false);

    /**
     * @brief геттер для получения ширины изображения
     * @return ширина изображения
     */
    uint32_t getWidth() const noexcept;

    /**
     * @brief геттер для получения длины изображения
     * @return длина изображения
     */
    uint32_t getHeight() const noexcept;

    void setData(const std::vector<Rgba>& data) noexcept;

    /**
     * @brief Метод для очищения (зануления) вектора пикселей
     */
    void clear() noexcept;

    /**
     * @brief оператор для получения пикселя
     * @param row строка изображения
     * @param column колонка изображения
     * @return пиксель
     */
    PixelType& operator()(uint32_t row, uint32_t column) noexcept(false);

    /**
     * @brief оператор для получения пикселя (константная ссылка)
     * @param row строка изображения
     * @param column колонка изображения
     * @return константный пиксель
     */
    PixelType const& operator()(uint32_t row, uint32_t col) const noexcept(false); 

    /**
     * @brief Геттер для получения вектора пикселей в формате RGBA
     * @return Вектор пикселей в формате RGBA
     */
    const std::vector<Rgba>& data() const noexcept;

    /**
     * @brief Метод для разворота строк пикселей по высоте
     */
    void reverseData() noexcept;

private:

    /**
     * @brief расчет длины строки с учетом padding
     * @param pixelsNum текущая ширина
     * @return ширина с учетом padding
     */
    uint32_t calculateBytesPerRowWithPadding(uint32_t pixelsNum) const noexcept;

    /**
     * @brief перевод 24-bit формата в 32-bit при необходимости
     * @param file файловый поток
     */
    void setBitFormat(std::ifstream& file) noexcept(false);

#pragma pack(push, 2)
    struct BmpHeader {
        uint16_t signature{bmpSignature};
        uint32_t fileSize{};
        uint32_t reserved{};
        uint32_t dataOffset{};
    } m_header; ///< заголовок BITMAPFILEHEADER
#pragma pack(pop)

    struct InfoHeader {
        uint32_t size{sizeof(InfoHeader)};
        uint32_t width{1};
        uint32_t height{1};
        uint16_t planes{1};
        uint16_t bitsPerPixel{1};
        uint32_t compression{};
        uint32_t imageSize{};
        uint32_t xPixelsPerM{defaultPixelsPerMeter};
        uint32_t yPixelsPerM{defaultPixelsPerMeter};
        uint32_t colorUsed{};
        uint32_t colorImportans{};
    } m_infoHeader; ///< заголовок BITMAPINFO

    std::vector<uint8_t> file_data; ///< вектор полученных 24-bit bmp данных для перевода в RGBA
    std::vector<Rgba> m_data; ///<  вектор полученных данных в формате RGBA

    inline static const uint16_t bmpSignature{0x4D42}; ///< BM в 16й системе
    inline static const uint32_t defaultPixelsPerMeter{3780}; ///< рандомное значение 
};

template<typename Pixel>
Bmp<Pixel>::Bmp(const std::string& filename) noexcept(false) {
    if (!loadFile(filename)) {
        throw std::runtime_error{ "Failed to read file" };
    }
}

template<typename Pixel>
bool Bmp<Pixel>::loadFile(const std::string& filename) noexcept(false) {
    std::ifstream file{ filename, std::ios::binary };
    if (file) {
        if (!file.read(reinterpret_cast<char*>(&m_header), sizeof(BmpHeader))) {
            throw std::runtime_error{ "Failed to read file" };
        }
        if (!file.read(reinterpret_cast<char*>(&m_infoHeader), sizeof(InfoHeader))) {
            throw std::runtime_error{ "Failed to read file" };
        }

        const auto imageSize = calculateBytesPerRowWithPadding(m_infoHeader.width) * m_infoHeader.height;
        m_infoHeader.imageSize = imageSize;

        m_data.resize(m_infoHeader.imageSize);

        setBitFormat(file);

        file.close();
        return true;
    }
    return false;
}

template<typename Pixel>
bool Bmp<Pixel>::saveFile(const std::string& filename) noexcept(false) {

    m_infoHeader.bitsPerPixel = 32;

    m_infoHeader.imageSize =
        m_infoHeader.width *
        m_infoHeader.height *
        sizeof(Rgba);

    m_header.fileSize =
        m_header.dataOffset +
        m_infoHeader.imageSize;

    std::ofstream file{ filename, std::ios::binary };
    if (file) {
        if (!file.write(reinterpret_cast<char*>(&m_header), sizeof(BmpHeader))) {
            throw std::runtime_error{ "Failed to read file" };
        }
        if (!file.write(reinterpret_cast<char*>(&m_infoHeader), sizeof(InfoHeader))) {
            throw std::runtime_error{ "Failed to read file" };
        }

        reverseData();

        if (!file.write(reinterpret_cast<char*>(m_data.data()), m_infoHeader.imageSize)) {
            throw std::runtime_error{ "Failed to read file" };
        }
        file.close();
        return true;
    }
    return false;
}

template<typename Pixel>
uint32_t Bmp<Pixel>::getWidth() const noexcept {
    return m_infoHeader.width;
}

template<typename Pixel>
uint32_t Bmp<Pixel>::getHeight() const noexcept {
    return m_infoHeader.height;
}

template<typename Pixel>
void Bmp<Pixel>::setData(const std::vector<Rgba>& data) noexcept {
    m_data = data;
}

template<typename Pixel>
void Bmp<Pixel>::clear() noexcept {
    for (uint32_t i{}; i < m_infoHeader.height; ++i) {
        for (uint32_t j{}; j < m_infoHeader.width; ++j) {
            (*this)(i, j) = Pixel{};
        }
    }
}

template<typename Pixel>
Bmp<Pixel>::PixelType& Bmp<Pixel>::operator()(uint32_t row, uint32_t column) noexcept(false) {
    if (row >= m_infoHeader.height || column >= m_infoHeader.width) {
        throw std::out_of_range{ "Out of range" };
    }
    return m_data[row * m_infoHeader.width + column];
}

template<typename Pixel>
Bmp<Pixel>::PixelType const& Bmp<Pixel>::operator()(uint32_t row, uint32_t column) const noexcept(false) {
    if (row >= m_infoHeader.height || column >= m_infoHeader.width) {
        throw std::out_of_range{ "Out of range" };
    }
    return m_data[row * m_infoHeader.width + column];
}

template<typename Pixel>
const std::vector<Rgba>& Bmp<Pixel>::data() const noexcept {
    return m_data;
}

template<typename Pixel>
uint32_t Bmp<Pixel>::calculateBytesPerRowWithPadding(uint32_t pixelsNum) const noexcept {
    const auto bytesNum{ pixelsNum * m_infoHeader.bitsPerPixel / CHAR_BIT };
    const auto nearestDivisibleByPadding{((bytesNum + 3) / 4) * 4};
    return nearestDivisibleByPadding;
}

template<typename Pixel>
void Bmp<Pixel>::setBitFormat(std::ifstream& file) noexcept(false) {

    if (m_infoHeader.bitsPerPixel == 24) {
        file_data.resize(m_infoHeader.imageSize);
        if (!file.read(reinterpret_cast<char*>(file_data.data()), m_infoHeader.imageSize)) {
            throw std::runtime_error{ "Failed to read file" };
        }
        int j{};
        for (int i{}; i < file_data.size() - 3; i += 3) {
            m_data[j].blue = file_data[i];
            m_data[j].green = file_data[i + 1];
            m_data[j].red = file_data[i + 2];
            j++;
        }
        
    } else {
        if (!file.read(reinterpret_cast<char*>(m_data.data()), m_infoHeader.imageSize)) {
            throw std::runtime_error{ "Failed to read file" };
        }
    }
    reverseData();
}

template<typename Pixel>
void Bmp<Pixel>::reverseData() noexcept {
    for (uint32_t row = 0; row < m_infoHeader.height / 2; ++row) {
        auto first = m_data.begin() + row * m_infoHeader.width;
        auto second = m_data.begin() + (m_infoHeader.height - row - 1) * m_infoHeader.width;

        std::swap_ranges(first,
                        first + m_infoHeader.width,
                        second);
    }
}

#endif