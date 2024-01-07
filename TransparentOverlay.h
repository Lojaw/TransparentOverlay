#pragma once
#include <windows.h>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
// TransparentOverlay.h

class TransparentOverlay {
public:
    TransparentOverlay();
    ~TransparentOverlay();
    void Run();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void createWindow();
    void initOpenGL();
    void renderText(const char* text, float x, float y, float sx, float sy);

    HWND hwnd;
    HINSTANCE hInstance;
    HDC hdc;
    HGLRC hglrc;
    FT_Library ft;
    FT_Face face;  // Freetype face
    LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

