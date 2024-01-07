// TransparentOverlay.h
#pragma once
#include <windows.h>
#include <string>

class TransparentOverlay {
public:
    TransparentOverlay();
    ~TransparentOverlay();
    void Run();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void createWindow();
    HWND hwnd;
    HINSTANCE hInstance;
};
