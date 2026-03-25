#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Enumeration;

// Global state for the widget
struct DeviceStatus {
    std::wstring name;
    bool isConnected;
};

std::vector<DeviceStatus> g_devices;
std::mutex g_devicesMutex;
HWND g_hWnd = NULL;

// Function to refresh Bluetooth device list
IAsyncAction RefreshBluetoothStatus() {
    auto selector = BluetoothDevice::GetDeviceSelector();
    auto devices = co_await DeviceInformation::FindAllAsync(selector);

    std::vector<DeviceStatus> newDevices;
    for (auto&& info : devices) {
        try {
            auto device = co_await BluetoothDevice::FromIdAsync(info.Id());
            if (device) {
                newDevices.push_back({ info.Name().c_str(), device.ConnectionStatus() == BluetoothConnectionStatus::Connected });
            }
        } catch (...) {
            // Some devices might fail to open
        }
    }

    {
        std::lock_guard<std::mutex> lock(g_devicesMutex);
        g_devices = std::move(newDevices);
    }

    // Trigger a repaint of the window
    if (g_hWnd) {
        InvalidateRect(g_hWnd, NULL, TRUE);
    }
}

// Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        std::lock_guard<std::mutex> lock(g_devicesMutex);
        int y = 10;
        TextOut(hdc, 10, y, L"Bluetooth Device Status:", 24);
        y += 30;

        if (g_devices.empty()) {
            TextOut(hdc, 20, y, L"No paired devices found.", 24);
        } else {
            for (const auto& dev : g_devices) {
                std::wstring status = dev.name + L" - " + (dev.isConnected ? L"Connected" : L"Disconnected");
                TextOut(hdc, 20, y, status.c_str(), static_cast<int>(status.length()));
                y += 20;
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_TIMER:
        RefreshBluetoothStatus();
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    init_apartment();

    // Register Window Class
    const wchar_t CLASS_NAME[] = L"BluetoothWidgetClass";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Create Window (Widget-like: Small, No resizing)
    g_hWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        L"Bluetooth Status",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
        NULL, NULL, hInstance, NULL
    );

    if (g_hWnd == NULL) return 0;

    ShowWindow(g_hWnd, nCmdShow);

    // Initial Refresh
    RefreshBluetoothStatus();

    // Refresh every 5 seconds
    SetTimer(g_hWnd, 1, 5000, NULL);

    // Message Loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
