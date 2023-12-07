#include "bump.hpp"

int main() {
    Bump<20> allocator; // Allocating 20 bytes to the bump object
    
    char* ch = allocator.alloc<char>(1); // Allocate 1 instance of a char
    double* d = allocator.alloc<double>(1); // Allocate 1 instance of a double
    char* ch1 = allocator.alloc<char>(1); // Allocate 1 instance of a char
    int* i = allocator.alloc<int>(1); // Allocate 1 instance of a int
    
    // Check if i is a nullptr if it doesnt have enough space to be allocated
    if (i == nullptr) {
      printf("nullptr\n");
    }

    allocator.dealloc(); // Deallocate the heap of the object
}