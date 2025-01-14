# Vector Templates In C

In this project, we will explore the process of implementing a vector template in C.

Vectors are sequence containers that represent arrays, capable of dynamically changing in size.

Macros can be used to implement vector templates in C, enabling compile-time polymorphism.

However, [C++ templates](https://www.geeksforgeeks.org/vector-in-cpp-stl/) offer stronger type checking compared to C macros.

**Example**

```C
int main(void) {
    // a vector of integers
    struct Vector(long) *pVec1 = CreateVector(long)();
    VectorAppend(long)(pVec1, 10);
    VectorAppend(long)(pVec1, 20);
    PrintVector(long)(pVec1);
    ReleaseVector(long)(pVec1);

    // a vector of doubles
    struct Vector(double) *pVec2 = CreateVector(double)();
    VectorAppend(double)(pVec2, 300.0);
    VectorAppend(double)(pVec2, 3.14);
    PrintVector(double)(pVec2);
    ReleaseVector(double)(pVec2);
    return 0;  
}
```

**Output**
```sh
TemplatesInC$ ./main
10 20 
300.000000 3.140000
```

More details are given in [Vector.c](./src/Vector.c) and [Vector.h](./src/Vector.h).

