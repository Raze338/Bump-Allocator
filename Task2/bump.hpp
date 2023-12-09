#pragma once

#include <iostream>

template <size_t size>
class bump {
public:
    // Constructor
    // Allocates memory on the heap and initializes class members
    bump() {
        heap_size = size; // Initialise heap size to template parameter
        heap = new char[heap_size]; // Initialise heap by allocating a new char array on the heap with the specified size
        heap_used = 0; // Initialise the amount used on the heap to 0
        next = heap; // Initialise the next pointer to the beginning of the heap
        allocation_counter = 0; // Initialise counter to 0
    }

    // Destructor
    // Deletes the entire heap memory when the object is destroyed
    ~bump() {
        delete[] heap; // Deletes memory on heap
    }

    // alloc function
    // Allocate memory for 'n' elements of type 'T'
    template <class T>
    T* alloc(size_t n) {
        size_t address = reinterpret_cast<size_t>(next); // Current memory address
        size_t current_alignment = alignof(T);  // Alignment of type 'T'
        size_t padding = (current_alignment - (address % current_alignment)) % current_alignment; // Padding needed to satisfy alignment

        // Check if there is enough space for allocation, return nullptr if failed
        if (heap + heap_size - next < (sizeof(T) * n) + padding) {
            return nullptr;
        }
        
        next += padding; // Adjust alignment of 'next'
        T* ptr = reinterpret_cast<T*>(next); // Create a typed pointer at the adjusted 'next' location
        next += sizeof(T) * n; // Move 'next' pointer to next available memory after allocation
        heap_used += sizeof(T) * n + padding; // Update memory used in heap
        allocation_counter++; // Increment counter

        return ptr;
    }

    // dealloc function
    // Deallocate by resetting the 'next' pointer to the beginning of the heap
    bool dealloc() {
        if (allocation_counter > 0) {
            allocation_counter--;
        }

        if (allocation_counter == 0) {
            next = heap; // Reset the 'next' pointer to the beginning of the heap
        }
        return true;
    }

private:
    char* heap; // Pointer to the start of the allocated heap
    char* next; // Pointer to the next available memory location in the heap
    size_t heap_size; // Size of the allocated heap in bytes
    size_t heap_used; // Amount of heap memory currently in use
    size_t allocation_counter; // Tracks number of allocations
};