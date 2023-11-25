#include "bump.hpp"

int main() {
    Bump<20> allocator;
    
    char* ch = allocator.alloc<char>(1);
    double* d = allocator.alloc<double>(1);
    char* ch1 = allocator.alloc<char>(1);
    int* i = allocator.alloc<int>(1);
    
    if (i == nullptr) {
      printf("Zibbi mango\n");
    }

}