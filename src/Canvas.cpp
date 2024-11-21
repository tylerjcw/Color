#define NOMINMAX

#include "../include/Canvas.hpp"

#include <unordered_set>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <array>

namespace KTLib
{
    #pragma region Constructors
    Canvas::Canvas(int width, int height, Color col) : m_width(width), m_height(height)
    {
        m_colors.resize(width * height, col);
    }

    Canvas::Canvas(unsigned int* buffer, int width, int height) : m_width(width), m_height(height)
    {
        m_colors.resize(width * height);
        for (int i = 0; i < width * height; ++i)
        {
            m_colors[i] = Color(buffer[i]);
        }
    }

    Canvas::Canvas(Color** colors, int width, int height) : m_width(width), m_height(height)
    {
        m_colors.reserve(width * height);
        for (int i = 0; i < width * height; ++i)
        {
            m_colors.push_back(*colors[i]);
        }
    }

    Canvas::Canvas(const std::vector<std::vector<Color>>& colors) : m_width(colors.empty() ? 0 : colors[0].size()), m_height(colors.size())
    {
        m_colors.reserve(m_width * m_height);
        for (const auto& row : colors)
        {
            m_colors.insert(m_colors.end(), row.begin(), row.end());
        }
    }

    Canvas::Canvas(const Gradient& gradient, int width, int height) : m_width(width), m_height(height)
    {
        m_colors.resize(width * height);

        const float centerX = width / 2.0f;
        const float centerY = height / 2.0f;
        const float maxRadius = std::max(centerX, centerY);

        #pragma omp parallel for
        for (int i = 0; i < width * height; ++i)
        {
            const int x = i % width;
            const int y = i / width;
            const float position = gradient.CalculatePosition(x, y, centerX, centerY, maxRadius);
            m_colors[i] = gradient.GetColorAt(position);
        }
    }
    #pragma endregion

    #pragma region Canvas Functions
    Color& Canvas::Get(int x, int y)                   { return m_colors[y * m_width + x]; }
    const Color& Canvas::Get(int x, int y) const       { return m_colors[y * m_width + x]; }
    void Canvas::Set(int x, int y, const Color& color) { m_colors[y * m_width + x] = color; }

    Color Canvas::GetAt(int index) const              { return m_colors[index]; }
    void Canvas::SetAt(int index, const Color& color) { m_colors[index] = color; }

    void Canvas::GetXY(int index, int& x, int& y) const   { y = index / m_width, x = index % m_width; }
    void Canvas::GetIndex(int x, int y, int& index) const { index = y * m_width + x; }

    void Canvas::ShiftRed(int amount)   { for (auto& color : m_colors) color.SetRed(std::clamp(color.GetRed() + amount, 0, 255)); }
    void Canvas::ShiftGreen(int amount) { for (auto& color : m_colors) color.SetGreen(std::clamp(color.GetGreen() + amount, 0, 255)); }
    void Canvas::ShiftBlue(int amount)  { for (auto& color : m_colors) color.SetBlue(std::clamp(color.GetBlue() + amount, 0, 255)); }
    void Canvas::ShiftAlpha(int amount) { for (auto& color : m_colors) color.SetAlpha(std::clamp(color.GetAlpha() + amount, 0, 255)); }

    void Canvas::SetRed(int value)   { for (auto& color : m_colors) color.SetRed(std::clamp(value, 0, 255)); }
    void Canvas::SetGreen(int value) { for (auto& color : m_colors) color.SetGreen(std::clamp(value, 0, 255)); }
    void Canvas::SetBlue(int value)  { for (auto& color : m_colors) color.SetBlue(std::clamp(value, 0, 255)); }
    void Canvas::SetAlpha(int value) { for (auto& color : m_colors) color.SetAlpha(std::clamp(value, 0, 255)); }
    void Canvas::ApplyMatrix(const ColorMatrix& matrix) { std::transform(m_colors.begin(), m_colors.end(), m_colors.begin(), [&matrix](const Color& color) { return color * matrix; }); }
    #pragma endregion

    #pragma region Color Modification Functions
    void Canvas::Invert()                         { for (auto& color : m_colors) color.Invert(); }
    void Canvas::ShiftHue(double degrees)         { for (auto& color : m_colors) color.ShiftHue(degrees); }
    void Canvas::Grayscale()                      { for (auto& color : m_colors) color.Grayscale(); }
    void Canvas::Sepia(double factor)             { for (auto& color : m_colors) color.Sepia(factor); }
    void Canvas::CrossProcess(double factor)      { for (auto& color : m_colors) color.CrossProcess(factor); }
    void Canvas::Moonlight(double factor)         { for (auto& color : m_colors) color.Moonlight(factor); }
    void Canvas::VintageFilm(double factor)       { for (auto& color : m_colors) color.VintageFilm(factor); }
    void Canvas::Technicolor(double factor)       { for (auto& color : m_colors) color.Technicolor(factor); }
    void Canvas::Polaroid(double factor)          { for (auto& color : m_colors) color.Polaroid(factor); }
    void Canvas::Complement()                     { for (auto& color : m_colors) color.Complement(); }
    void Canvas::ShiftSaturation(double amount)   { for (auto& color : m_colors) color.ShiftSaturation(amount); }
    void Canvas::ShiftLightness(double amount)    { for (auto& color : m_colors) color.ShiftLightness(amount); }
    void Canvas::ShiftValue(double amount)        { for (auto& color : m_colors) color.ShiftValue(amount); }
    void Canvas::ShiftIntensity(double amount)    { for (auto& color : m_colors) color.ShiftIntensity(amount); }
    void Canvas::ShiftWhiteLevel(double amount)   { for (auto& color : m_colors) color.ShiftWhiteLevel(amount); }
    void Canvas::ShiftBlackLevel(double amount)   { for (auto& color : m_colors) color.ShiftBlackLevel(amount); }
    void Canvas::ShiftContrast(double amount)     { for (auto& color : m_colors) color.ShiftContrast(amount); }

    void Canvas::Pixelate(int pixelSize)
    {
        if (pixelSize <= 1) return;

        int numPixelsX = (m_width + pixelSize - 1) / pixelSize;
        int numPixelsY = (m_height + pixelSize - 1) / pixelSize;

        #pragma omp parallel for
        for (int i = 0; i < numPixelsX * numPixelsY; ++i)
        {
            int blockX = (i % numPixelsX) * pixelSize;
            int blockY = (i / numPixelsX) * pixelSize;

            Color avgColor = CalculateAverageColor(blockX, blockY, pixelSize, pixelSize);

            int endX = std::min(blockX + pixelSize, m_width);
            int endY = std::min(blockY + pixelSize, m_height);

            for (int j = blockY * m_width + blockX; j < endY * m_width; j += m_width)
            {
                for (int k = 0; k < endX - blockX; ++k)
                {
                    m_colors[j + k] = avgColor;
                }
            }
        }
    }

