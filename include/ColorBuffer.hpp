#pragma once

#include "Color.hpp"
#include "Gradient.hpp"

namespace KTLib
{
    class Gradient;

    class ColorBuffer
    {
        public:
            ColorBuffer() = default;
            ColorBuffer(int width, int height, Color col = Color::Transparent());
            ColorBuffer(unsigned int* buffer, int width, int height);
            ColorBuffer(Color** colors, int width, int height);
            ColorBuffer(const std::vector<std::vector<Color>>& colors);
            ColorBuffer(const Gradient& gradient, int width, int height);

            void Rotate(double angle);
            void Resize(int newWidth, int newHeight, int resizeImage, Color fillColor);
            void Scale(double scale);
            HBITMAP ToHBITMAP(int targetWidth = 0, int targetHeight = 0) const;
            static ColorBuffer* FromHBITMAP(HBITMAP hBitmap, int width = 0, int height = 0);
            HDC ToHDC(int width, int height) const;
            static ColorBuffer* FromHDC(HDC hdc, int x, int y, int width, int height);
            HICON ToHICON(int width, int height) const;
            static ColorBuffer* FromHICON(HICON hIcon);
            HCURSOR ToHCURSOR(int width, int height) const;
            static ColorBuffer* FromHCURSOR(HCURSOR hCursor);
            static ColorBuffer* FromHWND(HWND hWnd, int x, int y, int width, int height);
            void Draw(HWND hwnd, int x, int y) const;

            int GetWidth() const { return m_width; }
            int GetHeight() const { return m_height; }
            int GetSize() const { return m_colors.size() * sizeof(Color); }
            int GetStride() const { return std::round(GetSize() / m_height); }
            const Color& operator[](int index) const { return m_colors[index]; }

            Color& Get(int x, int y);
            const Color& Get(int x, int y) const;
            void Set(int x, int y, const Color& color);
            Color GetAt(int index) const;
            void SetAt(int index, const Color& color);
            void GetXY(int index, int& x, int& y) const;
            void GetIndex(int x, int y, int& index) const;

            void ShiftRed(int amount);
            void ShiftGreen(int amount);
            void ShiftBlue(int amount);
            void ShiftAlpha(int amount);

            void SetRed(int value);
            void SetGreen(int value);
            void SetBlue(int value);
            void SetAlpha(int value);

            void ApplyMatrix(const ColorMatrix& matrix);

            void Invert();
            void ShiftHue(double degrees);
            void Grayscale();
            void Sepia(double factor);
            void CrossProcess(double factor);
            void Moonlight(double factor);
            void VintageFilm(double factor);
            void Technicolor(double factor);
            void Polaroid(double factor);
            void Complement();
            void ShiftSaturation(double amount);
            void ShiftLightness(double amount);
            void ShiftValue(double amount);
            void ShiftIntensity(double amount);
            void ShiftWhiteLevel(double amount);
            void ShiftBlackLevel(double amount);
            void ShiftContrast(double amount);
            void Pixelate(int pixelSize);
            void Blur(int radius);
            void GaussianBlur(double sigma);
            void Sharpen(float amount);
            void Flip(bool horizontal);
            void Crop(int x, int y, int width, int height);
            void AdjustContrast(double factor);
            void AdjustColorBalance(double redFactor, double greenFactor, double blueFactor);

            void OverlayImage(const ColorBuffer& overlay, int x, int y, double opacity = 1.0);
            void Emboss();
            void EdgeDetect();
            void Vignette(double strength = 0.3, double radius = 1.0);
            void TwoColorNoise(double density = 0.05, const Color& saltColor = Color::White(), const Color& pepperColor = Color::Black());
            void GaussianNoise(double mean = 0.0, double stdDev = 10.0);
            void PerlinNoise(double frequency, double amplitude, int octaves, double persistence, double lacunarity);
            void SimplexNoise(double frequency, double amplitude, int octaves, double persistence, double lacunarity);
            void FractalBrownianMotion(double frequency, double amplitude, int octaves, double persistence, double lacunarity);
            void Voronoi(int numPoints, double falloff, double strength);
            void Plasma(double frequency, double phase);
            void DiamondSquare(double roughness, double waterLevel, double levelsPerStop);
            void Posterize(int levels);

            ColorBuffer* Copy() const;
            ColorBuffer* CopyRegion(int xmin, int ymin, int width, int height) const;
            size_t CountUniqueColors() const;
            void MapColors(int x, int y, int width, int height, unsigned int (*mapFunction)(int, int, unsigned int));
            void ForEach(const std::function<void(const Color&)>& func) const;
            int Find(const Color& color) const;
            int FindLast(const Color& color) const;
            void Swap(size_t index1, size_t index2);
            ColorBuffer Filter(const std::function<bool(const Color&)>& predicate) const;
            int Count(const Color& color) const;
            void Shuffle();
            void Clear();
            void Sort(const std::function<bool(const Color&, const Color&)>& compare);
            void AppendRight(const ColorBuffer& other);
            void AppendBottom(const ColorBuffer& other);
            std::vector<int> FindAll(const Color& color) const;
            Color CalculateAverageColor(int startX = 0, int startY = 0, int pixelWidth = 0, int pixelHeight = 0);

        private:
            alignas(32) std::vector<Color> m_colors;
            int m_width = 0;
            int m_height = 0;
    };
}