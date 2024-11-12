#include "Showcase.hpp"

#include <iostream>

namespace KTLib
{
    Showcase::Showcase(Color* color, const char* title)
        : m_hwnd(nullptr)
        , m_title(title)
        , m_color(color)
        , m_gradient(nullptr)
        , m_isColor(true)
    { }

    Showcase::Showcase(Gradient* gradient, const char* title)
        : m_hwnd(nullptr)
        , m_title(title)
        , m_color(nullptr)
        , m_gradient(gradient)
        , m_isColor(false)
    { }

    Showcase::~Showcase() { if (m_hwnd) DestroyWindow(m_hwnd); }

    int Showcase::Show()
    {
        // Register window class
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"KTLibShowcase";
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassExW(&wc);

        // Create Consolas font
        m_hFont = CreateFontW(
            -14,                    // Height (negative for character height)
            0,                      // Width
            0,                      // Escapement
            0,                      // Orientation
            FW_NORMAL,             // Weight
            FALSE,                 // Italic
            FALSE,                 // Underline
            FALSE,                 // StrikeOut
            DEFAULT_CHARSET,       // CharSet
            OUT_DEFAULT_PRECIS,    // OutputPrecision
            CLIP_DEFAULT_PRECIS,   // ClipPrecision
            CLEARTYPE_QUALITY,     // Quality
            FIXED_PITCH | FF_MODERN, // PitchAndFamily
            L"Consolas"            // Face Name
        );

        // Calculate metrics using our new font
        HDC hdc = GetDC(NULL);
        HFONT hOldFont = (HFONT)SelectObject(hdc, m_hFont);
        TEXTMETRICW tm;
        GetTextMetricsW(hdc, &tm);
        m_fontHeight = tm.tmHeight;
        SelectObject(hdc, hOldFont);
        ReleaseDC(NULL, hdc);

        wchar_t wtitle[256] = {0};
        MultiByteToWideChar(CP_UTF8, 0, m_title.c_str(), -1, wtitle, 256);

        m_hwnd = CreateWindowExW(
            WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
            L"KTLibShowcase",
            wtitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            400, 235,
            NULL, NULL,
            GetModuleHandle(NULL),
            this
        );

        CreateControls();
        LayoutControls();
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);

