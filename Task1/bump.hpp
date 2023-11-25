#pragma once

#include <iostream>

template <size_t size>
class Bump
{
public:
    Bump()
    {
        heap_size = size;
        heap = new char[heap_size];
        next = heap;
        heap_used = 0;
    }

    template <class T>
    T *alloc(size_t n)
    {
        size_t address = reinterpret_cast<size_t>(next);
        size_t current_alignment = alignof(T);  // Default alignment size
        size_t padding = (current_alignment - (address % current_alignment)) % current_alignment;

        // Check if alignment is satisfied
        if (heap + heap_size - next < (sizeof(T) * n) + padding)
        {
            return nullptr;
        }
        
        char* new_alignment = next + padding;
        T* ptr = reinterpret_cast<T*>(new_alignment);
        next = new_alignment + (sizeof(T) * n);
        heap_used += sizeof(T) * n;
        std::cout << "Allocated " << n << " elements at address " << reinterpret_cast<uintptr_t>(next) << std::endl;
        std::cout << "Heap used is: " << heap_used << std::endl;
        std::cout << "The next available memory address is: " << reinterpret_cast<uintptr_t>(next) << std::endl;
        return ptr;
    }

    void dealloc()
    {
        reset();
        std::cout << "Deallocated all memory." << std::endl;
        std::cout << "Deallocated memory at address " << static_cast<void *>(heap) << std::endl;
    }

private:
    void reset()
    {
        delete[] heap;
        heap = new char[heap_size];
        heap_used = 0;
        next = heap;
    }


    char *heap;
    char *next;
    size_t heap_size;
    size_t heap_used;
};