    void Canvas::Blur(int radius)
    {
        if (radius <= 0) return;

        std::vector<Color> tempBuffer(m_width * m_height);

        // Horizontal pass
        #pragma omp parallel for
        for (int y = 0; y < m_height; ++y)
        {
            int sum[4] = {0, 0, 0, 0};
            for (int x = 0; x < radius; ++x)
            {
                Color pixel = m_colors[y * m_width + x];
                sum[0] += pixel.GetRed();
                sum[1] += pixel.GetGreen();
                sum[2] += pixel.GetBlue();
                sum[3] += pixel.GetAlpha();
            }

            for (int x = 0; x < m_width; ++x)
            {
                if (x > radius)
                {
                    Color removePixel = m_colors[y * m_width + x - radius - 1];
                    sum[0] -= removePixel.GetRed();
                    sum[1] -= removePixel.GetGreen();
                    sum[2] -= removePixel.GetBlue();
                    sum[3] -= removePixel.GetAlpha();
                }
                if (x + radius < m_width)
                {
                    Color addPixel = m_colors[y * m_width + x + radius];
                    sum[0] += addPixel.GetRed();
                    sum[1] += addPixel.GetGreen();
                    sum[2] += addPixel.GetBlue();
                    sum[3] += addPixel.GetAlpha();
                }
                int count = std::min(x + radius + 1, m_width) - std::max(x - radius, 0);
                tempBuffer[y * m_width + x] = Color(sum[0] / count, sum[1] / count, sum[2] / count, sum[3] / count);
            }
        }

        // Vertical pass
        #pragma omp parallel for
        for (int x = 0; x < m_width; ++x)
        {
            int sum[4] = {0, 0, 0, 0};
            for (int y = 0; y < radius; ++y)
            {
                Color pixel = tempBuffer[y * m_width + x];
                sum[0] += pixel.GetRed();
                sum[1] += pixel.GetGreen();
                sum[2] += pixel.GetBlue();
                sum[3] += pixel.GetAlpha();
            }

            for (int y = 0; y < m_height; ++y)
            {
                if (y > radius)
                {
                    Color removePixel = tempBuffer[(y - radius - 1) * m_width + x];
                    sum[0] -= removePixel.GetRed();
                    sum[1] -= removePixel.GetGreen();
                    sum[2] -= removePixel.GetBlue();
                    sum[3] -= removePixel.GetAlpha();
                }
                if (y + radius < m_height)
                {
                    Color addPixel = tempBuffer[(y + radius) * m_width + x];
                    sum[0] += addPixel.GetRed();
                    sum[1] += addPixel.GetGreen();
                    sum[2] += addPixel.GetBlue();
                    sum[3] += addPixel.GetAlpha();
                }
                int count = std::min(y + radius + 1, m_height) - std::max(y - radius, 0);
                m_colors[y * m_width + x] = Color(sum[0] / count, sum[1] / count, sum[2] / count, sum[3] / count);
            }
        }
    }

    void Canvas::GaussianBlur(double sigma)
    {
        int radius = static_cast<int>(ceil(3 * sigma));
        std::vector<double> kernel(2 * radius + 1);
        double sum = 0.0;

        // Generate 1D Gaussian kernel
        for (int i = -radius; i <= radius; ++i)
        {
            kernel[i + radius] = exp(-(i * i) / (2 * sigma * sigma));
            sum += kernel[i + radius];
        }

        // Normalize kernel
        for (auto& k : kernel)
            k /= sum;

        std::vector<Color> tempBuffer(m_width * m_height);

        // Horizontal pass
        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            double r = 0, g = 0, b = 0, a = 0;
            for (int j = -radius; j <= radius; ++j)
            {
                int sx = std::clamp(x + j, 0, m_width - 1);
                const Color& pixel = m_colors[y * m_width + sx];
                double weight = kernel[j + radius];
                r += pixel.r * weight;
                g += pixel.g * weight;
                b += pixel.b * weight;
                a += pixel.a * weight;
            }
            tempBuffer[i] = Color(
                std::clamp(static_cast<int>(r), 0, 255),
                std::clamp(static_cast<int>(g), 0, 255),
                std::clamp(static_cast<int>(b), 0, 255),
                std::clamp(static_cast<int>(a), 0, 255)
            );
        }

