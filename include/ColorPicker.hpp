#pragma once

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include "Color.hpp"

#include <windows.h>
#include <gdiplus.h>
#include <shellscalingapi.h>

namespace KTLib
{
    class ColorPicker
    {
        public:
            ColorPicker(const std::string& colorSpace = "Hex", const std::string& format = "#{R}{G}{B}");
            ~ColorPicker();

            void Start();
            void Stop();
            void Toggle() { !m_isActive ? Start() : Stop(); };
            bool IsActive() const { return m_isActive; }
            Color GetCurrentColor() const { return m_currentColor; }
            const char* GetFormattedValue() const;

            void SetPosition(int x, int y);
            void GetPosition(int& x, int& y);
            void Move(int deltaX, int deltaY);
            void SetPreviewSize(int width, int height);
            void SetCaptureSize(int size) { m_captureSize = size; }
            void SetZoomLevel(float zoom);
            void SetFontSize(int size);
            void SetFontName(const wchar_t* name) { m_fontName = name; }
            void SetTextColor(const Color& color) { m_textColor = color; }
            void SetTextBackgroundColor(const Color& color) { m_textBgColor = color; }
            void SetBorderColor(const Color& color) { m_borderColor = color; }
            void SetViewModeColor(const Color& color) { m_viewModeColor = color; }
            void SetHighlightColor(const Color& color) { m_highlightColor = color; }

            void ZoomIn(float amount = 0.1f);
            void ZoomOut(float amount = 0.1f);
            void IncreaseCaptureSize(int amount = 1);
            void DecreaseCaptureSize(int amount = 1);

            enum class ViewMode { None, Grid, Dot, Crosshair };
            void SetViewMode(ViewMode mode);
            void ToggleViewMode(int direction = 1);
            void ToggleHighlight() { m_highlightCenter = !m_highlightCenter; }

            using SimpleCallback = void(*)();
            using ColorCallback = void(*)(Color*);
            void SetOnStart(SimpleCallback callback) { m_onStart = callback; }
            void SetOnUpdate(ColorCallback callback) { m_onUpdate = callback; }
            void SetOnExit(ColorCallback callback) { m_onExit = callback; }


        private:
            HWND m_previewWindow;
            Color m_currentColor;
            POINT m_currentPosition;
            int m_previewXOffset = 10;
            int m_previewYOffset = 10;
            bool m_isActive = false;

            std::string m_colorSpace;
            std::string m_format;

            int m_previewWidth = 128;
            int m_previewHeight = 128;
            int m_basePreviewWidth = 128;
            int m_basePreviewHeight = 128;
            int m_captureSize = 19;
            int m_fontSize = 14;
            float m_zoomLevel = 1;
            std::wstring m_fontName = L"Consolas";
            int m_textHeight = 14;
            Color m_textColor = Color::White();
            Color m_textBgColor = Color::Black();
            Color m_borderColor = Color::Black();
            Color m_viewModeColor = Color::Black();
            Color m_highlightColor = Color::White();

            Gdiplus::Bitmap* m_buffer;
            ULONG_PTR m_gdiplusToken;

            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, msg, wParam, lParam); }
            float GetDpiScale() const { return GetDpiForWindow(m_previewWindow) / 96.0f; }
            float GetScaledDimension(float value) const { return value * m_zoomLevel * GetDpiScale(); }
            float CalculateTextHeight(Gdiplus::Graphics& graphics) const;
            void CaptureScreen(Gdiplus::Bitmap& capture);
            void DrawGrid(Gdiplus::Graphics& graphics);
            void DrawDot(Gdiplus::Graphics& graphics);
            void DrawCrosshair(Gdiplus::Graphics& graphics);
            void DrawHighlight(Gdiplus::Graphics& graphics);
            void DrawText(Gdiplus::Graphics& graphics);
            void UpdatePreview();
            Color GetColorUnderCursor();

            ViewMode m_viewMode = ViewMode::Grid;
            bool m_highlightCenter = true;

            SimpleCallback m_onStart = nullptr;
            ColorCallback m_onUpdate = nullptr;
            ColorCallback m_onExit = nullptr;
    };
}