#include "TransparentOverlay.h"
#include <Windows.h>
#include <iostream>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <fstream>
#include <string>
//TransparentOverlay.cpp

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

TransparentOverlay::TransparentOverlay() {
    // Verwendung
    this->hInstance = GetModuleHandle(NULL);
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType library" << std::endl;
        exit(1);
    }
    if (FT_New_Face(ft, "C:\\Users\\jpsch\\Desktop\\C++\\Help For DLLInjection\\OpenSans-Italic-VariableFont_wdth,wght.ttf", 0, &face)) {
        std::cerr << "Could not open font" << std::endl;
        exit(1);
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
}


TransparentOverlay::~TransparentOverlay() {
    if (this->hglrc) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(this->hglrc);
    }
    if (this->hdc) ReleaseDC(this->hwnd, this->hdc);
    FT_Done_FreeType(ft);
    FT_Done_Face(face);
}

void TransparentOverlay::Run() {
    createWindow();
    std::cout << "Fenster erstellt." << std::endl;

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        std::cout << "Nachricht verarbeitet: " << msg.message << std::endl;

        //renderText("Hello World", screenWidth / 2.0f, screenHeight / 2.0f, 0.5f, 0.5f);
        InvalidateRect(hwnd, NULL, FALSE); // Markiert den gesamten Bereich des Fensters als ungültig
        UpdateWindow(hwnd); // Erzwingt ein Update des Fensters
    }
}

void TransparentOverlay::createWindow() {
    const wchar_t CLASS_NAME[] = L"TransparentOverlayWindowClass";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = this->hInstance;
    wc.lpszClassName = CLASS_NAME;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    RegisterClassW(&wc);

    this->hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        CLASS_NAME, L"Transparentes Overlay-Fenster",
        WS_POPUP, 0, 0, screenWidth, screenHeight,
        NULL, NULL, this->hInstance, NULL
    );

    if (this->hwnd == NULL) return;

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    ShowWindow(this->hwnd, SW_SHOW);
    UpdateWindow(this->hwnd);

    // OpenGL-Initialisierung
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    initOpenGL();
}

void TransparentOverlay::initOpenGL() {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32, // 32-bit Farbtiefe
        0, 0, 0, 0, 0, 0,
        0, 0,
        0, 0, 0, 0, 0,
        24, // 24-bit Z-Buffertiefe
        8,  // 8-bit Stencil Buffer
        0,
        PFD_MAIN_PLANE,
        0, 0, 0, 0
    };

    this->hdc = GetDC(this->hwnd);
    int format = ChoosePixelFormat(this->hdc, &pfd);
    SetPixelFormat(this->hdc, format, &pfd);
    this->hglrc = wglCreateContext(this->hdc);
    wglMakeCurrent(this->hdc, this->hglrc);

    // Weitere OpenGL-Einrichtung (z.B. GLEW-Initialisierung, falls verwendet)
    glDisable(GL_DEPTH_TEST); // Deaktivieren Sie den Tiefentest für 2D-Zeichnungen
    glDisable(GL_LIGHTING); // Deaktivieren Sie Beleuchtung
}

void TransparentOverlay::renderText(const char* text, float x, float y, float sx, float sy) {
    // Code zum Rendern der Textur für jedes Zeichen
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;

        GLuint texture;  // Declare texture here
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Positionieren Sie die Textur auf dem Bildschirm
        float x2 = x + face->glyph->bitmap_left * sx;
        float y2 = -y - face->glyph->bitmap_top * sy;
        float w = face->glyph->bitmap.width * sx;
        float h = face->glyph->bitmap.rows * sy;

        // Zeichnen Sie Quadrate und binden Sie die Textur
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex2f(x2, -y2);
        glTexCoord2d(0, 1); glVertex2f(x2, -y2 - h);
        glTexCoord2d(1, 1); glVertex2f(x2 + w, -y2 - h);
        glTexCoord2d(1, 0); glVertex2f(x2 + w, -y2);
        glEnd();

        glDisable(GL_BLEND);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &texture);

        // Update x for the next character
        x += (face->glyph->advance.x >> 6) * sx; // Bitshift um 6, um Pixel in Einheiten umzurechnen
    }
}

void TransparentOverlay::renderRectangle() {
    std::cout << "Beginne mit dem Rendering des Rechtecks." << std::endl;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 0.0f, 0.0f); // Rote Farbe

    glBegin(GL_QUADS); // Beginn des Zeichnens eines Rechtecks
    glVertex2f(100.0f, 100.0f); // Oben links
    glVertex2f(200.0f, 100.0f); // Oben rechts
    glVertex2f(200.0f, 200.0f); // Unten rechts
    glVertex2f(100.0f, 200.0f); // Unten links
    glEnd(); // Ende des Zeichnens
    std::cout << "Rendering des Rechtecks abgeschlossen." << std::endl;
}


LRESULT TransparentOverlay::handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);

        // Hier OpenGL-Rendering-Aufrufe
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Funktion zum Rendern von Text aufrufen
        //renderText("Hello World", screenWidth / 2.0f, screenHeight / 2.0f, 0.5f, 0.5f);
        renderRectangle();
        SwapBuffers(hdc);

        EndPaint(hwnd, &ps);
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK TransparentOverlay::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    TransparentOverlay* overlay = nullptr;
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        overlay = reinterpret_cast<TransparentOverlay*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(overlay));
    }
    else {
        overlay = reinterpret_cast<TransparentOverlay*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (overlay) {
        return overlay->handleMessage(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}