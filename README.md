# Bump_Allocator

## Table of Contents

- [About](#about)
- [Getting Started](#getting-started)
- [Tasks](#tasks)
    - [Task 1](#task-1)
        - [Review](#review)
        - ['Bump' Class Breakdown](#bump-class-breakdown)
        - [The alignment and padding](#the-alignment-and-padding)
        - [Output](#output)

    - [Task 2](#task-2)
        - [Review](#review-1)
        - ['Bump' Class Unit Tests Breakdown](#bump-class-unit-tests-breakdown)
        - [Output](#output-1)
        - [Output Breakdown](#output-breakdown)


    - [Task 3](#task-3)
        - [Review](#review-2)
        - ['Bump' Class Bump Downwards Breakdown](#bump-class-bump-downwards-breakdown)
        - [The alignment and padding (bump down)](#the-alignment-and-padding-bump-down)
        - [Benchmark function](#benchmark-function)
        - [Output](#output-2)

## About

This worksheet focuses on memory allocation and the implementation of a bump allocator.

## Getting Started

To run the Tasks in this worksheet, use the following commands:

Acessing tasks:

~~~ruby
$ cd Task<task number>
~~~


Task 1:
~~~ruby
$ clang++ main.cpp -o main; ./main
~~~

For Task 2, it is essential to complie the simpletest.cpp file as well while running the code:

~~~ruby
$ clang++ main.cpp simpletest/simpletest.cpp -o main; ./main
~~~

For Task 3, a comparison in performance can be made when compiling with or without '-O3' (aggressive optimisation):

without:
~~~ruby
$ clang++ main.cpp -o main; ./main
~~~

with:
~~~ruby
$ clang++ -O3 main.cpp -o main; ./main
~~~

## Memory Allocation

Memory allocation is a crucial aspect of programming where programs request and release memory during their execution. In this worksheet, we explore the concept of memory allocation by delving into the implementation of a custom heap allocator, specifically focusing on a type known as a bump allocator.

## Tasks

### Task 1

#### Review

-Task 1 explores the implementation of a 'Bump Allocator'. It is a template class that takes in the specified size for the allocator, and handles the allocation of bytes depending on the datatype, and the deallocation of the entire heap once called.

#### 'Bump' Class Breakdown

Constructor:

~~~ruby
bump() {
        heap_size = size;
        heap = new char[heap_size];
        heap_used = 0;
        next = heap;
        allocation_counter = 0;
    }
~~~
The constructor consists of the 'heap_size', which underlines the size of the heap itself when specified in main, and the 'heap' itself (allocated with memory dynamically using the 'new' function, and being a char[heap_size] to be exactly one byte to work with memory in the byte level, and to ensure accuracy with respect to allocaions and for the alignment), the 'heap_used' which is set to '0' to be a starting point for the amount of memory allocated in our bump allocator, and a 'next' pointer that points to the heap, to be able to keep track of the next available memory address after allocation (further explained in the alloc function). An allocation counter is initialised to 0 to keep track of the number of allocations.

---

Destructor:
~~~ruby
 ~bump() {
        delete[] heap;
    }
~~~

The destructor is used to delete the entire heap after the bump object goes out of scope, and is done to safely free up memory, eventhough the bump has a deallocation function.

---

alloc function:

~~~ruby
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
        allocation_counter++;
        
        std::cout << "Allocated " << n << " elements at address " << reinterpret_cast<void*>(ptr) << std::endl;
        std::cout << "Heap used is: " << heap_used << std::endl;
        std::cout << "The next available memory address is: " << reinterpret_cast<void*>(next) << std::endl;
        return ptr;
~~~

The alloc function is used to allocate memory in the heap for a specified number of elements of type 'T'. It implements the use of an alignment logic, where it ensures that when an instance is allocated in the heap, alignment requirements of the specific type 'T' is calculated to adjust the memory address 'next' to meet these requirements, and padding is applied between allocations to ensure that the next allocation starts at a properly aligned memory address.

### The alignment and padding:

We firstly need to get the current memory address (by converting the current value of the 'next' pointer to 'size_t'), then determine the alignement required for type 'T' (current_alignment) by using the 'alignof' function, which is the boundary on which objects of the current datatype should be aligned to in memory. 

Once we acquire both the address and the alignment needed for the current object, we then need to calculate the padding needed to be set between allocations when necessary. 

~~~ruby
size_t padding = (current_alignment - (address % current_alignment)) % current_alignment;
~~~

The 'address % current_alignment' operation gives the remainder of the 'address' being divided by the 'current_alignment' to see how much the current address would deviate from being aligned. Subtracting this result with the 'current_alignment' would then give us the additional memory needed to reach the next aligned memory. 

This is then used with '% current_alignemnt' to calculate the final padding value, ensuring that the calculated padding is with the range of 0 to 'current_alignment' - 1. If the address is already aligned, the result will be 0 (meaning no padding needed).


---

dealloc function:

~~~ruby
void dealloc() {
       if (allocation_counter > 0) {
            allocation_counter--;
        }

        if (allocation_counter == 0) {
            next = heap; // Reset the 'next' pointer to the beginning of the heap
        }
        std::cout << "Deallocated all memory." << std::endl;
        std::cout << "Deallocated memory at address " << static_cast<void *>(heap) << std::endl;
    }
~~~

The dealloc funciton is used set the 'next' pointer back to the beginning of the heap. The reason for not having it delete the entire heap is that the memory that is already used for it in the heap will always remain no matter what you do, unless you destroy the object. The destructor does this to free up the resources used for the object after it has been destroyed. This means that when we move the 'next' pointer back to the beginning, we overwrite the existing memory in the heap instead of having a new one. 

---
#### Output

Main:
~~~ruby
int main() {
    bump<20> allocator; // Allocating 20 bytes to the bump object
    
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
~~~

Terminal:
~~~ruby
Allocated 1 elements at address 0x2120eb0
Heap used is: 1
The next available memory address is: 0x2120eb1
Allocated 1 elements at address 0x2120eb8
Heap used is: 16
The next available memory address is: 0x2120ec0
Allocated 1 elements at address 0x2120ec0
Heap used is: 17
The next available memory address is: 0x2120ec1
nullptr
Deallocated all memory.
Deallocated memory at address 0x2120eb0
~~~
---

### Task 2

#### Review

-Task 2 involves the use of a subrepo on github that is used to create unit tests. They are implemented in this task to ensure the accuracy of the allocations and deallocations of the bump allocator using different datatypes and bump sizes.

The code remains the same, with the exception for the dealloc function now being of type 'bool' rather than 'void', to be able to test it and produce results.

#### 'Bump' Class Unit Tests Breakdown

dealloc function bool type:
~~~ruby
    bool dealloc() {
        if (allocation_counter > 0) {
            allocation_counter--;
        }

        if (allocation_counter == 0) {
            next = heap; // Reset the 'next' pointer to the beginning of the heap
        }
        return true;
    }
~~~

Unit tests in main:
~~~ruby
char const * groups[] = {
"Bump",
};

DEFINE_TEST_G(AllocInt, Bump) {
     bump<20 * sizeof(int)> bumper;

    int * x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    int * y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    int * z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}

DEFINE_TEST_G(Bump0, Bump) {
    bump<0 * sizeof(int)> bumper;

    float * x = bumper.alloc<float>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    float * y = bumper.alloc<float>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    float * z = bumper.alloc<float>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}

DEFINE_TEST_G(Alloc0, Bump) {
    bump<300 * sizeof(int)> bumper;

    double * x = bumper.alloc<double>(0);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    double * y = bumper.alloc<double>(0);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    double * z = bumper.alloc<double>(0);
    TEST_MESSAGE(z != nullptr, "Failed to allocate!!!!");
}

DEFINE_TEST_G(AllocExceed, Bump) {
    bump<20 * sizeof(int)> bumper;

    char * x = bumper.alloc<char>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    char * y = bumper.alloc<char>(50);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    char * z = bumper.alloc<char>(100);
    TEST_MESSAGE(z != nullptr, "Failed to allocate!!!!");
}

DEFINE_TEST_G(AllocStruct, Bump) {
    bump<20 * sizeof(double)> bumper;

    struct Point {
    double x;  
    double y;  
    double z;  
    };

    Point myPoint;

    myPoint.x = 1.0;
    myPoint.y = 2.5;
    myPoint.z = -3.7;

    double* xPtr = bumper.alloc<double>(10);
    TEST_MESSAGE(xPtr != nullptr, "Failed to allocate!!!!");
    myPoint.x = *xPtr;

    double* yPtr = bumper.alloc<double>(10);
    TEST_MESSAGE(yPtr != nullptr, "Failed to allocate!!!!");
    myPoint.y = *yPtr;

    double* zPtr = bumper.alloc<double>(10);
    TEST_MESSAGE(zPtr == nullptr, "Should have failed to allocate!!!!");
}


DEFINE_TEST_G(AllocUnion, Bump) {
    union DataUnion {
        int intValue;
        float floatValue;
    };

    DataUnion data;

    bump<20 * sizeof(DataUnion)> bumper;

    data.intValue = bumper.alloc<int>(1)[0];
    TEST_MESSAGE(data.intValue == 0, "Failed to allocate int value");

    data.floatValue = bumper.alloc<float>(1)[0];
    TEST_MESSAGE(data.floatValue == 0.0f, "Failed to allocate float value");
}

DEFINE_TEST_G(Dealloc, Bump) {
    bump<30 * sizeof(int)> bumper;

    int * x1 = bumper.alloc<int>(5);
    int * y1 = bumper.alloc<int>(5);
    int * z1 = bumper.alloc<int>(5);
    
    TEST_MESSAGE(bumper.dealloc(), "Failed deallocation");
}

int main() {

    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    return pass ? 0 : 1;
}
~~~

#### Output

~~~ruby
Running all tests in groups [Bump].
Running [Bump/AllocInt]: Passed 3 out of 3 tests in 4e-06 seconds
Running [Bump/Bump0]: Failed 2 out of 3 tests
main.cpp(23): Condition [x != nullptr] Failed. Failed to allocate!!!!
main.cpp(25): Condition [y != nullptr] Failed. Failed to allocate!!!!
Running [Bump/Alloc0]: Passed 3 out of 3 tests in 1e-06 seconds
Running [Bump/AllocExceed]: Failed 1 out of 3 tests
main.cpp(49): Condition [z != nullptr] Failed. Should have failed to allocate!!!!
Running [Bump/AllocStruct]: Passed 3 out of 3 tests in 1e-06 seconds
Running [Bump/AllocUnion]: Passed 2 out of 2 tests in 0 seconds
Running [Bump/Dealloc]: Passed 1 out of 1 tests in 1e-06 seconds
7 Tests finished, 3 of 18 assertions failed. Some tests are reporting errors.
~~~

#### Output Breakdown

If we take the first output for the first test function 'AllocInt', we get that all the results passed. 

~~~ruby
DEFINE_TEST_G(AllocInt, Bump) {
     bump<20 * sizeof(int)> bumper;

    int * x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    int * y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    int * z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}
~~~

The first 2 test messages check whether 'x' and 'y' are not null pointers, which is correct since the bump has enough space to allocate the 10 instances of each 'x' and 'y'.

The third test message checks whether 'z' is a null pointer, which is correct since the bump at that point does not have enough space to allocate for the 10 instances of 'z'. 

We know this since the size of the bump is 20 * 4(which is the size of bytes for int) giving us 80. The first 2 allocations total at 80 bytes (4 bytes for each instance, meaning 10 instances would be 40, and since both 'x' and 'y' allocate the same amount, it totals 80). This means that the bump used up 100% capacity for allocations (80/80). The allocation of 10 bytes for 'z' is not possible, making it a nullptr.

~~~ruby
Running [Bump/AllocInt]: Passed 3 out of 3 tests in 4e-06 seconds
~~~

---

When it comes to the Second test function, we get that the test had failed 2 out of 3 tests. 

~~~ruby
DEFINE_TEST_G(Bump0, Bump) {
    bump<0 * sizeof(int)> bumper;

    float * x = bumper.alloc<float>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    float * y = bumper.alloc<float>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    float * z = bumper.alloc<float>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}
~~~

This function is introduced to test the outcome when the bump size is 0. The first 2 test messages fail since the is no space for the bump to allocate to. The third test message passes due to the fact that in my implementation for the bump, if there is not enough space to allocate, it would return a 'nullptr'. This suffices the condition, making it pass.

~~~ruby
Running [Bump/Bump0]: Failed 2 out of 3 tests
main.cpp(23): Condition [x != nullptr] Failed. Failed to allocate!!!!
main.cpp(25): Condition [y != nullptr] Failed. Failed to allocate!!!!
~~~

---
When it comes to the fourth test function, we get that the test had failed 1 out of 3 tests.

~~~ruby
DEFINE_TEST_G(AllocExceed, Bump) {
    bump<20 * sizeof(int)> bumper;

    char * x = bumper.alloc<char>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    char * y = bumper.alloc<char>(50);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    char * z = bumper.alloc<char>(100);
    TEST_MESSAGE(z != nullptr, "Failed to allocate!!!!");
}
~~~

This function is introduced to test the outcome when the allocations exceed the limit of the bump size. The first 2 test messages pass as expected due to them having the necessary space to allocate, while the last one fails due to it not having enough space to allocate (Each instance is 1 byte since a char is 1 byte, 10 + 50 = 60, bump size = 80, only 20 bytes left in bump after the first 2 allocations, 3rd allocation exceeeds that allowed limit as it wants to allocate 100 instances).

~~~ruby
Running [Bump/Alloc0]: Passed 3 out of 3 tests in 1e-06 seconds
Running [Bump/AllocExceed]: Failed 1 out of 3 tests
main.cpp(49): Condition [z != nullptr] Failed. Should have failed to allocate!!!!
~~~

---

### Task 3

#### Review

-Task 3 explores the implementation of a "bump down" allocator, and a benchmark function that tests the time efficiency of both bump up and bump down allocators. 

The bump down code is derived from the bump up code (which is the code from task 1 and 2) and modified so that it incorporates the bump down logic, in the alloc and dealloc functions.

---

#### 'Bump' Class Bump Downwards Breakdown

Bump down alloc function:

~~~ruby
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
        allocation_counter++;
        return ptr;
    }
~~~

The alloc function allocates space for a specified number (n) of elements of type 'T'. It calculates the current memory address, determines the alignment requirement for type T, and calculates the necessary padding to achieve alignment using bump-down logic. 

### The alignment and padding (bump down):

The main difference between the alignment and padding of the bump down allocator from the bump up allocator is the calculation for the padding.

In the padding calculation, 'address & -current_alignment' uses the bitwise AND operator (&) with the two's complement of 'current_alignment' to calculate the padding required for alignment. The '& -current_alignment' operation basically rounds down the current address to the nearest multiple of the alignment.

The new alignment for the memory location for the allocation is then calculated by adjusting the address, moving it backwards by the size of the allocation 'sizeof(T) * n', ensuring that it is aligned correctly.

---

Bump down dealloc function:
~~~ruby
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
~~~

The dealloc funciton is used set the 'next' pointer back to the end of the heap. The reason for not having it delete the entire heap is that the memory that is already used for it in the heap will always remain no matter what you do, unless you destroy the object. The destructor does this to free up the resources used for the object after it has been destroyed. This means that when we move the 'next' pointer back to the end, we overwrite the existing memory in the heap instead of having a new one. 

#### Benchmark function:

~~~ruby
template <typename Func, typename... Args>
auto benchmark(Func func, Args&... args) {
    auto start_time = std::chrono::high_resolution_clock::now(); // Record start time

    func(args...); // Call the function with provided arguments

    auto end_time = std::chrono::high_resolution_clock::now(); // Record end time

    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count(); // Calculate time elapsed in nanoseconds
}
~~~

The benchmark function is a template class that takes 2 parameters,  (Func:type of function) and (Args:types of arguments). It takes in a function (func) and a variable number of arguments (args). The function records the start time and the end time using the 'chrono' library to obtain a high resolution time point. 

'func(args...)' is then called with the provided arguments, recording the beginning and the end of the execution time, calculating the time elapsed by subracting the start time from the end time. The duration is then returned in nanoseconds to achieve accuracy.

3 funtions then are created, having 3 different tests that showcase each bump's efficiency. A small allocation, a big allocation, and a mixed allocation.

Passing the fucntion arguments by reference has proven more efficent than passing by value. This is due to the fact that passing by reference does not create a copy, rather it allows the function to work directly with the original data in the sme memory location. This reduces memory overhead, especially when working with large objects.

---

#### Benchmark tests:

~~~ruby
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
~~~

---

#### Output

It is worth noting that each run may give a different result. 

Output without passing by reference (testing for big allocations only):
~~~ruby
Time taken for big allocations up: 15600 nanoseconds.
Time taken for big allocations down: 6400 nanoseconds.
~~~

---

Output with passing by reference:
~~~ruby
Time taken for small allocations: 24300 nanoseconds.
Time taken for big allocations up: 300 nanoseconds.
Time taken for mixed allocations: 17500 nanoseconds.
Bump up benchmark complete
Time taken for small allocations: 7100 nanoseconds.
Time taken for big allocations down: 200 nanoseconds.
Time taken for mixed allocations: 7100 nanoseconds.
Bump down benchmark complete
~~~

---

Optimised output when passed by reference (lowest recorded):
~~~ruby
Time taken for small allocations: 100 nanoseconds.
Time taken for big allocations up: 0 nanoseconds.
Time taken for mixed allocations: 0 nanoseconds.
Bump up benchmark complete
Time taken for small allocations: 0 nanoseconds.
Time taken for big allocations down: 0 nanoseconds.
Time taken for mixed allocations: 0 nanoseconds.
Bump down benchmark complete
~~~

---
