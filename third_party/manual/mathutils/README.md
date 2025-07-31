# MathUtils - Example Header-Only Manual Dependency

This is an example of a header-only library managed as a manual dependency.

## Overview

MathUtils provides basic mathematical utility functions for C++ applications. It demonstrates how to structure and integrate header-only libraries in the cpp-template project.

## Features

- Statistical functions (mean, standard deviation)
- Interpolation utilities (linear interpolation)
- Utility functions (clamp, approximate equality)
- Header-only implementation for easy integration

## Usage

```cpp
#include <mathutils/mathutils.h>
#include <vector>
#include <iostream>

int main() {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    
    double avg = mathutils::mean(data);
    double stddev = mathutils::standard_deviation(data);
    
    std::cout << "Mean: " << avg << std::endl;
    std::cout << "Standard Deviation: " << stddev << std::endl;
    
    // Linear interpolation
    double interpolated = mathutils::lerp(0.0, 10.0, 0.5); // Result: 5.0
    
    // Clamping
    double clamped = mathutils::clamp(15.0, 0.0, 10.0); // Result: 10.0
    
    return 0;
}
```

## CMake Integration

The library is automatically detected and configured by the manual dependencies CMake script:

```cmake
target_link_libraries(your_target PRIVATE third_party::mathutils)
```

## Structure

```
mathutils/
├── include/
│   └── mathutils/
│       └── mathutils.h    # Main header file
└── README.md              # This file
```

## Installation

This library is included as an example manual dependency. To replace it with a real library:

1. Remove the existing files
2. Copy your library files to this directory
3. Ensure headers are in `include/mathutils/`
4. Update this README with your library's documentation
5. Reconfigure CMake

## License

This example code is provided for demonstration purposes only.