        // Vertical pass
        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            double r = 0, g = 0, b = 0, a = 0;
            for (int j = -radius; j <= radius; ++j)
            {
                int sy = std::clamp(y + j, 0, m_height - 1);
                const Color& pixel = tempBuffer[sy * m_width + x];
                double weight = kernel[j + radius];
                r += pixel.r * weight;
                g += pixel.g * weight;
                b += pixel.b * weight;
                a += pixel.a * weight;
            }
            m_colors[i] = Color(
                std::clamp(static_cast<int>(r), 0, 255),
                std::clamp(static_cast<int>(g), 0, 255),
                std::clamp(static_cast<int>(b), 0, 255),
                std::clamp(static_cast<int>(a), 0, 255)
            );
        }
    }

    void Canvas::Sharpen(float amount)
    {
        if (amount <= 0) return;

        std::vector<Color> newColors(m_width * m_height);

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            Color center = m_colors[i];
            Color left = (x > 0) ? m_colors[i - 1] : center;
            Color right = (x < m_width - 1) ? m_colors[i + 1] : center;
            Color top = (y > 0) ? m_colors[i - m_width] : center;
            Color bottom = (y < m_height - 1) ? m_colors[i + m_width] : center;

            Color sharpened = center * (4 * amount + 1) - (left + right + top + bottom) * amount;
            newColors[i] = sharpened;
        }

        m_colors = std::move(newColors);
    }

    void Canvas::Flip(bool horizontal)
    {
        if (horizontal)
        {
            #pragma omp parallel for
            for (int i = 0; i < m_height * m_width / 2; ++i)
            {
                int y = i / (m_width / 2);
                int x = i % (m_width / 2);
                std::swap(m_colors[y * m_width + x], m_colors[y * m_width + (m_width - 1 - x)]);
            }
        }
        else // vertical flip
        {
            #pragma omp parallel for
            for (int i = 0; i < m_width * m_height / 2; ++i)
            {
                int x = i % m_width;
                int y = i / m_width;
                std::swap(m_colors[y * m_width + x], m_colors[(m_height - 1 - y) * m_width + x]);
            }
        }
    }

    void Canvas::Crop(int x, int y, int width, int height)
    {
        if (x < 0 || y < 0 || x + width > m_width || y + height > m_height)
        {
            throw std::out_of_range("Crop dimensions are out of bounds");
        }

        std::vector<Color> newColors;
        newColors.reserve(width * height);

        #pragma omp parallel for
        for (int i = 0; i < width * height; ++i)
        {
            int newX = i % width;
            int newY = i / width;
            newColors.push_back(m_colors[(y + newY) * m_width + (x + newX)]);
        }

        m_colors = std::move(newColors);
        m_width = width;
        m_height = height;
    }

    void Canvas::AdjustContrast(double factor)
    {
        #pragma omp parallel for
        for (auto& color : m_colors)
        {
            int r = static_cast<int>((color.GetRed() - 128) * factor + 128);
            int g = static_cast<int>((color.GetGreen() - 128) * factor + 128);
            int b = static_cast<int>((color.GetBlue() - 128) * factor + 128);
            color = Color(std::clamp(r, 0, 255), std::clamp(g, 0, 255), std::clamp(b, 0, 255), color.GetAlpha());
        }
    }

    void Canvas::AdjustColorBalance(double redFactor, double greenFactor, double blueFactor)
    {
        #pragma omp parallel for
        for (auto& color : m_colors)
        {
            int r = static_cast<int>(color.GetRed() * redFactor);
            int g = static_cast<int>(color.GetGreen() * greenFactor);
            int b = static_cast<int>(color.GetBlue() * blueFactor);
            color = Color(std::clamp(r, 0, 255), std::clamp(g, 0, 255), std::clamp(b, 0, 255), color.GetAlpha());
        }
    }

    void Canvas::OverlayImage(const Canvas& overlay, int x, int y, double opacity)
    {
        #pragma omp parallel for
        for (int i = 0; i < overlay.GetWidth() * overlay.GetHeight(); ++i)
        {
            int dx = i % overlay.GetWidth();
            int dy = i / overlay.GetWidth();
            int destX = x + dx;
            int destY = y + dy;

            if (destX >= 0 && destX < m_width && destY >= 0 && destY < m_height)
            {
                const Color& overlayColor = overlay.GetAt(i);
                if (overlayColor.GetAlpha() > 0) // Only blend non-transparent pixels
                {
                    double alpha = (overlayColor.GetAlpha() / 255.0) * opacity;
                    Color& baseColor = m_colors[destY * m_width + destX];
                    baseColor = Color::Mix(baseColor, overlayColor, alpha);
                }
            }
        }
    }

    void Canvas::Emboss()
    {
        Canvas temp = *this;

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            if (x == 0 || y == 0 || x == m_width - 1 || y == m_height - 1)
            {
                m_colors[i] = Color(128, 128, 128);
                continue;
            }

            Color topLeft = temp.Get(x - 1, y - 1);
            Color bottomRight = temp.Get(x + 1, y + 1);

            int r = std::clamp(bottomRight.GetRed() - topLeft.GetRed() + 128, 0, 255);
            int g = std::clamp(bottomRight.GetGreen() - topLeft.GetGreen() + 128, 0, 255);
            int b = std::clamp(bottomRight.GetBlue() - topLeft.GetBlue() + 128, 0, 255);

            m_colors[i] = Color(r, g, b);
        }
    }

    void Canvas::EdgeDetect()
    {
        Canvas temp = *this;
        int kernel[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            if (x == 0 || y == 0 || x == m_width - 1 || y == m_height - 1)
            {
                m_colors[i] = Color(0, 0, 0);
                continue;
            }

            int r = 0, g = 0, b = 0;
            for (int k = 0; k < 9; ++k)
            {
                int kx = k % 3 - 1;
                int ky = k / 3 - 1;
                Color neighborColor = temp.Get(x + kx, y + ky);
                r += neighborColor.GetRed() * kernel[ky+1][kx+1];
                g += neighborColor.GetGreen() * kernel[ky+1][kx+1];
                b += neighborColor.GetBlue() * kernel[ky+1][kx+1];
            }

            m_colors[i] = Color(std::clamp(r, 0, 255), std::clamp(g, 0, 255), std::clamp(b, 0, 255));
        }
    }

    void Canvas::Vignette(double strength, double radius)
    {
        int centerX = m_width / 2;
        int centerY = m_height / 2;
        int size = m_width * m_height;

        #pragma omp parallel for
        for (int i = 0; i < size; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;
            double dx = (x - centerX) / static_cast<double>(centerX);
            double dy = (y - centerY) / static_cast<double>(centerY);
            double distance = std::sqrt(dx * dx + dy * dy);

            double falloff = 1.0 - std::pow(std::min(distance / radius, 1.0), 2.0);
            double vignette = 1.0 - strength * (1.0 - falloff);

            m_colors[i].ShiftValue(-100 * (1.0 - vignette));
            m_colors[i].ShiftSaturation(-50 * (1.0 - vignette));
        }
    }

    void Canvas::TwoColorNoise(double density, const Color& colorOne, const Color& colorTwo)
    {
        std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_real_distribution<> dis(0.0, 1.0);

        int totalPixels = m_width * m_height;

        #pragma omp parallel for
        for (int i = 0; i < totalPixels; ++i)
            if (dis(gen) < density)
                m_colors[i] = (dis(gen) < 0.5) ? colorTwo : colorOne;
    }

    void Canvas::GaussianNoise(double mean, double stdDev)
    {
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> d{mean, stdDev};

        #pragma omp parallel for
        for (auto& color : m_colors)
        {
            int noise = static_cast<int>(d(gen));
            color += noise;
        }
    }

    void Canvas::PerlinNoise(double frequency, double amplitude, int octaves, double persistence, double lacunarity)
    {
        int seed = std::random_device{}();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 255);

        std::vector<int> p(512);
        for (int i = 0; i < 256; ++i) p[i] = i;
        std::shuffle(p.begin(), p.begin() + 256, gen);
        for (int i = 0; i < 256; ++i) p[256 + i] = p[i];

        auto fade = [](double t) { return t * t * t * (t * (t * 6 - 15) + 10); };
        auto lerp = [](double t, double a, double b) { return a + t * (b - a); };
        auto grad = [](int hash, double x, double y, double z) {
            int h = hash & 15;
            double u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        };

        auto noise = [&](double x, double y) {
            int X = static_cast<int>(std::floor(x)) & 255;
            int Y = static_cast<int>(std::floor(y)) & 255;
            x -= std::floor(x);
            y -= std::floor(y);
            double u = fade(x), v = fade(y);
            int A = p[X] + Y, AA = p[A], AB = p[A + 1], B = p[X + 1] + Y, BA = p[B], BB = p[B + 1];
            return lerp(v, lerp(u, grad(p[AA], x, y, 0), grad(p[BA], x - 1, y, 0)),
                           lerp(u, grad(p[AB], x, y - 1, 0), grad(p[BB], x - 1, y - 1, 0)));
        };

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            double total = 0;
            double freq = frequency;
            double amp = amplitude;

            for (int o = 0; o < octaves; ++o)
            {
                double sampleX = x * freq / m_width;
                double sampleY = y * freq / m_height;
                total += noise(sampleX, sampleY) * amp;

                freq *= lacunarity;
                amp *= persistence;
            }

            total = (total + 1) / 2;  // Normalize to 0-1
            int noiseColor = static_cast<int>(total * 255);

            Color& color = m_colors[i];
            color.r = std::clamp(color.r + noiseColor, 0, 255);
            color.g = std::clamp(color.g + noiseColor, 0, 255);
            color.b = std::clamp(color.b + noiseColor, 0, 255);
        }
    }

    void Canvas::SimplexNoise(double frequency, double amplitude, int octaves, double persistence, double lacunarity)
    {
        int seed = std::random_device{}();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 255);

        std::vector<int> perm(512);
        for (int i = 0; i < 256; i++) perm[i] = i;
        std::shuffle(perm.begin(), perm.begin() + 256, gen);
        for (int i = 0; i < 256; i++) perm[i + 256] = perm[i];

        auto dot = [](const std::array<double, 3>& g, double x, double y) {
            return g[0] * x + g[1] * y;
        };

        const std::array<std::array<double, 3>, 12> grad3 = {{
            {{1,1,0}},{{-1,1,0}},{{1,-1,0}},{{-1,-1,0}},
            {{1,0,1}},{{-1,0,1}},{{1,0,-1}},{{-1,0,-1}},
            {{0,1,1}},{{0,-1,1}},{{0,1,-1}},{{0,-1,-1}}
        }};

        auto noise = [&](double xin, double yin) {
            const double F2 = 0.5 * (std::sqrt(3.0) - 1.0);
            const double G2 = (3.0 - std::sqrt(3.0)) / 6.0;
            double n0, n1, n2;

            double s = (xin + yin) * F2;
            int i = std::floor(xin + s);
            int j = std::floor(yin + s);
            double t = (i + j) * G2;
            double X0 = i - t;
            double Y0 = j - t;
            double x0 = xin - X0;
            double y0 = yin - Y0;

            int i1, j1;
            if (x0 > y0) { i1 = 1; j1 = 0; }
            else { i1 = 0; j1 = 1; }

            double x1 = x0 - i1 + G2;
            double y1 = y0 - j1 + G2;
            double x2 = x0 - 1.0 + 2.0 * G2;
            double y2 = y0 - 1.0 + 2.0 * G2;

            int ii = i & 255;
            int jj = j & 255;
            int gi0 = perm[ii + perm[jj]] % 12;
            int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
            int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;

            double t0 = 0.5 - x0 * x0 - y0 * y0;
            if (t0 < 0) n0 = 0.0;
            else {
                t0 *= t0;
                n0 = t0 * t0 * dot(grad3[gi0], x0, y0);
            }

            double t1 = 0.5 - x1 * x1 - y1 * y1;
            if (t1 < 0) n1 = 0.0;
            else {
                t1 *= t1;
                n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
            }

            double t2 = 0.5 - x2 * x2 - y2 * y2;
            if (t2 < 0) n2 = 0.0;
            else {
                t2 *= t2;
                n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
            }

            return 70.0 * (n0 + n1 + n2);
        };

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            double total = 0;
            double freq = frequency;
            double amp = amplitude;

            for (int o = 0; o < octaves; ++o)
            {
                double sampleX = x * freq / m_width;
                double sampleY = y * freq / m_height;
                total += noise(sampleX, sampleY) * amp;

                freq *= lacunarity;
                amp *= persistence;
            }

            total = (total + 1) / 2;  // Normalize to 0-1
            int noiseColor = static_cast<int>(total * 255);

            Color& color = m_colors[i];
            color.r = std::clamp(color.r + noiseColor, 0, 255);
            color.g = std::clamp(color.g + noiseColor, 0, 255);
            color.b = std::clamp(color.b + noiseColor, 0, 255);
        }
    }

    void Canvas::FractalBrownianMotion(double frequency, double amplitude, int octaves, double persistence, double lacunarity)
    {
        int seed = std::random_device{}();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 255);

        std::vector<int> p(512);
        for (int i = 0; i < 256; ++i) p[i] = i;
        std::shuffle(p.begin(), p.begin() + 256, gen);
        for (int i = 0; i < 256; ++i) p[256 + i] = p[i];

        auto fade = [](double t) { return t * t * t * (t * (t * 6 - 15) + 10); };
        auto lerp = [](double t, double a, double b) { return a + t * (b - a); };
        auto grad = [](int hash, double x, double y, double z) {
            int h = hash & 15;
            double u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        };

        auto noise = [&](double x, double y) {
            int X = static_cast<int>(std::floor(x)) & 255;
            int Y = static_cast<int>(std::floor(y)) & 255;
            x -= std::floor(x);
            y -= std::floor(y);
            double u = fade(x), v = fade(y);
            int A = p[X] + Y, AA = p[A], AB = p[A + 1], B = p[X + 1] + Y, BA = p[B], BB = p[B + 1];
            return lerp(v, lerp(u, grad(p[AA], x, y, 0), grad(p[BA], x - 1, y, 0)),
                           lerp(u, grad(p[AB], x, y - 1, 0), grad(p[BB], x - 1, y - 1, 0)));
        };

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            double total = 0.0;
            double freq = frequency;
            double amp = amplitude;

            for (int o = 0; o < octaves; ++o)
            {
                double sampleX = x * freq / m_width;
                double sampleY = y * freq / m_height;

                total += noise(sampleX, sampleY) * amp;

                freq *= lacunarity;
                amp *= persistence;
            }

            // Normalize to [0, 1]
            total = (total + 1.0) / 2.0;

            // Apply to color buffer
            int noiseColor = static_cast<int>(total * 255);
            m_colors[i] &= noiseColor;
        }
    }

    void Canvas::Voronoi(int numPoints, double falloff, double strength)
    {
        int seed = std::random_device{}();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1.0);

        std::vector<std::pair<double, double>> points(numPoints);
        for (auto& point : points)
        {
            point.first = dis(gen);
            point.second = dis(gen);
        }

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            double px = static_cast<double>(x) / m_width;
            double py = static_cast<double>(y) / m_height;

            double minDist = std::numeric_limits<double>::max();
            for (const auto& point : points)
            {
                double dx = px - point.first;
                double dy = py - point.second;
                double dist = std::sqrt(dx*dx + dy*dy);
                minDist = std::min(minDist, dist);
            }

            double value = (std::pow(minDist, falloff) * strength) * 255;
            m_colors[i] &= value;
        }
    }

    void Canvas::Plasma(double frequency, double phase)
    {
        auto plasma = [](double x, double y, double freq, double phase) {
            return std::sin(x * freq + phase) + std::sin(y * freq + phase) +
                   std::sin((x + y) * freq + phase) + std::sin(std::sqrt(x * x + y * y) * freq + phase);
        };

        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;

            double nx = x / static_cast<double>(m_width) - 0.5;
            double ny = y / static_cast<double>(m_height) - 0.5;

            double value = plasma(nx, ny, frequency, phase);
            value = (value + 4) / 8.0;  // Normalize to [0, 1]

            int r = static_cast<int>(std::sin(value * CONST_PI * 2) * 127 + 128);
            int g = static_cast<int>(std::sin(value * CONST_PI * 2 + 2 * CONST_PI / 3) * 127 + 128);
            int b = static_cast<int>(std::sin(value * CONST_PI * 2 + 4 * CONST_PI / 3) * 127 + 128);

            m_colors[i] &= Color(r, g, b);
        }
    }

    void Canvas::DiamondSquare(double roughness, double waterLevel, double levelsPerStop)
    {
        int seed = std::random_device{}();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(-1.0, 1.0);

        int size = std::max(m_width, m_height);
        int power = static_cast<int>(std::ceil(std::log2(size - 1)));
        size = (1 << power) + 1;

        std::vector<std::vector<double>> heightmap(size, std::vector<double>(size, 0.0));

        // Initialize corners
        heightmap[0][0] = heightmap[0][size-1] = heightmap[size-1][0] = heightmap[size-1][size-1] = std::max(waterLevel, dis(gen));

        for (int step = size - 1; step > 1; step /= 2)
        {
            int halfStep = step / 2;
            double scaleFactor = 1.0 - (static_cast<double>(size - step) / size);

            // Diamond step
            #pragma omp parallel for
            for (int y = halfStep; y < size; y += step)
            {
                for (int x = halfStep; x < size; x += step)
                {
                    double avg = (heightmap[y-halfStep][x-halfStep] + heightmap[y-halfStep][x+halfStep] +
                                  heightmap[y+halfStep][x-halfStep] + heightmap[y+halfStep][x+halfStep]) / 4.0;
                    double randomOffset = dis(gen) * roughness * step / size * scaleFactor;
                    heightmap[y][x] = std::max(waterLevel, avg + randomOffset);
                }
            }

            // Square step
            #pragma omp parallel for
            for (int y = 0; y < size; y += halfStep)
            {
                for (int x = (y + halfStep) % step; x < size; x += step)
                {
                    double avg = 0.0;
                    int count = 0;

                    if (y >= halfStep) { avg += heightmap[y-halfStep][x]; count++; }
                    if (y + halfStep < size) { avg += heightmap[y+halfStep][x]; count++; }
                    if (x >= halfStep) { avg += heightmap[y][x-halfStep]; count++; }
                    if (x + halfStep < size) { avg += heightmap[y][x+halfStep]; count++; }

                    avg /= count;
                    double randomOffset = dis(gen) * roughness * step / size * scaleFactor;
                    heightmap[y][x] = std::max(waterLevel, avg + randomOffset);
                }
            }

            roughness *= 0.5;
        }

        // After generating the heightmap, let's adjust the distribution
        double maxHeight = 0.0;
        double minHeight = 1.0;
        for (const auto& row : heightmap) {
            maxHeight = std::max(maxHeight, *std::max_element(row.begin(), row.end()));
            minHeight = std::min(minHeight, *std::min_element(row.begin(), row.end()));
        }

        // Adjust heights to reduce white areas
        for (auto& row : heightmap) {
            for (auto& height : row) {
                // Normalize the height
                double normalizedHeight = (height - minHeight) / (maxHeight - minHeight);
                // Apply a power function to reduce higher elevations
                height = std::pow(normalizedHeight, 1.5) * (maxHeight - minHeight) + minHeight;
            }
        }

        // Predefined color stops
        const std::vector<std::pair<double, Color>> colorStops = {
            {waterLevel, Color(0, 0, 200)},        // Deep water
            {waterLevel + 0.05, Color(0, 100, 255)}, // Shallow water
            {waterLevel + 0.1, Color(240, 220, 130)}, // Light sand
            {waterLevel + 0.15, Color(210, 180, 140)}, // Dark sand
            {0.3, Color(34, 139, 34)},             // Dark green
            {0.4, Color(50, 205, 50)},             // Lime green
            {0.5, Color(107, 142, 35)},            // Olive drab
            {0.6, Color(160, 82, 45)},             // Sienna
            {0.7, Color(139, 69, 19)},             // Saddle brown
            {0.8, Color(128, 128, 128)},           // Gray
            {0.9, Color(192, 192, 192)},           // Light gray
            {1.0, Color(255, 255, 255)}            // White
        };

        // Generate color bands with interpolated levels
        std::vector<std::pair<double, Color>> colorBands;
        for (size_t i = 1; i < colorStops.size(); ++i)
        {
            double startHeight = colorStops[i-1].first;
            double endHeight = colorStops[i].first;
            Color startColor = colorStops[i-1].second;
            Color endColor = colorStops[i].second;

            for (int level = 0; level < levelsPerStop; ++level)
            {
                double t = static_cast<double>(level) / levelsPerStop;
                double height = startHeight + (endHeight - startHeight) * t;
                Color color = Color::Mix(startColor, endColor, t);
                colorBands.emplace_back(height, color);
            }
        }
        colorBands.push_back(colorStops.back());

        // Color mapping function
        auto mapToColor = [&colorBands](double height) -> Color {
            for (size_t i = 1; i < colorBands.size(); ++i) {
                if (height <= colorBands[i].first) {
                    return colorBands[i-1].second;
                }
            }
            return colorBands.back().second;
        };

        // Map heightmap to Canvas
        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;
            double height = heightmap[y * size / m_height][x * size / m_width];
            m_colors[i] = mapToColor(height);
        }
    }

    void Canvas::Posterize(int levels)
    {
        levels = std::clamp(levels, 2, 256);
        double factor = 255.0 / (levels - 1);

        #pragma omp parallel for
        for (auto& color : m_colors)
        {
            color.r = static_cast<int>(std::min(255.0, std::round(std::round(color.r / factor) * factor)));
            color.g = static_cast<int>(std::min(255.0, std::round(std::round(color.g / factor) * factor)));
            color.b = static_cast<int>(std::min(255.0, std::round(std::round(color.b / factor) * factor)));
        }
    }
    #pragma endregion

    #pragma region Utility
    Color Canvas::CalculateAverageColor(int startX, int startY, int pixelWidth, int pixelHeight)
    {
        if (pixelWidth == 0) pixelWidth = m_width;
        if (pixelHeight == 0) pixelHeight = m_height;

        int totalR = 0, totalG = 0, totalB = 0, totalA = 0;
        int count = 0;
        int endX = std::min(startX + pixelWidth, m_width);
        int endY = std::min(startY + pixelHeight, m_height);

        #pragma omp parallel for
        for (int i = 0; i < pixelWidth * pixelHeight; ++i)
        {
            int x = startX + (i % pixelWidth);
            int y = startY + (i / pixelWidth);

            if (x < endX && y < endY)
            {
                Color pixel = Get(x, y);
                totalR += pixel.GetRed();
                totalG += pixel.GetGreen();
                totalB += pixel.GetBlue();
                totalA += pixel.GetAlpha();
                ++count;
            }
        }

        return Color(
            totalR / count,
            totalG / count,
            totalB / count,
            totalA / count
        );
    }

    void Canvas::MapColors(int x, int y, int width, int height, unsigned int (*mapFunction)(int, int, unsigned int))
    {
        int xmin = std::max(0, x);
        int ymin = std::max(0, y);
        int xmax = std::min(m_width - 1, x + width - 1);
        int ymax = std::min(m_height - 1, y + height - 1);

        int xRange = xmax - xmin + 1;
        int yRange = ymax - ymin + 1;

        for (int i = 0; i < xRange * yRange; ++i)
        {
            int x = i % xRange + xmin;
            int y = i / xRange + ymin;
            int index = y * m_width + x;
            unsigned int color = m_colors[index].ToInt(0);
            m_colors[index] = Color(mapFunction(x, y, color));
        }
    }

    void Canvas::ForEach(const std::function<void(const Color&)>& func) const
    {
        for (const auto& color : m_colors)
        {
            func(color);
        }
    }

    Canvas* Canvas::CopyRegion(int xmin, int ymin, int width, int height) const
    {
        Canvas* subBuffer = new Canvas(width, height);

        #pragma omp parallel for
        for (int i = 0; i < width * height; ++i)
        {
            int x = i % width;
            int y = i / width;
            subBuffer->m_colors[i] = m_colors[(y + ymin) * m_width + (x + xmin)];
        }

        return subBuffer;
    }

    Canvas* Canvas::Copy() const
    {
        Canvas* newBuffer = new Canvas(m_width, m_height);
        newBuffer->m_colors = m_colors;
        return newBuffer;
    }

    void Canvas::Rotate(double angle)
    {
        double radians = -(angle * CONST_PI / 180.0);
        double cos_angle = cos(radians);
        double sin_angle = sin(radians);

        int newWidth = static_cast<int>(std::abs(m_width * cos_angle) + std::abs(m_height * sin_angle));
        int newHeight = static_cast<int>(std::abs(m_width * sin_angle) + std::abs(m_height * cos_angle));

        std::vector<Color> newColors(newWidth * newHeight, Color(0, 0, 0, 0));

        int centerX = m_width / 2;
        int centerY = m_height / 2;
        int newCenterX = newWidth / 2;
        int newCenterY = newHeight / 2;

        #pragma omp parallel for
        for (int i = 0; i < newWidth * newHeight; ++i)
        {
            int x = i % newWidth;
            int y = i / newWidth;

            int translatedX = x - newCenterX;
            int translatedY = y - newCenterY;

            int originalX = static_cast<int>(translatedX * cos_angle - translatedY * sin_angle + centerX);
            int originalY = static_cast<int>(translatedX * sin_angle + translatedY * cos_angle + centerY);

            if (originalX >= 0 && originalX < m_width && originalY >= 0 && originalY < m_height)
                newColors[i] = m_colors[originalY * m_width + originalX];
        }

        m_colors = std::move(newColors);
        m_width = newWidth;
        m_height = newHeight;
    }

    void Canvas::Resize(int newWidth, int newHeight, int resizeImage = 1, Color fillColor = Color::Black())
    {
        resizeImage = (resizeImage != 0) ? true : false;

        // Calculate dimensions preserving aspect ratio if only one dimension provided
        if (newWidth > 0 && newHeight <= 0)
        {
            double ratio = static_cast<double>(m_height) / m_width;
            newHeight = static_cast<int>(newWidth * ratio);
        }
        else if (newHeight > 0 && newWidth <= 0)
        {
            double ratio = static_cast<double>(m_width) / m_height;
            newWidth = static_cast<int>(newHeight * ratio);
        }
        else if (newWidth <= 0 || newHeight <= 0)
        {
            return;
        }

        std::vector<Color> newColors(newWidth * newHeight, fillColor);

        if (resizeImage)
        {
            float xRatio = static_cast<float>(m_width - 1) / (newWidth - 1);
            float yRatio = static_cast<float>(m_height - 1) / (newHeight - 1);

            #pragma omp parallel for
            for (int i = 0; i < newWidth * newHeight; ++i)
            {
                int x = i % newWidth;
                int y = i / newWidth;

                float gx = x * xRatio;
                float gy = y * yRatio;
                int gxi = static_cast<int>(gx);
                int gyi = static_cast<int>(gy);
                float dx = gx - gxi;
                float dy = gy - gyi;

                Color c00 = Get(gxi, gyi);
                Color c10 = Get(std::min(gxi + 1, m_width - 1), gyi);
                Color c01 = Get(gxi, std::min(gyi + 1, m_height - 1));
                Color c11 = Get(std::min(gxi + 1, m_width - 1), std::min(gyi + 1, m_height - 1));

                Color interpolated =
                    c00 * ((1 - dx) * (1 - dy)) +
                    c10 * (dx * (1 - dy)) +
                    c01 * ((1 - dx) * dy) +
                    c11 * (dx * dy);

                newColors[i] = interpolated;
            }
        }
        else
        {
            int copyWidth = std::min(m_width, newWidth);
            int copyHeight = std::min(m_height, newHeight);

            #pragma omp parallel for
            for (int y = 0; y < copyHeight; ++y)
            {
                std::copy(m_colors.begin() + y * m_width,
                          m_colors.begin() + y * m_width + copyWidth,
                          newColors.begin() + y * newWidth);
            }
        }

        m_colors = std::move(newColors);
        m_width = newWidth;
        m_height = newHeight;
    }

    void Canvas::Scale(double scale)
    {
        int newWidth  = m_width * scale;
        int newHeight = m_height * scale;
        this->Resize(newWidth, newHeight);
    }

    size_t Canvas::CountUniqueColors() const
    {
        std::unordered_set<Color> uniqueColors;
        uniqueColors.reserve(m_colors.size());

        for (const auto& color : m_colors) uniqueColors.insert(color);

        return uniqueColors.size();
    }

    int Canvas::Find(const Color& color) const
    {
        auto it = std::find(m_colors.begin(), m_colors.end(), color);
        return (it != m_colors.end()) ? std::distance(m_colors.begin(), it) : -1;
    }

    int Canvas::FindLast(const Color& color) const
    {
        auto it = std::find(m_colors.rbegin(), m_colors.rend(), color);
        return (it != m_colors.rend()) ? m_colors.size() - 1 - std::distance(m_colors.rbegin(), it) : -1;
    }

    std::vector<int> Canvas::FindAll(const Color& color) const
    {
        std::vector<int> indices;
        for (size_t i = 0; i < m_colors.size(); ++i)
        {
            if (m_colors[i] == color)
            {
                indices.push_back(static_cast<int>(i));
            }
        }
        return indices;
    }

    void Canvas::Swap(size_t index1, size_t index2)
    {
        if (index1 < m_colors.size() && index2 < m_colors.size())
        {
            std::swap(m_colors[index1], m_colors[index2]);
        }
    }

    Canvas Canvas::Filter(const std::function<bool(const Color&)>& predicate) const
    {
        Canvas result(m_width, m_height);
        std::copy_if(m_colors.begin(), m_colors.end(), result.m_colors.begin(), predicate);
        return result;
    }

    int Canvas::Count(const Color& color) const
    {
        return std::count(m_colors.begin(), m_colors.end(), color);
    }

    void Canvas::Shuffle()
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(m_colors.begin(), m_colors.end(), g);
    }

    void Canvas::Clear()
    {
        m_colors.clear();
        m_width = 0;
        m_height = 0;
    }

    void Canvas::Sort(const std::function<bool(const Color&, const Color&)>& compare)
    {
        std::sort(m_colors.begin(), m_colors.end(), compare);
    }

    void Canvas::AppendRight(const Canvas& other)
    {
        int newWidth = m_width + other.m_width;
        int newHeight = std::max(m_height, other.m_height);
        std::vector<Color> newColors(newWidth * newHeight, Color(0, 0, 0, 0)); // Initialize with transparent pixels

        for (int y = 0; y < newHeight; ++y)
        {
            if (y < m_height)
            {
                std::copy(m_colors.begin() + y * m_width, m_colors.begin() + (y + 1) * m_width, newColors.begin() + y * newWidth);
            }
            if (y < other.m_height)
            {
                std::copy(other.m_colors.begin() + y * other.m_width, other.m_colors.begin() + (y + 1) * other.m_width, newColors.begin() + y * newWidth + m_width);
            }
        }

        m_colors = std::move(newColors);
        m_width = newWidth;
        m_height = newHeight;
    }

    void Canvas::AppendBottom(const Canvas& other)
    {
        int newWidth = std::max(m_width, other.m_width);
        int newHeight = m_height + other.m_height;
        std::vector<Color> newColors(newWidth * newHeight, Color(0, 0, 0, 0)); // Initialize with transparent pixels

        for (int y = 0; y < m_height; ++y)
        {
            std::copy(m_colors.begin() + y * m_width, m_colors.begin() + (y + 1) * m_width, newColors.begin() + y * newWidth);
        }

        for (int y = 0; y < other.m_height; ++y)
        {
            std::copy(other.m_colors.begin() + y * other.m_width, other.m_colors.begin() + (y + 1) * other.m_width, newColors.begin() + (m_height + y) * newWidth);
        }

        m_colors = std::move(newColors);
        m_width = newWidth;
        m_height = newHeight;
    }

    HBITMAP Canvas::ToHBITMAP(int targetWidth, int targetHeight) const
    {
        targetWidth = (targetWidth <= 0) ? m_width : targetWidth;
        targetHeight = (targetHeight <= 0) ? m_height : targetHeight;

        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = targetWidth;
        bmi.bmiHeader.biHeight = -targetHeight;  // Top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

        std::vector<BYTE> buffer(targetWidth * targetHeight * 4);

        for (int i = 0; i < targetWidth * targetHeight; ++i)
        {
            int x = i % targetWidth;
            int y = i / targetWidth;
            int srcX = x * m_width / targetWidth;
            int srcY = y * m_height / targetHeight;
            const Color& color = Get(srcX, srcY);
            int index = i * 4;
            buffer[index] = color.GetBlue();
            buffer[index + 1] = color.GetGreen();
            buffer[index + 2] = color.GetRed();
            buffer[index + 3] = color.GetAlpha();
        }

        SetDIBits(hdc, hBitmap, 0, targetHeight, buffer.data(), &bmi, DIB_RGB_COLORS);

        SelectObject(memDC, oldBitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);

        return hBitmap;
    }

    Canvas* Canvas::FromHBITMAP(HBITMAP hBitmap, int width, int height)
    {
        BITMAP bm;
        GetObject(hBitmap, sizeof(BITMAP), &bm);

        int sourceWidth = bm.bmWidth;
        int sourceHeight = bm.bmHeight;

        width = (width <= 0) ? sourceWidth : width;
        height = (height <= 0) ? sourceHeight : height;

        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = sourceWidth;
        bmi.bmiHeader.biHeight = -sourceHeight;  // Top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        std::vector<BYTE> buffer(sourceWidth * sourceHeight * 4);
        GetDIBits(memDC, hBitmap, 0, sourceHeight, buffer.data(), &bmi, DIB_RGB_COLORS);

        Canvas* canvas = new Canvas(width, height);

        for (int i = 0; i < width * height; ++i)
        {
            int x = i % width;
            int y = i / width;
            int srcX = x * sourceWidth / width;
            int srcY = y * sourceHeight / height;
            int index = (srcY * sourceWidth + srcX) * 4;
            canvas->m_colors[i] = Color(buffer[index + 2], buffer[index + 1], buffer[index], buffer[index + 3]);
        }

        SelectObject(memDC, oldBitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);

        return canvas;
    }

    HDC Canvas::ToHDC(int width, int height) const
    {
        width = (width <= 0) ? m_width : width;
        height = (height <= 0) ? m_height : height;

        HDC hdc = CreateCompatibleDC(NULL);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;  // Top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hdc, hBitmap);

        BYTE* pixels = (BYTE*)pBits;
        for (int i = 0; i < width * height; ++i)
        {
            int x = i % width;
            int y = i / width;
            int srcX = x * m_width / width;
            int srcY = y * m_height / height;
            const Color& color = Get(srcX, srcY);

            int index = i * 4;
            pixels[index] = color.GetBlue();
            pixels[index + 1] = color.GetGreen();
            pixels[index + 2] = color.GetRed();
            pixels[index + 3] = color.GetAlpha();
        }

        return hdc;
    }

    Canvas* Canvas::FromHDC(HDC hdc, int x, int y, int width, int height)
    {
        if (width == 0) width = GetDeviceCaps(hdc, HORZRES) - x;
        if (height == 0) height = GetDeviceCaps(hdc, VERTRES) - y;

        HDC memDC = CreateCompatibleDC(NULL);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

        BitBlt(memDC, 0, 0, width, height, hdc, x, y, SRCCOPY);

        BITMAPINFOHEADER bi = { };
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;

        BYTE* bits = new BYTE[width * height * 4];
        GetDIBits(memDC, hBitmap, 0, height, bits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        Canvas* buffer = new Canvas(width, height);
        #pragma omp parallel for
        for (int i = 0; i < width * height; ++i) {
            int index = i * 4;
            buffer->SetAt(i, Color(
                bits[index + 2],
                bits[index + 1],
                bits[index],
                255
            ));
        }

        delete[] bits;
        SelectObject(memDC, oldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(memDC);

        return buffer;
    }

    HICON Canvas::ToHICON(int width, int height) const
    {
        width = (width <= 0) ? m_width : width;
        height = (height <= 0) ? m_height : height;

        // Create color and mask bitmaps
        BITMAPV5HEADER bi = {};
        bi.bV5Size = sizeof(BITMAPV5HEADER);
        bi.bV5Width = width;
        bi.bV5Height = -height;  // Top-down
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask = 0x00FF0000;
        bi.bV5GreenMask = 0x0000FF00;
        bi.bV5BlueMask = 0x000000FF;
        bi.bV5AlphaMask = 0xFF000000;

        void* pBits;
        HDC hdc = GetDC(NULL);
        HBITMAP hColor = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &pBits, NULL, 0);
        BYTE* pixels = (BYTE*)pBits;

        // Create mask bitmap (monochrome)
        HBITMAP hMask = CreateBitmap(width, height, 1, 1, NULL);

        // Fill color bitmap with our image data
        for (int i = 0; i < width * height; ++i)
        {
            int x = i % width;
            int y = i / width;
            int srcX = x * m_width / width;
            int srcY = y * m_height / height;
            const Color& color = Get(srcX, srcY);

            int index = i * 4;
            pixels[index] = color.GetBlue();
            pixels[index + 1] = color.GetGreen();
            pixels[index + 2] = color.GetRed();
            pixels[index + 3] = color.GetAlpha();
        }

        ICONINFO ii = {};
        ii.fIcon = TRUE;
        ii.hbmMask = hMask;
        ii.hbmColor = hColor;

        HICON hIcon = CreateIconIndirect(&ii);

        DeleteObject(hColor);
        DeleteObject(hMask);
        ReleaseDC(NULL, hdc);

        return hIcon;
    }

    Canvas* Canvas::FromHICON(HICON hIcon)
    {
        ICONINFO ii;
        if (!GetIconInfo(hIcon, &ii))
            return nullptr;

        BITMAP bm;
        GetObject(ii.hbmColor, sizeof(BITMAP), &bm);
        int width = bm.bmWidth;
        int height = bm.bmHeight;

        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(memDC, hBitmap);

        DrawIconEx(memDC, 0, 0, hIcon, width, height, 0, NULL, DI_NORMAL);

        BYTE* pixels = (BYTE*)pBits;
        Canvas* buffer = new Canvas(width, height);

        for (int i = 0; i < width * height; ++i)
        {
            int index = i * 4;
            buffer->SetAt(i, Color(
                pixels[index + 2],  // Red
                pixels[index + 1],  // Green
                pixels[index],      // Blue
                pixels[index + 3]   // Alpha
            ));
        }

        DeleteObject(hBitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);
        DeleteObject(ii.hbmColor);
        DeleteObject(ii.hbmMask);

        return buffer;
    }

    HCURSOR Canvas::ToHCURSOR(int width, int height) const
    {
        width = (width <= 0) ? m_width : width;
        height = (height <= 0) ? m_height : height;

        BITMAPV5HEADER bi = {};
        bi.bV5Size = sizeof(BITMAPV5HEADER);
        bi.bV5Width = width;
        bi.bV5Height = -height;  // Top-down
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask = 0x00FF0000;
        bi.bV5GreenMask = 0x0000FF00;
        bi.bV5BlueMask = 0x000000FF;
        bi.bV5AlphaMask = 0xFF000000;

        void* pBits;
        HDC hdc = GetDC(NULL);
        HBITMAP hColor = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &pBits, NULL, 0);
        BYTE* pixels = (BYTE*)pBits;

        HBITMAP hMask = CreateBitmap(width, height, 1, 1, NULL);

        for (int i = 0; i < width * height; ++i)
        {
            int x = i % width;
            int y = i / width;
            int srcX = x * m_width / width;
            int srcY = y * m_height / height;
            const Color& color = Get(srcX, srcY);

            int index = i * 4;
            pixels[index] = color.GetBlue();
            pixels[index + 1] = color.GetGreen();
            pixels[index + 2] = color.GetRed();
            pixels[index + 3] = color.GetAlpha();
        }

        ICONINFO ii = {};
        ii.fIcon = FALSE;
        ii.xHotspot = width / 2;
        ii.yHotspot = height / 2;
        ii.hbmMask = hMask;
        ii.hbmColor = hColor;

        HCURSOR hCursor = CreateIconIndirect(&ii);

        DeleteObject(hColor);
        DeleteObject(hMask);
        ReleaseDC(NULL, hdc);

        return hCursor;
    }

    Canvas* Canvas::FromHCURSOR(HCURSOR hCursor)
    {
        ICONINFO ii;
        if (!GetIconInfo(hCursor, &ii))
            return nullptr;

        BITMAP bm;
        GetObject(ii.hbmColor, sizeof(BITMAP), &bm);
        int width = bm.bmWidth;
        int height = bm.bmHeight;

        HDC hdc = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(hdc);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(memDC, hBitmap);

        DrawIconEx(memDC, 0, 0, hCursor, width, height, 0, NULL, DI_NORMAL);

        BYTE* pixels = (BYTE*)pBits;
        Canvas* buffer = new Canvas(width, height);

        for (int i = 0; i < width * height; ++i)
        {
            int index = i * 4;
            buffer->SetAt(i, Color(
                pixels[index + 2],  // Red
                pixels[index + 1],  // Green
                pixels[index],      // Blue
                pixels[index + 3]   // Alpha
            ));
        }

        DeleteObject(hBitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);
        DeleteObject(ii.hbmColor);
        DeleteObject(ii.hbmMask);

        return buffer;
    }

    Canvas* Canvas::FromHWND(HWND hwnd, int x, int y, int width, int height)
    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        if (width == 0) width = clientRect.right - x;
        if (height == 0) height = clientRect.bottom - y;

        HDC windowDC = GetDCEx(hwnd, NULL, DCX_WINDOW | DCX_CACHE);
        HDC memDC = CreateCompatibleDC(windowDC);
        HBITMAP hBitmap = CreateCompatibleBitmap(windowDC, width + x, height + y);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

        PrintWindow(hwnd, memDC, PW_RENDERFULLCONTENT | PW_CLIENTONLY);

        // Create a second DC for the offset region
        HDC offsetDC = CreateCompatibleDC(windowDC);
        HBITMAP offsetBitmap = CreateCompatibleBitmap(windowDC, width, height);
        HBITMAP oldOffsetBitmap = (HBITMAP)SelectObject(offsetDC, offsetBitmap);

        // Copy the region at offset x,y to our final bitmap
        BitBlt(offsetDC, 0, 0, width, height, memDC, x, y, SRCCOPY);

        BITMAPINFOHEADER bi = { };
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;

        BYTE* bits = new BYTE[width * height * 4];
        GetDIBits(offsetDC, offsetBitmap, 0, height, bits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        Canvas* buffer = new Canvas(width, height);
        #pragma omp parallel for
        for (int i = 0; i < width * height; ++i)
        {
            int index = i * 4;
            buffer->SetAt(i, Color(
                bits[index + 2],
                bits[index + 1],
                bits[index],
                255
            ));
        }

        delete[] bits;
        SelectObject(offsetDC, oldOffsetBitmap);
        SelectObject(memDC, oldBitmap);
        DeleteObject(offsetBitmap);
        DeleteObject(hBitmap);
        DeleteDC(offsetDC);
        DeleteDC(memDC);
        ReleaseDC(hwnd, windowDC);

        return buffer;
    }

    void Canvas::Draw(HWND hwnd, int x, int y) const
    {
        int width = this->GetWidth();
        int height = this->GetHeight();
        HBITMAP hBitmap = this->ToHBITMAP(width, height); // Explicitly pass dimensions
        HDC hdcWindow = GetDC(hwnd);
        HDC hdcMemory = CreateCompatibleDC(hdcWindow);

        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmap);

        BitBlt(hdcWindow, x, y, width, height, hdcMemory, 0, 0, SRCCOPY);

        SelectObject(hdcMemory, hOldBitmap);
        DeleteDC(hdcMemory);
        ReleaseDC(hwnd, hdcWindow);
        DeleteObject(hBitmap);
    }
    #pragma endregion
}