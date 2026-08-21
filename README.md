# xvec

C++ 3D vector class designed to be interoperable with other library
implementations.

## Sample Usage

```cpp
#include "vector.hpp"

struct Vector3D;

template <>
struct xvec::is_vector<Derived> : std::true_type {};

struct Vector3D : xvec::vector<float, float, float>
{
	using xvec::vector<float, float, float>::vector;
	// member funtions etc...
};

struct Unknown { float x, y, z; }; // any type with x, y, z

Vector3D v {1.0f, 2.0f, 3.0f};
Unknown u {4.0f, 5.0f, 6.0f};

v += u; // works!
```

## Build instructions

2. run `cmake -S . -B build`
3. run `cmake --build build`
