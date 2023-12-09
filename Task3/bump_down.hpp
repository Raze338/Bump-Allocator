#pragma once

#include <iostream>

template <size_t size>
class bump_down {
public:
    // Constructor
    // Allocates memory on the heap and initializes class members
    bump_down() {
        heap_size = size; // Initialise heap size to template parameter
        heap = new char[heap_size]; // Initialise heap by allocating a new char array on the heap with the specified size
        heap_used = 0; // Initialise the amount used on the heap to 0
        next = heap + heap_size; // Initialise the next pointer to the end of the heap
        allocation_counter = 0; // Initialise counter to 0
    }

    // Destructor
    // Deletes the entire heap memory when the object is destroyed
    ~bump_down() {
        delete[] heap; // Deletes memory on heap
    }
    
    // alloc function
    // Allocate memory for 'n' elements of type 'T'
    template <class T>
    T* alloc(size_t n) {
        size_t address = reinterpret_cast<size_t>(next); // Current memory address
        size_t current_alignment = alignof(T); // Alignment of type 'T'
        size_t padding = (address & -current_alignment); // Calculate padding using bump down logic

        // Check if there is enough space for allocation, return nullptr if failed
        if (next - (sizeof(T) * n) - padding < heap) {
            return nullptr;
        }

        char* new_alignment = reinterpret_cast<char*>(padding) - (sizeof(T) * n); // Calculate the new aligned memory location for the allocation
        T* ptr = reinterpret_cast<T *>(new_alignment); // Create a typed pointer at the calculated aligned memory location
        next = new_alignment; // Update 'next' pointer to the new aligned memory location
        heap_used += (address - reinterpret_cast<uintptr_t>(padding)) + sizeof(T) * n; // Update memory used in heap
        allocation_counter++; // Increment counter

        return ptr;
    }

    // dealloc function
    // Deallocate by resetting the 'next' pointer to the end of the heap
    void dealloc() {
        if (allocation_counter > 0) {
            allocation_counter--;
        }

        if (allocation_counter == 0) {
            next = heap + heap_size; // Reset the 'next' pointer to the end of the heap
        }
    }

private:
    char* heap; // Pointer to the start of the allocated heap
    char* next; // Pointer to the next available memory location in the heap
    size_t heap_size; // Size of the allocated heap in bytes
    size_t heap_used; // Amount of heap memory currently in use
    size_t allocation_counter; // Tracks number of allocations
};
