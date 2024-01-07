#include "TransparentOverlay.h"
#include <Windows.h>

TransparentOverlay::TransparentOverlay() {
    this->hInstance = GetModuleHandle(NULL);
}

TransparentOverlay::~TransparentOverlay() {
    // Aufräumarbeiten, falls nötig
}

void TransparentOverlay::Run() {
    createWindow();

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Ändern Sie Zeichenkettenliterale zu Unicode-Literalen
void TransparentOverlay::createWindow() {
    const wchar_t CLASS_NAME[] = L"TransparentOverlayWindowClass";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = this->hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    this->hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        CLASS_NAME, L"Transparentes Overlay-Fenster",
        WS_POPUP, 100, 100, 500, 500, NULL, NULL, this->hInstance, NULL
    );

    if (this->hwnd == NULL) return;

    // Fenster transparent machen, aber Text sichtbar lassen
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    ShowWindow(this->hwnd, SW_SHOW);
    UpdateWindow(this->hwnd);
}

LRESULT CALLBACK TransparentOverlay::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Schriftart erstellen
        HFONT hFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        // Text zeichnen
        const wchar_t* text = L"Hallo Welt!";
        RECT rect;
        GetClientRect(hwnd, &rect);
        DrawTextW(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Aufräumen
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

