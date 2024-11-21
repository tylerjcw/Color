#include "ColorPicker.hpp"

#include <iostream>

namespace KTLib
{
    ColorPicker::ColorPicker(const std::string& colorSpace, const std::string& format) : m_colorSpace(colorSpace), m_format(format)
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"KTLibColorPicker";
        RegisterClassExW(&wc);

        m_previewWindow = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
            L"KTLibColorPicker",
            NULL,
            WS_POPUP,
            0, 0, 100, 100,
            NULL, NULL,
            GetModuleHandle(NULL),
            this
        );

        SetWindowDisplayAffinity(m_previewWindow, WDA_EXCLUDEFROMCAPTURE);
        m_buffer = new Gdiplus::Bitmap(m_previewWidth, m_previewHeight + m_textHeight);
    }

    ColorPicker::~ColorPicker()
    {
        delete m_buffer;
        Gdiplus::GdiplusShutdown(m_gdiplusToken);
        if (m_previewWindow) DestroyWindow(m_previewWindow);
    }

    void ColorPicker::SetFontSize(int size)
    {
        m_fontSize = size;
        if (m_previewWindow)
        {
            HDC dc = GetDC(m_previewWindow);
            Gdiplus::Graphics graphics(dc);
            m_textHeight = CalculateTextHeight(graphics);
            ReleaseDC(m_previewWindow, dc);
        }
    }

    void ColorPicker::SetPreviewSize(int width, int height)
    {
        m_basePreviewWidth = width;
        m_basePreviewHeight = height;
        m_previewWidth = width * m_zoomLevel;
        m_previewHeight = height * m_zoomLevel;
        SetWindowPos(m_previewWindow, NULL, 0, 0, m_previewWidth, m_previewHeight + m_textHeight, SWP_NOMOVE | SWP_NOZORDER);
    }

    void ColorPicker::SetZoomLevel(float zoom)
    {
        m_zoomLevel = zoom;
        int newWidth = m_basePreviewWidth * zoom;
        int newHeight = m_basePreviewHeight * zoom;
        m_previewWidth = newWidth;
        m_previewHeight = newHeight;

        HDC dc = GetDC(m_previewWindow);
        Gdiplus::Graphics graphics(dc);
        m_textHeight = CalculateTextHeight(graphics);
        ReleaseDC(m_previewWindow, dc);

        // Update both preview and window size
        delete m_buffer;
        m_buffer = new Gdiplus::Bitmap(newWidth, newHeight + m_textHeight);
        SetWindowPos(m_previewWindow, NULL, 0, 0, newWidth, newHeight + m_textHeight, SWP_NOMOVE | SWP_NOZORDER);
    }

    void ColorPicker::ZoomIn(float amount)
    {
        float newZoom = m_zoomLevel + amount;
        if (newZoom < 5.0f) SetZoomLevel(newZoom);
    }

    void ColorPicker::ZoomOut(float amount)
    {
        float newZoom = m_zoomLevel - amount;
        if (newZoom > 1.0f) SetZoomLevel(newZoom);
    }

    void ColorPicker::IncreaseCaptureSize(int amount)
    {
        int newSize = m_captureSize + amount;
        if (newSize <= 51) // Reasonable upper limit
        {
            SetCaptureSize(newSize);
        }
    }

    void ColorPicker::DecreaseCaptureSize(int amount)
    {
        int newSize = m_captureSize - amount;
        if (newSize >= 3) // Ensure we have at least a 3x3 capture area
        {
            SetCaptureSize(newSize);
        }
    }

    void ColorPicker::SetViewMode(ViewMode mode)
    {
        m_viewMode = mode;
    }

    void ColorPicker::ToggleViewMode(int direction)
    {
        int currentMode = static_cast<int>(m_viewMode);
        int numModes = static_cast<int>(ViewMode::Crosshair) + 1;
        int nextMode = (currentMode + direction + numModes) % numModes;
        m_viewMode = static_cast<ViewMode>(nextMode);
    }

    void ColorPicker::Start()
    {
        m_isActive = true;

        if (m_onStart) m_onStart();

        HDC dc = GetDC(m_previewWindow);
        Gdiplus::Graphics graphics(dc);
        m_textHeight = CalculateTextHeight(graphics);
        ReleaseDC(m_previewWindow, dc);

        delete m_buffer;
        m_buffer = new Gdiplus::Bitmap(m_previewWidth, m_previewHeight + m_textHeight);

        HCURSOR hCross = LoadCursor(NULL, IDC_CROSS);
        for (DWORD cursorId : {32512, 32513, 32514, 32515, 32516, 32631, 32640, 32641, 32642, 32643, 32644, 32645, 32646, 32648, 32649, 32650, 32651}) SetSystemCursor(CopyCursor(hCross), cursorId);

        CreateThread(NULL, 0, [](LPVOID param) -> DWORD __stdcall
        {
            ColorPicker* picker = static_cast<ColorPicker*>(param);
            ShowWindow(picker->m_previewWindow, SW_SHOW);

            while (picker->m_isActive)
            {
                picker->m_currentColor = picker->GetColorUnderCursor();
                picker->UpdatePreview();
            }
            return 0;
        }, this, 0, NULL); // End CreateThread
    }

    void ColorPicker::Stop()
    {
        m_isActive = false;
        ShowWindow(m_previewWindow, SW_HIDE);

        SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);

        Color* exitColor = new Color(m_currentColor);
        if (m_onExit) m_onExit(exitColor);
    }

    Color ColorPicker::GetColorUnderCursor()
    {
        HDC dc = GetDC(NULL);
        COLORREF pixel = GetPixel(dc, m_currentPosition.x - 2, m_currentPosition.y - 2);
        ReleaseDC(NULL, dc);

        return Color(
            GetRValue(pixel),
            GetGValue(pixel),
            GetBValue(pixel)
        );
    }

    const char* ColorPicker::GetFormattedValue() const
    {
        static std::string value;
        value = m_currentColor.ToString(m_colorSpace.c_str(), m_format);
        return value.c_str();
    }

    void ColorPicker::SetPosition(int x, int y)
    {
        SetCursorPos(x, y);
    }

    void ColorPicker::GetPosition(int& x, int& y)
    {
        x = m_currentPosition.x;
        y = m_currentPosition.y;
    }

    void ColorPicker::Move(int deltaX, int deltaY)
    {
        SetCursorPos(m_currentPosition.x + deltaX, m_currentPosition.y + deltaY);
    }

    float ColorPicker::CalculateTextHeight(Gdiplus::Graphics& graphics) const
    {
        Gdiplus::FontFamily fontFamily(m_fontName.c_str());
        Gdiplus::Font font(&fontFamily, GetScaledDimension(m_fontSize));

        std::string hexColor = "#FFFFFF"; // Use a test string to get consistent height
        std::wstring whexColor(hexColor.begin(), hexColor.end());

        Gdiplus::RectF bounds;
        Gdiplus::StringFormat format;
        format.SetAlignment(Gdiplus::StringAlignmentCenter);
        format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

        graphics.MeasureString(whexColor.c_str(), -1, &font, Gdiplus::PointF(0, 0), &format, &bounds);
        return bounds.Height + GetScaledDimension(1.0f);
    }

    void ColorPicker::CaptureScreen(Gdiplus::Bitmap& capture)
    {
        Gdiplus::Graphics captureGraphics(&capture);
        HDC screenDC = GetDC(NULL);
        HDC captureDC = captureGraphics.GetHDC();
        int halfSize = m_captureSize / 2;

        BitBlt(captureDC, 0, 0, m_captureSize + 2, m_captureSize + 2, screenDC, m_currentPosition.x - halfSize - 2, m_currentPosition.y - halfSize - 2, SRCCOPY);

        captureGraphics.ReleaseHDC(captureDC);
        ReleaseDC(NULL, screenDC);
    }

    void ColorPicker::DrawGrid(Gdiplus::Graphics& graphics)
    {
        float scaledWidth = GetScaledDimension(m_basePreviewWidth);
        float scaledHeight = GetScaledDimension(m_basePreviewHeight);
        float cellWidth = scaledWidth / m_captureSize;
        float cellHeight = scaledHeight / m_captureSize;

        Gdiplus::Pen gridPen(Gdiplus::Color(255, m_viewModeColor.r, m_viewModeColor.g, m_viewModeColor.b), 1.0f);

        for (int x = 0; x <= m_captureSize; x++)
        {
            float xPos = x * cellWidth;
            graphics.DrawLine(&gridPen, xPos, 0.0f, xPos, scaledHeight);
        }

        for (int y = 0; y <= m_captureSize; y++)
        {
            float yPos = y * cellHeight;
            graphics.DrawLine(&gridPen, 0.0f, yPos, scaledWidth, yPos);
        }
    }

    void ColorPicker::DrawDot(Gdiplus::Graphics& graphics)
    {
        float scaledWidth = GetScaledDimension(m_basePreviewWidth);
        float cellSize = scaledWidth / m_captureSize;
        float centerX = (m_captureSize / 2) * cellSize + (cellSize / 2);
        float centerY = (m_captureSize / 2) * cellSize + (cellSize / 2);
        float dotRadius = 2.0f;

        Gdiplus::Color dotColor(255, m_viewModeColor.r, m_viewModeColor.g, m_viewModeColor.b);

        Gdiplus::SolidBrush dotBrush(dotColor);
        graphics.FillEllipse(&dotBrush,
            centerX - dotRadius,
            centerY - dotRadius,
            dotRadius * 2.0f,
            dotRadius * 2.0f);
    }

    void ColorPicker::DrawCrosshair(Gdiplus::Graphics& graphics)
    {
        float scaledWidth = GetScaledDimension(m_basePreviewWidth);
        float scaledHeight = GetScaledDimension(m_basePreviewHeight);
        float cellSize = scaledWidth / m_captureSize;
        float centerX = (m_captureSize / 2) * cellSize + (cellSize / 2);
        float centerY = (m_captureSize / 2) * cellSize + (cellSize / 2);
        float thickness = 1.0f;

        Gdiplus::Pen crossPen(Gdiplus::Color(255, m_viewModeColor.r, m_viewModeColor.g, m_viewModeColor.b), thickness);
        graphics.DrawLine(&crossPen, centerX, 0.0f, centerX, scaledHeight);
        graphics.DrawLine(&crossPen, 0.0f, centerY, scaledWidth, centerY);
    }

    void ColorPicker::DrawHighlight(Gdiplus::Graphics& graphics)
    {
        float scaledWidth = GetScaledDimension(m_basePreviewWidth);
        float scaledHeight = GetScaledDimension(m_basePreviewHeight);
        float cellSize = scaledWidth / m_captureSize;
        float centerX = (m_captureSize / 2) * cellSize + (cellSize / 2);
        float centerY = (m_captureSize / 2) * cellSize + (cellSize / 2);
        float cellWidth = scaledWidth / m_captureSize;
        float cellHeight = scaledHeight / m_captureSize;

        Gdiplus::Color highlightGdiColor(255, m_highlightColor.r, m_highlightColor.g, m_highlightColor.b);

        switch (m_viewMode)
        {
            case ViewMode::Grid:
            {
                int gridCenterX = m_captureSize / 2;
                int gridCenterY = m_captureSize / 2;
                float highlightX = gridCenterX * cellWidth;
                float highlightY = gridCenterY * cellHeight;
                Gdiplus::Pen highlightPen(highlightGdiColor, 1.0f);
                graphics.DrawRectangle(&highlightPen, highlightX, highlightY, cellWidth, cellHeight);
                break;
            }
            case ViewMode::Dot:
            {
                Gdiplus::SolidBrush highlightBrush(highlightGdiColor);
                float highlightRadius = 2.0f;
                graphics.FillEllipse(&highlightBrush,
                    centerX - highlightRadius,
                    centerY - highlightRadius,
                    highlightRadius * 2.0f,
                    highlightRadius * 2.0f);
                break;
            }
            case ViewMode::Crosshair:
            {
                float halfZoom = cellWidth / 2.0f;
                Gdiplus::Pen highlightPen(highlightGdiColor, 1.0f);
                graphics.DrawLine(&highlightPen, centerX, centerY - halfZoom, centerX, centerY + halfZoom);
                graphics.DrawLine(&highlightPen, centerX - halfZoom, centerY, centerX + halfZoom, centerY);
                break;
            }
            case ViewMode::None:
                break;
        }
    }

    void ColorPicker::DrawText(Gdiplus::Graphics& graphics)
    {
        float scaledWidth = GetScaledDimension(m_basePreviewWidth);
        float scaledHeight = GetScaledDimension(m_basePreviewHeight);
        float scaledTextHeight = CalculateTextHeight(graphics);
        float scaledFontSize = GetScaledDimension(m_fontSize);

        // Background
        Gdiplus::SolidBrush textBgBrush(Gdiplus::Color(255, m_textBgColor.r, m_textBgColor.g, m_textBgColor.b));
        graphics.FillRectangle(&textBgBrush, 0.0f, scaledHeight, scaledWidth, scaledTextHeight);

        // Border
        Gdiplus::Pen borderPen(Gdiplus::Color(255, m_borderColor.r, m_borderColor.g, m_borderColor.b), GetScaledDimension(2.0f));
        graphics.DrawRectangle(&borderPen, 0.0f, 0.0f, scaledWidth, scaledHeight + scaledTextHeight);

        // Text
        Gdiplus::FontFamily fontFamily(m_fontName.c_str());
        Gdiplus::Font font(&fontFamily, scaledFontSize);
        Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, m_textColor.r, m_textColor.g, m_textColor.b));

        std::string formattedColor = m_currentColor.ToString("Hex", "#{R}{G}{B}");
        std::wstring wFormattedColor(formattedColor.begin(), formattedColor.end());

        Gdiplus::RectF layoutRect(0, scaledHeight, scaledWidth, scaledTextHeight);
        Gdiplus::StringFormat format;
        format.SetAlignment(Gdiplus::StringAlignmentCenter);
        format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

        graphics.DrawString(wFormattedColor.c_str(), -1, &font, layoutRect, &format, &textBrush);
    }

    void ColorPicker::UpdatePreview()
    {
        GetCursorPos(&m_currentPosition);

        HMONITOR hMonitor = MonitorFromPoint(m_currentPosition, MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitorInfo = {  };
        GetMonitorInfo(hMonitor, &monitorInfo);

        float dpiScale = GetDpiForWindow(m_previewWindow) / 96.0f;

        int scaledPreviewWidth = m_previewWidth * dpiScale;
        int scaledPreviewHeight = (m_previewHeight + m_textHeight) * dpiScale;

        int previewX = m_currentPosition.x + m_previewXOffset;
        int previewY = m_currentPosition.y + m_previewYOffset;

        int monitorCount = GetSystemMetrics(SM_CMONITORS);
        for (int i = 1; i <= monitorCount; i++)
        {
            MONITORINFO mi = {  };
            HMONITOR hMon = MonitorFromPoint({m_currentPosition.x, m_currentPosition.y}, MONITOR_DEFAULTTONEAREST);
            GetMonitorInfo(hMon, &mi);

            if (m_currentPosition.x >= mi.rcMonitor.left &&
                m_currentPosition.x < mi.rcMonitor.right &&
                m_currentPosition.y >= mi.rcMonitor.top &&
                m_currentPosition.y < mi.rcMonitor.bottom)
            {
                if (previewX + scaledPreviewWidth > mi.rcMonitor.right)
                {
                    previewX = m_currentPosition.x - m_previewXOffset * dpiScale - scaledPreviewWidth;
                }

                if (previewY + scaledPreviewHeight > mi.rcMonitor.bottom)
                {
                    previewY = m_currentPosition.y - m_previewYOffset * dpiScale - scaledPreviewHeight;
                }

                previewX = static_cast<int>(std::max(static_cast<LONG>(mi.rcMonitor.left),
                                          std::min(static_cast<LONG>(previewX),
                                          mi.rcMonitor.right - scaledPreviewWidth)));
                previewY = static_cast<int>(std::max(static_cast<LONG>(mi.rcMonitor.top),
                                          std::min(static_cast<LONG>(previewY),
                                          mi.rcMonitor.bottom - scaledPreviewHeight)));
                break;
            }
        }

        Gdiplus::Graphics graphics(m_buffer);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.Clear(Gdiplus::Color::Transparent);

        float scaledWidth = GetScaledDimension(m_basePreviewWidth);
        float scaledHeight = GetScaledDimension(m_basePreviewHeight);

        Gdiplus::Bitmap capture(m_captureSize, m_captureSize);
        CaptureScreen(capture);

        graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
        graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
        graphics.DrawImage(&capture, 0.0f, 0.0f, scaledWidth, scaledHeight);

        switch (m_viewMode)
        {
            case ViewMode::Grid: DrawGrid(graphics); break;
            case ViewMode::Dot: DrawDot(graphics); break;
            case ViewMode::Crosshair: DrawCrosshair(graphics); break;
            case ViewMode::None: break;
        }

        if (m_highlightCenter) DrawHighlight(graphics);
        DrawText(graphics);

        HDC windowDC = GetDC(m_previewWindow);
        Gdiplus::Graphics(windowDC).DrawImage(m_buffer, 0, 0);
        ReleaseDC(m_previewWindow, windowDC);

        SetWindowPos(m_previewWindow, HWND_TOPMOST, previewX, previewY,
                    m_previewWidth, m_previewHeight + m_textHeight, SWP_NOACTIVATE);

        SetForegroundWindow(m_previewWindow);

        if (m_onUpdate) m_onUpdate(new Color(m_currentColor));
    }
}