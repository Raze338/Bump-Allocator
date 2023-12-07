#pragma once

#include <iostream>

template <size_t size>
class Bump_down
{
public:
    Bump_down()
    {
        heap_size = size;
        init();
    }

    ~Bump_down()
    {
        delete[] heap;
    }
    
    template <class T>
    T *alloc(size_t n)
    {
        size_t address = reinterpret_cast<size_t>(next);
        size_t current_alignment = alignof(T); // Default alignment size

        // Calculate padding using bump-down logic
        size_t padding = (address & -current_alignment);

        // Check if alignment is satisfied
        if (next - (sizeof(T) * n) - padding < heap)
        {
            return nullptr;
        }

        char *new_alignment = reinterpret_cast<char*>(padding) - (sizeof(T) * n);
        T *ptr = reinterpret_cast<T *>(new_alignment);
        next = new_alignment;
        heap_used += (address - reinterpret_cast<uintptr_t>(padding)) + sizeof(T) * n;

        // std::cout << "Allocated " << n << " elements at address " << reinterpret_cast<void*>(ptr) << std::endl;
        // std::cout << "Heap used is: " << heap_used << std::endl;
        // std::cout << "The next available memory address is: " << reinterpret_cast<void*>(next) << std::endl;

        return ptr;
    }

    void dealloc()
    {
        delete[] heap;
        init();
        // std::cout << "Deallocated all memory." << std::endl;
        // std::cout << "Deallocated memory at address " << static_cast<void *>(heap) << std::endl;
    }

private:
    void init()
    {
        heap = new char[heap_size];
        heap_used = 0;
        next = heap + heap_size; // Start at the end of the allocated memory
    }

    char *heap;
    char *next;
    size_t heap_size;
    size_t heap_used;
};
