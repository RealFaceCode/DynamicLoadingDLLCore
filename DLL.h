#ifndef DYNAMICLOADING_DLL_CORE_H
#define DYNAMICLOADING_DLL_CORE_H
#include <iostream>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
    typedef HINSTANCE DLL_Type;
    #define DLL_Load(path) LoadLibrary(path)
    #define DLL_LoadFunc(dll, funcName) GetProcAddress(dll, funcName)
    #define DLL_Close(dll) FreeLibrary(dll)
#elif __unix__
    #include <dlfcn.h>
    typedef void* DLL_Type;
    #define DLL_Load(path) dlopen(path, RTLD_NOW)
    #define DLL_LoadFunc(dll, funcName) dlsym(dll, funcName)
    #define DLL_Close(dll) dlclose(dll)
#else
    typedef void* DLL_Type;
    #define DLL_Load(path)
    #define DLL_LoadFunc(dll, funcName)
    #define DLL_Close(dll)
#endif

typedef DLL_Type DLL;

#ifdef _WIN32
  #ifdef LIB_EXPORTS
    #define API extern "C" __declspec(dllexport)
  #else
    #define API extern "C" __declspec(dllimport)
  #endif
#else
  #define API extern "C"
#endif

#define MakeFuncDefine(r_type, func_def, func_name, ...)    typedef r_type (*func_def)(__VA_ARGS__);\
                                                            func_def func_name = nullptr;
#define LoadFunc(dll, func_def, func_name) func_name = reinterpret_cast<func_def>(DLL_LoadFunc(dll, #func_name))

DLL LoadDLL(const std::filesystem::path& path)
{
    DLL dll = DLL_Load(path.string().c_str());
    if(!dll)
        std::cerr << "Failed to load DLL[" << path.filename() << "]" << std::endl;
    return dll;
}
#endif //DYNAMICLOADING_DLL_CORE_H
