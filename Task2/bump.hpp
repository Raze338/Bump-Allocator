#pragma once

#include <iostream>

template <size_t size>
class Bump {
public:
    Bump() {
        heap_size = size;
        init();
    }


    ~Bump() {
        delete[] heap;
    }


    template <class T>
    T* alloc(size_t n) {
        size_t address = reinterpret_cast<size_t>(next);
        size_t current_alignment = alignof(T);  // Default alignment size
        size_t padding = (current_alignment - (address % current_alignment)) % current_alignment;

        // Check if alignment is satisfied
        if (heap + heap_size - next < (sizeof(T) * n) + padding) {
            return nullptr;
        }
        
        char* new_alignment = next + padding;
        T* ptr = reinterpret_cast<T*>(new_alignment);
        next = new_alignment + (sizeof(T) * n);
        heap_used += sizeof(T) * n + padding;
        return ptr;
    }

    bool dealloc() {
        delete[] heap;
        init();
        return true;
    }

private:
    void init() {
        heap = new char[heap_size];
        heap_used = 0;
        next = heap;
    }

    char* heap;
    char* next;
    size_t heap_size;
    size_t heap_used;
};