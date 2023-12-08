#include <iostream>
#include <chrono>
#include "bump_down.hpp"
#include "bump_up.hpp"

// Benchmark function
template <typename Func, typename... Args>
auto benchmark(Func func, Args&... args) {
    auto start_time = std::chrono::high_resolution_clock::now(); // Record start time

    func(args...); // Call the function with provided arguments

    auto end_time = std::chrono::high_resolution_clock::now(); // Record end time

    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count(); // Calculate time elapsed in nanoseconds
}

// Bump up benchmarks
void upSmallAllocationsBenchmark(bump_up<1000000>& bumpAllocator) {
    // Make 1000 small allocations of type int
    for (int i = 0; i < 1000; i++) {
        bumpAllocator.alloc<int>(1);
    }
    bumpAllocator.dealloc();
}

void upBigAllocationsBenchmark(bump_up<1000000>& bumpAllocator) {
    // Make 10 big allocations of type char with a size of 25000
    for (int i = 0; i < 10; i++) {
        bumpAllocator.alloc<char>(25000);
    }
    bumpAllocator.dealloc();
}

void upMixedAllocationsBenchmark(bump_up<1000000>& bumpAllocator) {
    // Make 500 mixed allocations of type int (1) and int (10000)
    for (int i = 0; i < 500; i++) {
        bumpAllocator.alloc<int>(1);
        bumpAllocator.alloc<int>(10000);
    }
    bumpAllocator.dealloc();
}

// Bump down benchmarks
void downSmallAllocationsBenchmark(bump_down<1000000>& bumpAllocator) {
    // Make 1000 small allocations of type int
    for (int i = 0; i < 1000; i++) {
        bumpAllocator.alloc<int>(1);
    }
    bumpAllocator.dealloc();
}

void downBigAllocationsBenchmark(bump_down<1000000>& bumpAllocator) {
    // Make 10 big allocations of type char with a size of 25000
    for (int i = 0; i < 10; i++) {
        bumpAllocator.alloc<char>(25000);
    }
    bumpAllocator.dealloc();
}

void downMixedAllocationsBenchmark(bump_down<1000000>& bumpAllocator) {
    // Make 500 mixed allocations of type int (1) and int (10000)
    for (int i = 0; i < 500; i++) {
        bumpAllocator.alloc<int>(1);
        bumpAllocator.alloc<int>(10000);
    }
    bumpAllocator.dealloc();
}

int main() {

    // Bump up Benchmarks
    // Benchmark small allocations
    bump_up<1000000> bumpAllocatorUpSmall;
    auto upTimeSmallAllocations = benchmark(upSmallAllocationsBenchmark, bumpAllocatorUpSmall);
    std::cout << "Time taken for small allocations: " << upTimeSmallAllocations << " nanoseconds." << std::endl;

    // // Benchmark big allocations
    bump_up<1000000> bumpAllocatorUpBig;
    auto upTimeBigAllocations = benchmark(upBigAllocationsBenchmark, bumpAllocatorUpBig);
    std::cout << "Time taken for big allocations up: " << upTimeBigAllocations << " nanoseconds." << std::endl;

    // Benchmark mixed allocations
    bump_up<1000000> bumpAllocatorUpMixed;
    auto upTimeMixedAllocations = benchmark(upMixedAllocationsBenchmark, bumpAllocatorUpMixed);
    std::cout << "Time taken for mixed allocations: " << upTimeMixedAllocations << " nanoseconds." << std::endl;
    std::cout << "Bump up benchmark complete" << std::endl;

    // Bump down Benchmarks
    // Benchmark small allocations
    bump_down<1000000> bumpAllocatorDownSmall;
    auto downTimeSmallAllocations = benchmark(downSmallAllocationsBenchmark, bumpAllocatorDownSmall);
    std::cout << "Time taken for small allocations: " << downTimeSmallAllocations << " nanoseconds." << std::endl;

    // // Benchmark big allocations
    bump_down<1000000> bumpAllocatorDownBig;
    auto downTimeBigAllocations = benchmark(downBigAllocationsBenchmark, bumpAllocatorDownBig);
    std::cout << "Time taken for big allocations down: " << downTimeBigAllocations << " nanoseconds." << std::endl;

    // Benchmark mixed allocations
    bump_down<1000000> bumpAllocatorDownMixed;
    auto downTimeMixedAllocations = benchmark(downMixedAllocationsBenchmark, bumpAllocatorDownMixed);
    std::cout << "Time taken for mixed allocations: " << downTimeMixedAllocations << " nanoseconds." << std::endl;
    std::cout << "Bump down benchmark complete" << std::endl;


    return 0;
}