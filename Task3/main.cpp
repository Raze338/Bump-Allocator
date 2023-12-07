#include <iostream>
#include <chrono>
#include "bump_down.hpp"
#include "bump_up.hpp"

// Benchmark function
template <typename Func, typename... Args>
auto benchmark(Func func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Call the function with provided arguments
    func(std::forward<Args>(args)...);

    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
}

// Bump down benchmark
// // Example functions to benchmark
void downSmallAllocationsBenchmark() {
    Bump_down<1000000> bumpAllocator;  // Adjust the heap size accordingly
    for (int i = 0; i < 1000; ++i) {
        int* ptr = bumpAllocator.alloc<int>(1);
        // Do something with ptr
    }
    bumpAllocator.dealloc();
}

void downBigAllocationsBenchmark() {
    Bump_down<1000000> bumpAllocator;  // Adjust the heap size accordingly
    for (int i = 0; i < 10; ++i) {
        int* ptr = bumpAllocator.alloc<int>(100000);
        // Do something with ptr
    }
    bumpAllocator.dealloc();
}

void downMixedAllocationsBenchmark() {
    Bump_down<1000000> bumpAllocator;  // Adjust the heap size accordingly
    for (int i = 0; i < 500; ++i) {
        int* ptr1 = bumpAllocator.alloc<int>(1);
        int* ptr2 = bumpAllocator.alloc<int>(10000);
        // Do something with ptr1 and ptr2
    }
    bumpAllocator.dealloc();
}

// Bump up 
// Example functions to benchmark
void upSmallAllocationsBenchmark() {
    Bump_up<1000000> bumpAllocator;  // Adjust the heap size accordingly
    for (int i = 0; i < 1000; ++i) {
        int* ptr = bumpAllocator.alloc<int>(1);
        // Do something with ptr
    }
    bumpAllocator.dealloc();
}

void upBigAllocationsBenchmark() {
    Bump_up<1000000> bumpAllocator;  // Adjust the heap size accordingly
    for (int i = 0; i < 10; ++i) {
        int* ptr = bumpAllocator.alloc<int>(100000);
        // Do something with ptr
    }
    bumpAllocator.dealloc();
}

void upMixedAllocationsBenchmark() {
    Bump_up<1000000> bumpAllocator;  // Adjust the heap size accordingly
    for (int i = 0; i < 500; ++i) {
        int* ptr1 = bumpAllocator.alloc<int>(1);
        int* ptr2 = bumpAllocator.alloc<int>(10000);
        // Do something with ptr1 and ptr2
    }
    bumpAllocator.dealloc();
}

int main() {
    // Bump_down Benchmarks
    // Benchmark small allocations
    auto downTimeSmallAllocations = benchmark(downSmallAllocationsBenchmark);
    std::cout << "Time taken for small allocations: " << downTimeSmallAllocations << " milliseconds." << std::endl;

    // Benchmark big allocations
    auto downTimeBigAllocations = benchmark(downBigAllocationsBenchmark);
    std::cout << "Time taken for big allocations: " << downTimeBigAllocations << " milliseconds." << std::endl;

    // Benchmark mixed allocations
    auto downTimeMixedAllocations = benchmark(downMixedAllocationsBenchmark);
    std::cout << "Time taken for mixed allocations: " << downTimeMixedAllocations << " milliseconds." << std::endl;
    std::cout << "Bump down benchmark complete" << std::endl;


    // Bump_up Benchmarks
    // Benchmark small allocations
    auto upTimeSmallAllocations = benchmark(upSmallAllocationsBenchmark);
    std::cout << "Time taken for small allocations: " << upTimeSmallAllocations << " milliseconds." << std::endl;

    // Benchmark big allocations
    auto upTimeBigAllocations = benchmark(upBigAllocationsBenchmark);
    std::cout << "Time taken for big allocations: " << upTimeBigAllocations << " milliseconds." << std::endl;

    // Benchmark mixed allocations
    auto upTimeMixedAllocations = benchmark(upMixedAllocationsBenchmark);
    std::cout << "Time taken for mixed allocations: " << upTimeMixedAllocations << " milliseconds." << std::endl;
    std::cout << "Bump up benchmark complete" << std::endl;


    return 0;
}