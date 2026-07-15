#include "../include/CNN/CNN_kernel.h"

Kernel::Kernel() noexcept : 
    k_size{STANDART_KERNEL_SIZE}, 
    p_w{STANDART_KERNEL_SIZE - 1}, p_h{STANDART_KERNEL_SIZE - 1}  
    {
        kernel_data_.resize(k_size * k_size);
    }

Kernel::Kernel(int size, std::vector<float> kernek_data) noexcept : 
    k_size{size},
    p_w{size - 1}, p_h{size - 1},
    kernel_data_{kernek_data}
    {}