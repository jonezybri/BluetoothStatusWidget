# Bluetooth Status Widget (Windows)

A simple Windows C++ application that functions as a widget to display the connection status of paired Bluetooth devices.

## Features
- Lists all paired Bluetooth devices.
- Shows real-time connection status (Connected / Disconnected).
- Automatically refreshes every 5 seconds.
- Uses modern **C++/WinRT** APIs for Bluetooth interaction.
- Lightweight Win32 GUI window with "Always on Top" and "Tool Window" properties to mimic a widget.

## Prerequisites
- **Windows 10/11**
- **Visual Studio 2019 or later** with the following components:
  - C++ CMake tools for Windows
  - Windows 10/11 SDK
  - C++/WinRT support

## How to Build
1. Clone the repository:
   ```bash
   git clone https://github.com/jonezybri/BluetoothStatusWidget.git
   cd BluetoothStatusWidget
   ```
2. Open the folder in Visual Studio or use CMake from the command line:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3. Run the generated `BluetoothStatusWidget.exe`.

## License
This project uses open-source code and is licensed under the MIT License.
