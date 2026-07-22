#ifndef CNNCLASSES_H
#define CNNCLASSES_H


#include <memory>
#include <cstdint>
#include <vector>

#include "struct.h"

/**
 * @file CNN_kernel.h
 * @brief В этом файле находятся классы сверточного слоя (CNN) и ядра фильтра (kernel filter)
 * @author Tarasova Alina
 * 
 * @section design_patterns
 * 
 * 1. @b Стратегия (Strategy)
 *    - Классы: IFilterStrategy, ConvolutionFilter, GradientFilter
 * 
 * 2. @b Фабрика (Factory)
 *    - Класс: FilterFactory
 * 
 * 3. @b Шаблонный метод (Template Method)
 *    - Классы: ConvolutionFilter, GradientFilter
 * 
 * 4. @b Внедрение зависимости (Dependency Injection)
 *    - Класс: CNN
 */

#define STANDART_KERNEL_SIZE 3

/**
 * @enum CONVOLUTION_FILTER
 * @brief Фильтры для обработки изображения
 */
enum CONVOLUTION_FILTER {
    Emboss,          ///< Барельеф - создает эффект тиснения
    Sharpen,         ///< Повышение резкости - усиливает перепады яркости
    BoxBlur,         ///< Блочное размытие - усреднение по квадратной области
    GaussianBlur,    ///< Гауссово размытие - размытие с весовыми коэффициентами
    LaplacianFilter, ///< Фильтр Лапласа - выделение границ (вторая производная)
    PrewittFilter    ///< Фильтр Прюитта - выделение границ по направлению
};

/**
 * @class Kernel
 * @brief Класс ядра, который используется сетью для обработки изображения
 * @details Стандартно фильтр задается размером 3x3 с несколькими вариантами для обработки.
 *          При установлении собственного размера дополнительные методы для
 *          установки значений данных ядра не предусмотрены
 */
class Kernel {
public:
    /**
     * @brief Конструктор по умолчанию. Создает ядро размером 3x3 с нулевыми коэффициентами
     */
    Kernel() noexcept;

    /**
     * @brief Конструктор с параметрами
     * @param size Размер ядра (size x size)
     * @param kernel_data Вектор данных ядра размером size*size
     */
    explicit Kernel(int size, std::vector<float> kernel_data) noexcept;

    int k_size;                          ///< Размер ядра (количество элементов по стороне)
    int p_w, p_h;                        ///< Ширина и высота отступов (обычно k_size/2)
    std::vector<float> kernel_data_{};   ///< Вектор данных фильтра (значения коэффициентов)
};

/**
 * @class IFilterStrategy
 * @brief Абстрактный класс общего интерфейса для всех фильтров
 * @details Реализует паттерн "Стратегия" для гибкой замены алгоритмов фильтрации
 */
class IFilterStrategy {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IFilterStrategy() = default;

    /**
     * @brief Применяет фильтр к изображению
     * @param data Входное изображение в формате RGBA
     * @param height Высота изображения
     * @param width Ширина изображения
     * @return Обработанное изображение
     */
    virtual std::vector<Rgba> apply(const std::vector<Rgba>& data, int height, int width) const = 0;

    /**
     * @brief Устанавливает размер ядра фильтра
     * @param size Новый размер ядра (size x size)
     */
    virtual void setKernelSize(int size) noexcept = 0;

    /**
     * @brief Устанавливает коэффициенты ядра фильтра
     * @param kernel Вектор коэффициентов размером size*size
     */
    virtual void setKernel(const std::vector<float>& kernel) noexcept = 0;
};

/**
 * @class ConvolutionFilter
 * @brief Класс для всех фильтров кроме Фильтра Прюитта
 * @details Реализует стандартную свертку с одним ядром
 */
class ConvolutionFilter : public IFilterStrategy {
public:
    /**
     * @brief Конструктор по умолчанию. Создает фильтр с ядром 3x3
     */
    ConvolutionFilter() noexcept;

    /**
     * @brief Конструктор с параметрами
     * @param size Размер ядра
     * @param kernel_data Коэффициенты ядра
     */
    ConvolutionFilter(int size, std::vector<float> kernel_data) noexcept;

    /**
     * @brief Применяет свертку с ядром к изображению
     * @param data Входное изображение
     * @param height Высота изображения
     * @param width Ширина изображения
     * @return Результат свертки
     */
    std::vector<Rgba> apply(const std::vector<Rgba>& data, int height, int width) const override;

    /**
     * @brief Устанавливает размер ядра
     * @param size Новый размер
     */
    void setKernelSize(int size) noexcept override;

