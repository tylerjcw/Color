#pragma once

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include "Color.hpp"
#include "Gradient.hpp"

#include <windows.h>
#include <string>

namespace KTLib
{
    class Showcase
    {
        public:
            Showcase(Color* color, const char* title);
            Showcase(Gradient* gradient, const char* title);
            Showcase(Canvas* buffer, const char* title);
            ~Showcase();

            int Show();

        private:
            HWND m_hwnd;
            std::string m_title;
            Color* m_color;
            Gradient* m_gradient;
            Canvas* m_buffer;
            bool m_isColor;
            bool m_isGradient;
            HFONT m_hFont;
            int m_fontHeight;
            int m_bufferWidth;
            int m_bufferHeight;
            int m_bufferUnique;
            Color m_bufferAverage;

            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
            void CreateControls();
            void LayoutControls();
            void DrawPreview(HDC hdc);
            void DrawColorInfo(HDC hdc);
            void DrawGradientInfo(HDC hdc);
            void DrawCanvasInfo(HDC hdc);
            std::wstring ToWString(const std::string& str) { return std::wstring(str.begin(), str.end()); }
    };
}