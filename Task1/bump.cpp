#include "bump.hpp"
#include <iostream>

template <typename T> Bump<T>::Bump(size_t size) {
    heap = new char[size];
    heap_size = size;
    heap_used = 0;
    alloc_count = 0;
}

template <typename T> Bump<T>::~Bump() {
    delete[] heap;
}

template <typename > T *Bump<T>::alloc(size_t n) {
    if (heap_used + n * sizeof(T) > heap_size) {
        return nullptr;
    }
    T *ptr = reinterpret_cast<T *>(heap + heap_used);
    heap_used += n * sizeof(T);
    alloc_count++;
    std::cout << "Allocated " << n << " elements at address " << static_cast<void*>(ptr) << std::endl;
    return ptr;
}

template <typename T> void Bump<T>::dealloc(T *ptr) {
    if (ptr == nullptr) {
        return;
    }
    if (--(alloc_count) == 0) {
        std::cout << "Deallocated all memory." << std::endl;
        reset();
    }
    std::cout << "Deallocated memory at address " << static_cast<void*>(ptr) << std::endl;
}

template <typename T> void Bump<T>::reset() {
    heap_used = 0;
}

