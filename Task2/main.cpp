#include "./simpletest/simpletest.h"
#include "bump.hpp"

char const * groups[] = {
"Bump",
};

// Allocaing int
DEFINE_TEST_G(AllocInt, Bump) {
     Bump<20 * sizeof(int)> bumper;

    int * x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    int * y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    int * z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}

// Allocations with a bump size 0
DEFINE_TEST_G(Bump0, Bump) {
    Bump<0 * sizeof(int)> bumper;

    float * x = bumper.alloc<float>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    float * y = bumper.alloc<float>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    float * z = bumper.alloc<float>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}

// Allocations with 0 instances
DEFINE_TEST_G(Alloc0, Bump) {
    Bump<300 * sizeof(int)> bumper;

    double * x = bumper.alloc<double>(0);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    double * y = bumper.alloc<double>(0);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    double * z = bumper.alloc<double>(0);
    TEST_MESSAGE(z != nullptr, "Failed to allocate!!!!");
}

// Allocations with one that exceeds the bump size
DEFINE_TEST_G(AllocExceed, Bump) {
    Bump<20 * sizeof(int)> bumper;

    char * x = bumper.alloc<char>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    char * y = bumper.alloc<char>(50);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    char * z = bumper.alloc<char>(100);
    TEST_MESSAGE(z != nullptr, "Failed to allocate!!!!");
}

// Allocation of a struct
DEFINE_TEST_G(AllocStruct, Bump) {
    Bump<20 * sizeof(double)> bumper;

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

// Allocation of a union
DEFINE_TEST_G(AllocUnion, Bump) {
    union DataUnion {
        int intValue;
        float floatValue;
    };

    DataUnion data;

    Bump<20 * sizeof(DataUnion)> bumper;

    data.intValue = bumper.alloc<int>(1)[0];
    TEST_MESSAGE(data.intValue == 0, "Failed to allocate int value");

    data.floatValue = bumper.alloc<float>(1)[0];
    TEST_MESSAGE(data.floatValue == 0.0f, "Failed to allocate float value");
}

// Deallocating the bump
DEFINE_TEST_G(Dealloc, Bump) {
    Bump<30 * sizeof(int)> bumper;

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