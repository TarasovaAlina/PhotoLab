#pragma once

#include <vector>
#include <iostream>
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
 * @struct Rgba
 * @brief Содержит цвета в формате RGB и параметр прозрачности alpha
 */

struct Rgba {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha{255};
};

/**
 * @class Bmp
 * @brief Шаблонный класс, описывающий основные методы
 * по загрузке и сохранению .bmp файла
 */
template <typename Pixel = Rgba>
class Bmp {
public:

    using PixelType = Pixel; 

    Bmp(uint32_t width, uint32_t height) noexcept(false);
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

private:

    /**
     * @brief проверка на корректность скачивания/создания файла .bmp 
     * @return флаг успешного считывания/открытия файла 
     */
    bool validate() const noexcept;

    /**
     * @brief расчет длины строки с учетом padding
     * @param pixelsNum текущая ширина
     * @return ширина с учетом padding
     */
    uint32_t calculateBytesPerRowWithPadding(uint32_t pixelsNum) const noexcept;

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

    std::vector<Rgba> m_data; ///<  вектор полученных данных в формате RGBA; будет корректироваться с учетом других форматов

    inline static const uint16_t bmpSignature{0x4D42}; ///< BM в 16й системе
    inline static const uint32_t defaultPixelsPerMeter{3780}; ///< рандомное значение
    inline static const uint32_t padding{0}; ///< padding для 32-битового .bmp; будет корректироваться с учетом других форматов
};

template<typename Pixel>
Bmp<Pixel>::Bmp(uint32_t width, uint32_t height) noexcept(false) {
    const auto bytesPerRow{ (width * Pixel::bitsPerPixel) / CHAR_BIT };
    const auto bytesPerRowWithPadding{ calculateBytesPerRowWithPadding(width) };
    const auto dataOffset{ sizeof(BmpHeader) + sizeof(InfoHeader) };
    const auto imageSize{ bytesPerRowWithPadding * height };
    m_header.fileSize = dataOffset + imageSize;
    m_header.dataOffset = dataOffset;
    m_infoHeader.width = width;
    m_infoHeader.height = height;
    m_infoHeader.imageSize = imageSize;
    m_data.resize(imageSize, 0);
    clear();
}

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
        m_data.resize(imageSize);

        if (!file.read(reinterpret_cast<char*>(m_data.data()), m_infoHeader.imageSize)) {
            throw std::runtime_error{ "Failed to read file" };
        }



        file.close();
        // return validate();
        return true;
    }
    return false;
}

template<typename Pixel>
bool Bmp<Pixel>::saveFile(const std::string& filename) noexcept(false) {

    std::cout
    << sizeof(BmpHeader) << '\n'
    << sizeof(InfoHeader) << '\n'
    << m_header.dataOffset << '\n'
    << m_header.fileSize << '\n'
    << m_infoHeader.imageSize << '\n'
    << m_data.size() << '\n';

    std::cout << "width = " << m_infoHeader.width << '\n';
    std::cout << "height = " << m_infoHeader.height << '\n';
    std::cout << "bits = " << m_infoHeader.bitsPerPixel << '\n';

    m_infoHeader.bitsPerPixel = 32;

    std::ofstream file{ filename, std::ios::binary };
    if (file) {
        if (!file.write(reinterpret_cast<char*>(&m_header), sizeof(BmpHeader))) {
            throw std::runtime_error{ "Failed to read file" };
        }
        if (!file.write(reinterpret_cast<char*>(&m_infoHeader), sizeof(InfoHeader))) {
            throw std::runtime_error{ "Failed to read file" };
        }

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
    const auto reversedRow{ m_infoHeader.height - row - 1 };
    return m_data[reversedRow * m_infoHeader.width + column]; 
}

template<typename Pixel>
bool Bmp<Pixel>::validate() const noexcept {
    return (m_header.signature == bmpSignature
        && m_infoHeader.planes == 1
        && m_infoHeader.compression == 0);
}

template<typename Pixel>
uint32_t Bmp<Pixel>::calculateBytesPerRowWithPadding(uint32_t pixelsNum) const noexcept {
    const auto bytesNum{ pixelsNum * m_infoHeader.bitsPerPixel / CHAR_BIT };
    const auto nearestDivisibleByPadding{((bytesNum + 3) / 4) * 4};
    return nearestDivisibleByPadding;
}
