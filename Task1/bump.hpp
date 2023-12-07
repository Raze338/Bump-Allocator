#pragma once

#include <iostream>

template <size_t size>
class Bump {
public:
    // Constructor
    /* Constructor to initialise the bump object 
    with a size, and use the init() function to initialise 
    the rest of the heap */
    Bump() {
        heap_size = size;
        init();
    }

    // Destructor
    /* Destructor to deallocate the heap 
    memory when the object is destroyed */
    ~Bump() {
        delete[] heap;
    }
 
    // alloc function
    /* Function to allocate memory to the heap, calculating required padding 
    to ensure proper alignment, and ensuring if there is enough space in the 
    heap for allocation */
    template <class T>
    T* alloc(size_t n) {
        size_t address = reinterpret_cast<size_t>(next); // Current memory address
        size_t current_alignment = alignof(T);  // Alignment of type 'T'
        size_t padding = (current_alignment - (address % current_alignment)) % current_alignment; // Padding needed to satisfy alignment

        // Check if there is enough space for allocation, return nullptr if failed
        if (heap + heap_size - next < (sizeof(T) * n) + padding) {
            return nullptr;
        }
        
        char* new_alignment = next + padding; // Adjust alignment of 'next'
        T* ptr = reinterpret_cast<T*>(new_alignment); // Pointer to new alignment
        next = new_alignment + (sizeof(T) * n); // Update 'next' to point to next available memory address
        heap_used += sizeof(T) * n + padding; // Update memory used in heap
        
        std::cout << "Allocated " << n << " elements at address " << reinterpret_cast<void*>(ptr) << std::endl;
        std::cout << "Heap used is: " << heap_used << std::endl;
        std::cout << "The next available memory address is: " << reinterpret_cast<void*>(next) << std::endl;
        return ptr;
    }

    // dealloc function
    /* Function to deallocate the entire heap, and 
    reinitialise it again using the init() function */
    void dealloc() {
        delete[] heap;
        init();
        std::cout << "Deallocated all memory." << std::endl;
        std::cout << "Deallocated memory at address " << static_cast<void *>(heap) << std::endl;
    }

private:
    // init fucntion
    // Function to initialise the heap
    void init() {
        heap = new char[heap_size];
        heap_used = 0;
        next = heap;
    }

    char* heap; // Pointer to the beginning of the heap
    char* next; // Pointer to the next available memory address
    size_t heap_size; // Size of the heap
    size_t heap_used; // Amount of memory used
};