    /**
     * @brief Устанавливает коэффициенты ядра
     * @param kernel Новые коэффициенты
     */
    void setKernel(const std::vector<float>& kernel) noexcept override;

private:
    /**
     * @brief Обработка отдельного участка изображения с применением выбранного фильтра
     * @param output_data_ Выходное изображение (результат записывается сюда)
     * @param data_ Входное изображение (исходные данные)
     * @param h Индекс по высоте (верхний левый угол ядра)
     * @param w Индекс по ширине (верхний левый угол ядра)
     * @param width Ширина входного изображения
     */
    void proccesingParticularSpace(std::vector<Rgba>& output_data_, const std::vector<Rgba>& data_, int h, int w, int width) const noexcept;

    Kernel kernel_;  ///< Ядро фильтра (коэффициенты свертки)
};

/**
 * @class GradientFilter
 * @brief Класс для реализации Фильтра Прюитта
 * @details Использует два ядра (по X и Y) для вычисления градиента
 */
class GradientFilter : public IFilterStrategy {
public:
    /**
     * @brief Конструктор. Инициализирует ядра Прюитта
     */
    GradientFilter();

    /**
     * @brief Применяет фильтр Прюитта к изображению
     * @param data Входное изображение
     * @param height Высота изображения
     * @param width Ширина изображения
     * @return Изображение с выделенными границами
     */
    std::vector<Rgba> apply(const std::vector<Rgba>& data, int height, int width) const override;

private:
    /**
     * @brief Обработка отдельного участка изображения с применением выбранного фильтра
     * @param output_data_ Выходное изображение
     * @param data_ Входное изображение
     * @param h Индекс по высоте
     * @param w Индекс по ширине
     * @param width Ширина входного изображения
     * @param type Тип фильтра: 0 - по X, 1 - по Y
     */
    void proccesingParticularSpace(std::vector<Rgba>& output_data_, const std::vector<Rgba>& data_, int h, int w, int width, int type) const noexcept;

    /**
     * @brief Устанавливает размер ядра (для обоих ядер)
     * @param size Новый размер
     */
    void setKernelSize(int size) noexcept override;

    /**
     * @brief Устанавливает коэффициенты ядра (для обоих ядер)
     * @param kernel Новые коэффициенты
     */
    void setKernel(const std::vector<float>& kernel) noexcept override;

    Kernel gx_;  ///< Ядро фильтра по оси X (горизонтальные границы)
    Kernel gy_;  ///< Ядро фильтра по оси Y (вертикальные границы)
};

/**
 * @class CNN
 * @brief Класс сверточного слоя, который используется для применения фильтров 
 *        при обработке изображения
 */
class CNN {
public:
    /**
     * @brief Устанавливает фильтр для обработки
     * @param filter Умный указатель на стратегию фильтрации
     */
    void setFilter(std::unique_ptr<IFilterStrategy> filter);

    /**
     * @brief Запускает процесс фильтрации изображения
     * @param data Входное изображение
     * @param height Высота изображения
     * @param width Ширина изображения
     */
    void proccesingImage(const std::vector<Rgba>& data, int height, int width) noexcept;

    /**
     * @brief Возвращает результат обработки
     * @return Константная ссылка на выходное изображение
     */
    const std::vector<Rgba>& getOutputData() const noexcept;

    /**
     * @brief Устанавливает размер ядра для текущего фильтра
     * @param size Новый размер ядра
     */
    void setKernelSize(int size) noexcept;

    /**
     * @brief Устанавливает коэффициенты ядра для текущего фильтра
     * @param kernel Вектор коэффициентов
     */
    void setKernel(const std::vector<float>& kernel) noexcept;

private:
    std::unique_ptr<IFilterStrategy> filter_;  ///< Умный указатель на объект фильтра
    std::vector<Rgba> output_data_{};          ///< Вектор выходных данных (результат фильтрации)
};

/**
 * @class FilterFactory
 * @brief Класс фабрики, возвращающий класс слоя с нужным фильтром
 * @details Реализует паттерн "Фабрика" для создания фильтров по типу
 */
class FilterFactory {
public:
    /**
     * @brief Создает фильтр заданного типа
     * @param type Тип фильтра (из перечисления CONVOLUTION_FILTER)
     * @param size Размер ядра (по умолчанию 3)
     * @return Умный указатель на созданный фильтр
     */
    static std::unique_ptr<IFilterStrategy> create(CONVOLUTION_FILTER type, int size = 3);
};

#endif // CNN_KERNEL_H