        // Message loop
        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
    }

    LRESULT CALLBACK Showcase::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Showcase* showcase = nullptr;

        if (msg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            showcase = reinterpret_cast<Showcase*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(showcase));
        }
        else
        {
            showcase = reinterpret_cast<Showcase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        switch (msg)
        {
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                if (showcase)
                {
                    showcase->DrawPreview(hdc);
                    if (showcase->m_isColor) showcase->DrawColorInfo(hdc);
                    else if (showcase->m_gradient) showcase->DrawGradientInfo(hdc);
                }

                EndPaint(hwnd, &ps);
                return 0;
            }

            case WM_COMMAND:
            {
                if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
                {
                    DestroyWindow(hwnd);
                    return 0;
                }
                break;
            }

            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        }

        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    void Showcase::CreateControls()
    {
        CreateWindowExW(
            0, L"BUTTON", L"OK",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, 0, 0,  // Position and size set in LayoutControls
            m_hwnd, (HMENU)IDOK, GetModuleHandle(NULL), NULL
        );
    }

    void Showcase::LayoutControls()
    {
        RECT clientRect;
        GetClientRect(m_hwnd, &clientRect);

        const int padding = 20;
        const int buttonHeight = 25;

        // Button stays at bottom
        const int buttonWidth = 80;
        SetWindowPos(
            GetDlgItem(m_hwnd, IDOK),
            NULL,
            (clientRect.right - buttonWidth) / 2,
            clientRect.bottom - padding - buttonHeight,
            buttonWidth,
            buttonHeight,
            SWP_NOZORDER
        );
    }

    void Showcase::DrawPreview(HDC hdc)
    {
        RECT previewRect = { 10, 10, 100, 100 };  // 100x100 square with 20px padding

        if (m_isColor && m_color)
        {
            HBRUSH brush = CreateSolidBrush(RGB(m_color->r, m_color->g, m_color->b));
            FillRect(hdc, &previewRect, brush);
            DeleteObject(brush);

            // Add border
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
            HPEN oldPen = (HPEN)SelectObject(hdc, pen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, previewRect.left, previewRect.top, previewRect.right, previewRect.bottom);
            SelectObject(hdc, oldPen);
            DeleteObject(pen);
        }
        else if (m_gradient)
        {
            // Create compatible DC for the gradient bitmap
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP hBitmap = m_gradient->CreateHBITMAP(100, 100);  // Match preview size
            SelectObject(memDC, hBitmap);

            // Draw the gradient bitmap
            BitBlt(hdc, previewRect.left, previewRect.top, 100, 100, memDC, 0, 0, SRCCOPY);

            // Clean up
            DeleteDC(memDC);
            DeleteObject(hBitmap);
        }
    }

    void Showcase::DrawColorInfo(HDC hdc)
    {
        HFONT hOldFont = (HFONT)SelectObject(hdc, m_hFont);
        SetBkMode(hdc, TRANSPARENT);

        RECT infoRect = { 120, 10, 380, 200 };

        std::string hexInfo = "Hex: " + m_color->ToString("Hex", "#{R}{G}{B}");
        std::string rgbInfo = "RGB: " + m_color->ToString("RGB", "rgb({R}, {G}, {B})");
        std::string hslInfo = "HSL: " + m_color->ToString("HSL", "hsl({H}, {S}%, {L}%)");

        std::wstring whexInfo(hexInfo.begin(), hexInfo.end());
        std::wstring wrgbInfo(rgbInfo.begin(), rgbInfo.end());
        std::wstring whslInfo(hslInfo.begin(), hslInfo.end());

        DrawTextW(hdc, whexInfo.c_str(), -1, &infoRect, DT_LEFT);
        infoRect.top += m_fontHeight;
        DrawTextW(hdc, wrgbInfo.c_str(), -1, &infoRect, DT_LEFT);
        infoRect.top += m_fontHeight;
        DrawTextW(hdc, whslInfo.c_str(), -1, &infoRect, DT_LEFT);

        SelectObject(hdc, hOldFont);
    }

    void Showcase::DrawGradientInfo(HDC hdc)
    {
        HFONT hOldFont = (HFONT)SelectObject(hdc, m_hFont);
        SetBkMode(hdc, TRANSPARENT);

        RECT infoRect = {
            120,                                    // x
            10,  // y
            280,                                    // width
            200                                     // height
        };

        int type = static_cast<int>(m_gradient->GetType());
        std::string typeStr = "";

        switch (type)
        {
            case 0:
            {
                typeStr = "Linear";
                break;
            }
            case 1:
            {
                typeStr = "Radial";
                break;
            }
            case 2:
            {
                typeStr = "Conical";
                break;
            }
        }

        std::vector<std::string> properties = {
            "Type: " + typeStr,
            "Angle: " + std::to_string(m_gradient->GetAngle()),
            "Vertices: " + std::to_string(m_gradient->GetVertices()),
            "Edge Sharpness: " + std::to_string(m_gradient->GetEdgeSharpness()),
            "Wavelength: " + std::to_string(m_gradient->GetWavelength()),
            "Amplitude: " + std::to_string(m_gradient->GetAmplitude()),
            "Repetitions: " + std::to_string(m_gradient->GetRepetitions()),
            "Color Stops: " + std::to_string(m_gradient->GetColorStops().size())
        };

        for (const auto& prop : properties)
        {
            DrawTextW(hdc, ToWString(prop).c_str(), -1, &infoRect, DT_LEFT);
            infoRect.top += m_fontHeight;
        }

        SelectObject(hdc, hOldFont);
    }
}