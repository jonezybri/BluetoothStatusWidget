cmake_minimum_required(VERSION 3.10)
project(BluetoothStatusWidget)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Windows SDK
if(NOT MSVC)
    message(FATAL_ERROR "This project requires MSVC for C++/WinRT support.")
endif()

# Output executable name
set(EXE_NAME "BluetoothStatusWidget")

# Main Source File
add_executable(${EXE_NAME} WIN32 main.cpp)

# Link against Windows Runtime and Bluetooth libraries
target_link_libraries(${EXE_NAME} PRIVATE
    windowsapp.lib
    runtimeobject.lib
)

# Additional compile options for MSVC (C++/WinRT)
target_compile_options(${EXE_NAME} PRIVATE /ZW /permissive-)
