# Includerdll x64 injector

This code example demonstrates a method for injecting a dll into a process using WinAPI. However, it should be noted that such dll injection method is unsafe.

## Features
- **dll Injection Code Example**: This example illustrates how to perform dll injection into a process using WinAPI.

## Usage
1. **Specify target process**: In the line `const wchar_t* targetProcessName`, specify the name of the target process into which you want to inject the dll.
2. **Specify dll Path**: In the line `GetFullPathNameW(L"MyModule.dll", MAX_PATH, dllPath, nullptr)`, specify the path to the dll you want to inject.
3. **Compile**: Compile and run the code. It will attempt to inject the specified DLL into the specified process.
