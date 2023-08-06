# DynamicLoading DLL Core

This repository contains a C++ header file that provides cross-platform dynamic loading of shared libraries (DLLs on Windows, SO files on Unix-like systems) using the appropriate APIs. This can be useful for cases where you want to load functions from external libraries at runtime.

## Features

- Cross-platform support for loading and unloading shared libraries.
- Automatic handling of platform-specific APIs: Windows (using `LoadLibrary` and `GetProcAddress`) and Unix-like systems (using `dlopen` and `dlsym`).

## Requirements

- C++17 or later
- For Unix-like systems: POSIX compliant environment (such as Linux)

## Usage

To use the dynamic loading functionality provided by this header, follow these steps:

1. Include the `DLL.h` header in your C++ project.
2. Use the `DLL` type for managing the loaded library.
3. Define function signatures using the `MakeFuncDefine` macro.
4. Load functions from the DLL using the `LoadFunc` macro.

### Compiling the Library to be Dynamically Loaded

When compiling the library that you intend to dynamically load, you should define the `LIB_EXPORTS` macro to export the necessary symbols. Here's an example of how you can define `LIB_EXPORTS` during compilation:

#### On Windows
```bash
g++ -std=c++17 -o mylibrary.dll -DLIB_EXPORTS mylibrary.cpp -shared
```

#### On Unix-like systems (Linux)
```bash
g++ -std=c++17 -fPIC -o mylibrary.so -DLIB_EXPORTS mylibrary.cpp -shared
```

### Examples

```cpp
#include <iostream>
#include <filesystem>
#include "DLL.h"

// Define function signature
MakeFuncDefine(int, MyFunctionSignature, MyFunction, int);

int main()
{
    DLL dll = LoadDLL("path/to/your/library.dll");
    if (dll)
    {
        // Load the function from the DLL
        LoadFunc(dll, MyFunctionSignature, MyFunction);

        // No check for a nullptr necessary because a std::runtime_error is thrown if the function is not loaded
        int result = MyFunction(42);
        std::cout << "Result: " << result << std::endl;

        DLL_Close(dll);
    }
    else
    {
        std::cerr << "Failed to load DLL" << std::endl;
    }

    return 0;
}
```

Example of a DLL include header.
```cpp
#ifndef MYLIB_H
#define MYLIB_H
#include "DLL.h"

MakeFuncDefine(void, sayHello_t, sayHello)
MakeFuncDefine(int, add_t, add, int, int)

void loadFunction(DLL dll)
{
    LoadFunc(dll, sayHello_t, sayHello);
    LoadFunc(dll, add_t, add);
}

#endif // MYLIB_H
```

DLL main cpp file.
```cpp
#include "dll_core.h"
#include <iostream>

API void sayHello() {
    std::cout << "Hello from my shared library!" << std::endl;
}

API int add(int x, int y)
{
    return x + y;
}
```

Use example.
```cpp
#include "DLL/dll.h"
#include "DLL.h"

int main()
{
    DLL dll = LoadDLL("DLL_Test/libMySharedLib.dll");

    if(!dll)
        return -1;

    loadFunction(dll);

    sayHello();
    int i = add(1, 2);
}
```

## License

This project is licensed under the [MIT License](LICENSE).

## Contributions

Contributions to this project are welcome! If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.
