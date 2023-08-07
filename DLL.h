#ifndef DYNAMICLOADING_DLL_CORE_H
#define DYNAMICLOADING_DLL_CORE_H
#include <iostream>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
    typedef HINSTANCE DLL_Type;
    #define DLL_Load(path) LoadLibraryW(path)
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

#ifdef _WIN32
  #ifdef _WIN64
    #define CALL __cdecl 
  #else
    #define CALL __stdcall
  #endif
#else
  #define CALL
#endif

#define MakeFuncDefine(r_type, func_def, func_name, ...)      typedef CALL r_type (*func_def)(__VA_ARGS__);\
func_def func_name = nullptr;
#define LoadFunc(dll, func_def, func_name, func_name_string)  func_name = reinterpret_cast<func_def>(DLL_LoadFunc(dll, func_name_string))

#ifdef _WIN32
  DLL LoadDLL(const wchar_t* path)
  {
    DLL dll = DLL_Load(path);
    if(!dll)
      std::wcout << "Failed to load library[" << path  << "]" << std::endl;
      return dll;
  }
#elif __unix__
  DLL LoadDLL(const char* path)
  {
    DLL dll = DLL_Load(path);
    if(!dll)
      std::cout << "Failed to load library[" << utf8String << "]" << std::endl;
    
    return dll;
  }
#else 
  DLL LoadDLL(const char* path)
  {
    return nullptr;
  }
#endif
#endif //DYNAMICLOADING_DLL_CORE_H
