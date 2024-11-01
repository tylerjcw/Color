#define NOMINMAX

#include "Color.h"
#include <stdexcept>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <array>

namespace KTLib
{
    #pragma region Constructors
    ColorBuffer::ColorBuffer(int width, int height, Color col = BLACK) : m_width(width), m_height(height)
    {
        m_colors.resize(width * height, col);
    }

    ColorBuffer::ColorBuffer(unsigned int* buffer, int width, int height) : m_width(width), m_height(height)
    {
        m_colors.resize(width * height);
        for (int i = 0; i < width * height; ++i)
        {
            m_colors[i] = Color(buffer[i]);
        }
    }

    ColorBuffer::ColorBuffer(Color** colors, int width, int height) : m_width(width), m_height(height)
    {
        m_colors.reserve(width * height);
        for (int i = 0; i < width * height; ++i)
        {
            m_colors.push_back(*colors[i]);
        }
    }

    ColorBuffer::ColorBuffer(const std::vector<std::vector<Color>>& colors) : m_width(colors.empty() ? 0 : colors[0].size()), m_height(colors.size())
    {
        m_colors.reserve(m_width * m_height);
        for (const auto& row : colors)
        {
            m_colors.insert(m_colors.end(), row.begin(), row.end());
        }
    }

    ColorBuffer::ColorBuffer(const Gradient& gradient, int width, int height) : m_width(width), m_height(height)
    {
        m_colors.resize(width * height);

        float centerX = width / 2.0f;
        float centerY = height / 2.0f;
        float maxRadius = std::sqrt(centerX * centerX + centerY * centerY);

        #pragma omp parallel for
        for (int i = 0; i < width * height; ++i)
        {
            int x = i % width;
            int y = i / width;
            float position;

            switch (gradient.GetType())
            {
                case GradientType::Linear:
                    position = static_cast<float>(x) / (width - 1);
                    break;
                case GradientType::Radial:
                {
                    float dx = x - centerX;
                    float dy = y - centerY;
                    maxRadius = std::max(centerX, centerY);
                    position = std::sqrt(dx * dx + dy * dy) / maxRadius;
                    break;
                }
                case GradientType::Conical:
                {
                    float angle = std::atan2(y - centerY, x - centerX);
                    position = (angle + CONST_PI) / (2 * CONST_PI);
                    break;
                }
            }

            position = std::clamp(position, 0.0f, 1.0f);
            position = std::round(position * (gradient.GetTotalSteps() - 1)) / (gradient.GetTotalSteps() - 1);
            m_colors[i] = gradient.GetColorAt(GradientPosition::FromNormalized(position));
        }
    }
    #pragma endregion

    #pragma region ColorBuffer Functions
    Color& ColorBuffer::Get(int x, int y)                   { return m_colors[y * m_width + x]; }
    const Color& ColorBuffer::Get(int x, int y) const       { return m_colors[y * m_width + x]; }
    void ColorBuffer::Set(int x, int y, const Color& color) { m_colors[y * m_width + x] = color; }

    Color ColorBuffer::GetAt(int index) const              { return m_colors[index]; }
    void ColorBuffer::SetAt(int index, const Color& color) { m_colors[index] = color; }

    void ColorBuffer::GetXY(int index, int& x, int& y) const   { y = index / m_width, x = index % m_width; }
    void ColorBuffer::GetIndex(int x, int y, int& index) const { index = y * m_width + x; }

    void ColorBuffer::ShiftRed(int amount)   { for (auto& color : m_colors) color.SetRed(std::clamp(color.GetRed() + amount, 0, 255)); }
    void ColorBuffer::ShiftGreen(int amount) { for (auto& color : m_colors) color.SetGreen(std::clamp(color.GetGreen() + amount, 0, 255)); }
    void ColorBuffer::ShiftBlue(int amount)  { for (auto& color : m_colors) color.SetBlue(std::clamp(color.GetBlue() + amount, 0, 255)); }
    void ColorBuffer::ShiftAlpha(int amount) { for (auto& color : m_colors) color.SetAlpha(std::clamp(color.GetAlpha() + amount, 0, 255)); }

    void ColorBuffer::SetRed(int value)   { for (auto& color : m_colors) color.SetRed(std::clamp(value, 0, 255)); }
    void ColorBuffer::SetGreen(int value) { for (auto& color : m_colors) color.SetGreen(std::clamp(value, 0, 255)); }
    void ColorBuffer::SetBlue(int value)  { for (auto& color : m_colors) color.SetBlue(std::clamp(value, 0, 255)); }
    void ColorBuffer::SetAlpha(int value) { for (auto& color : m_colors) color.SetAlpha(std::clamp(value, 0, 255)); }
    #pragma endregion

    #pragma region Color Modification Functions
    void ColorBuffer::Invert()                         { for (auto& color : m_colors) color.Invert(); }
    void ColorBuffer::ShiftHue(double degrees)         { for (auto& color : m_colors) color.ShiftHue(degrees); }
    void ColorBuffer::Grayscale()                      { for (auto& color : m_colors) color.Grayscale(); }
    void ColorBuffer::Sepia(double factor)             { for (auto& color : m_colors) color.Sepia(factor); }
    void ColorBuffer::CrossProcess(double factor)      { for (auto& color : m_colors) color.CrossProcess(factor); }
    void ColorBuffer::Moonlight(double factor)         { for (auto& color : m_colors) color.Moonlight(factor); }
    void ColorBuffer::VintageFilm(double factor)       { for (auto& color : m_colors) color.VintageFilm(factor); }
    void ColorBuffer::Technicolor(double factor)       { for (auto& color : m_colors) color.Technicolor(factor); }
    void ColorBuffer::Polaroid(double factor)          { for (auto& color : m_colors) color.Polaroid(factor); }
    void ColorBuffer::Complement()                     { for (auto& color : m_colors) color.Complement(); }
    void ColorBuffer::ShiftSaturation(double amount)   { for (auto& color : m_colors) color.ShiftSaturation(amount); }
    void ColorBuffer::ShiftLightness(double amount)    { for (auto& color : m_colors) color.ShiftLightness(amount); }
    void ColorBuffer::ShiftValue(double amount)        { for (auto& color : m_colors) color.ShiftValue(amount); }
    void ColorBuffer::ShiftIntensity(double amount)    { for (auto& color : m_colors) color.ShiftIntensity(amount); }
    void ColorBuffer::ShiftWhiteLevel(double amount)   { for (auto& color : m_colors) color.ShiftWhiteLevel(amount); }
    void ColorBuffer::ShiftBlackLevel(double amount)   { for (auto& color : m_colors) color.ShiftBlackLevel(amount); }
    void ColorBuffer::ShiftContrast(double amount)     { for (auto& color : m_colors) color.ShiftContrast(amount); }

    void ColorBuffer::Pixelate(int pixelSize)
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

    void ColorBuffer::Blur(int radius)
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

    void ColorBuffer::GaussianBlur(double sigma)
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

    void ColorBuffer::Sharpen(float amount)
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

    void ColorBuffer::Flip(bool horizontal)
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

    void ColorBuffer::Crop(int x, int y, int width, int height)
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

    void ColorBuffer::AdjustContrast(double factor)
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

    void ColorBuffer::AdjustColorBalance(double redFactor, double greenFactor, double blueFactor)
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

    void ColorBuffer::OverlayImage(const ColorBuffer& overlay, int x, int y, double opacity)
    {
        int overlayWidth = overlay.GetWidth();
        int overlayHeight = overlay.GetHeight();

        #pragma omp parallel for
        for (int i = 0; i < overlayWidth * overlayHeight; ++i)
        {
            int dx = i % overlayWidth;
            int dy = i / overlayWidth;
            int destX = x + dx;
            int destY = y + dy;

            if (destX >= 0 && destX < m_width && destY >= 0 && destY < m_height)
            {
                Color& baseColor = m_colors[destY * m_width + destX];
                const Color& overlayColor = overlay.GetAt(i);
                baseColor.Mix(overlayColor, opacity);
            }
        }
    }

    void ColorBuffer::Emboss()
    {
        ColorBuffer temp = *this;

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

    void ColorBuffer::EdgeDetect()
    {
        ColorBuffer temp = *this;
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

    void ColorBuffer::Vignette(double strength, double radius)
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

    void ColorBuffer::TwoColorNoise(double density, const Color& colorOne, const Color& colorTwo)
    {
        std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_real_distribution<> dis(0.0, 1.0);

        int totalPixels = m_width * m_height;

        #pragma omp parallel for
        for (int i = 0; i < totalPixels; ++i)
            if (dis(gen) < density)
                m_colors[i] = (dis(gen) < 0.5) ? colorTwo : colorOne;
    }

    void ColorBuffer::GaussianNoise(double mean, double stdDev)
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

    void ColorBuffer::PerlinNoise(double frequency, double amplitude, int octaves, double persistence, double lacunarity)
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

    void ColorBuffer::SimplexNoise(double frequency, double amplitude, int octaves, double persistence, double lacunarity)
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

    void ColorBuffer::FractalBrownianMotion(double frequency, double amplitude, int octaves, double persistence, double lacunarity)
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

    void ColorBuffer::Voronoi(int numPoints, double falloff, double strength)
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

    void ColorBuffer::Plasma(double frequency, double phase)
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

    void ColorBuffer::DiamondSquare(double roughness, double waterLevel, double levelsPerStop)
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

        // Map heightmap to ColorBuffer
        #pragma omp parallel for
        for (int i = 0; i < m_width * m_height; ++i)
        {
            int x = i % m_width;
            int y = i / m_width;
            double height = heightmap[y * size / m_height][x * size / m_width];
            m_colors[i] = mapToColor(height);
        }
    }

    void ColorBuffer::Posterize(int levels)
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
    Color ColorBuffer::CalculateAverageColor(int startX, int startY, int pixelWidth, int pixelHeight)
    {
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

    void ColorBuffer::MapColors(int x, int y, int width, int height, unsigned int (*mapFunction)(int, int, unsigned int))
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

    void ColorBuffer::ForEach(const std::function<void(const Color&)>& func) const
    {
        for (const auto& color : m_colors)
        {
            func(color);
        }
    }

    ColorBuffer* ColorBuffer::CopyRegion(int xmin, int ymin, int width, int height) const
    {
        ColorBuffer* subBuffer = new ColorBuffer(width, height);

        #pragma omp parallel for
        for (int i = 0; i < width * height; ++i)
        {
            int x = i % width;
            int y = i / width;
            subBuffer->m_colors[i] = m_colors[(y + ymin) * m_width + (x + xmin)];
        }

        return subBuffer;
    }

    ColorBuffer* ColorBuffer::Copy() const
    {
        ColorBuffer* newBuffer = new ColorBuffer(m_width, m_height);
        newBuffer->m_colors = m_colors;
        return newBuffer;
    }

    void ColorBuffer::Rotate(double angle)
    {
        double radians = angle * CONST_PI / 180.0;
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

    void ColorBuffer::Resize(int newWidth, int newHeight, int resizeImage = 1, Color fillColor = BLACK)
    {
        resizeImage = (resizeImage != 0) ? true : false;
        if (newWidth <= 0 || newHeight <= 0)
            return;

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

    void ColorBuffer::Scale(double scale)
    {
        int newWidth  = m_width * scale;
        int newHeight = m_height * scale;
        this->Resize(newWidth, newHeight);
    }

    int ColorBuffer::Find(const Color& color) const
    {
        auto it = std::find(m_colors.begin(), m_colors.end(), color);
        return (it != m_colors.end()) ? std::distance(m_colors.begin(), it) : -1;
    }

    int ColorBuffer::FindLast(const Color& color) const
    {
        auto it = std::find(m_colors.rbegin(), m_colors.rend(), color);
        return (it != m_colors.rend()) ? m_colors.size() - 1 - std::distance(m_colors.rbegin(), it) : -1;
    }

    std::vector<int> ColorBuffer::FindAll(const Color& color) const
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

    void ColorBuffer::Swap(size_t index1, size_t index2)
    {
        if (index1 < m_colors.size() && index2 < m_colors.size())
        {
            std::swap(m_colors[index1], m_colors[index2]);
        }
    }

    ColorBuffer ColorBuffer::Filter(const std::function<bool(const Color&)>& predicate) const
    {
        ColorBuffer result(m_width, m_height);
        std::copy_if(m_colors.begin(), m_colors.end(), result.m_colors.begin(), predicate);
        return result;
    }

    int ColorBuffer::Count(const Color& color) const
    {
        return std::count(m_colors.begin(), m_colors.end(), color);
    }

    void ColorBuffer::Shuffle()
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(m_colors.begin(), m_colors.end(), g);
    }

    void ColorBuffer::Clear()
    {
        m_colors.clear();
        m_width = 0;
        m_height = 0;
    }

    void ColorBuffer::Sort(const std::function<bool(const Color&, const Color&)>& compare)
    {
        std::sort(m_colors.begin(), m_colors.end(), compare);
    }

    void ColorBuffer::AppendRight(const ColorBuffer& other)
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

    void ColorBuffer::AppendBottom(const ColorBuffer& other)
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

    HBITMAP ColorBuffer::ToHBITMAP(int targetWidth, int targetHeight) const
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

    ColorBuffer* ColorBuffer::FromHBITMAP(HBITMAP hBitmap, int targetWidth, int targetHeight)
    {
        BITMAP bm;
        GetObject(hBitmap, sizeof(BITMAP), &bm);

        int sourceWidth = bm.bmWidth;
        int sourceHeight = bm.bmHeight;

        targetWidth = (targetWidth <= 0) ? sourceWidth : targetWidth;
        targetHeight = (targetHeight <= 0) ? sourceHeight : targetHeight;

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

        ColorBuffer* colorBuffer = new ColorBuffer(targetWidth, targetHeight);

        for (int i = 0; i < targetWidth * targetHeight; ++i)
        {
            int x = i % targetWidth;
            int y = i / targetWidth;
            int srcX = x * sourceWidth / targetWidth;
            int srcY = y * sourceHeight / targetHeight;
            int index = (srcY * sourceWidth + srcX) * 4;
            colorBuffer->m_colors[i] = Color(buffer[index + 2], buffer[index + 1], buffer[index], buffer[index + 3]);
        }

        SelectObject(memDC, oldBitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, hdc);

        return colorBuffer;
    }
    #pragma endregion
}

extern "C"
{
    using namespace KTLib;

    #pragma region Constructors
    COLOR_API ColorBuffer* CreateColorBufferFromArray(Color** colors, int width, int height) { return new ColorBuffer(colors, width, height); }
    COLOR_API ColorBuffer* CreateColorBufferFromWHC(int width, int height, Color* defaultColor) { return new ColorBuffer(width, height, *defaultColor); }
    COLOR_API ColorBuffer* CreateColorBufferFromBuffer(unsigned int* buffer, int width, int height) { return new ColorBuffer(buffer, width, height); }
    COLOR_API ColorBuffer* CreateColorBufferFromHBITMAP(HBITMAP hBitmap, int targetWidth, int targetHeight) { return ColorBuffer::FromHBITMAP(hBitmap, targetWidth, targetHeight); }
    COLOR_API ColorBuffer* CreateColorBufferFromGradient(Gradient* gradient, int width, int height) { return new ColorBuffer(*gradient, width, height); }
    #pragma endregion

    #pragma region ColorBuffer Functions
    COLOR_API void ShiftRedColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftRed(amount); }
    COLOR_API void ShiftGreenColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftGreen(amount); }
    COLOR_API void ShiftBlueColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftBlue(amount); }
    COLOR_API void ShiftAlphaColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftAlpha(amount); }
    COLOR_API void SetRedColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetRed(value); }
    COLOR_API void SetGreenColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetGreen(value); }
    COLOR_API void SetBlueColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetBlue(value); }
    COLOR_API void SetAlphaColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetAlpha(value); }

    COLOR_API void RotateColorBuffer(ColorBuffer* colorBuffer, double angle) { colorBuffer->Rotate(angle); }
    COLOR_API void ResizeColorBuffer(ColorBuffer* colorBuffer, int newWidth, int newHeight, int resizeImage, Color* fillColor) { colorBuffer->Resize(newWidth, newHeight, resizeImage, *fillColor); }
    COLOR_API void ScaleColorBuffer(ColorBuffer* colorBuffer, double scale) { colorBuffer->Scale(scale); }
    COLOR_API void DeleteColorBuffer(ColorBuffer* colorBuffer) { delete colorBuffer; }

    COLOR_API int GetColorBufferWidth(ColorBuffer* colorBuffer) { return colorBuffer->GetWidth(); }
    COLOR_API int GetColorBufferHeight(ColorBuffer* colorBuffer) { return colorBuffer->GetHeight(); }
    COLOR_API int GetColorBufferSize(ColorBuffer* colorBuffer) { return colorBuffer->GetSize(); }
    COLOR_API int GetColorBufferStride(ColorBuffer* colorBuffer) { return colorBuffer->GetStride(); }
    COLOR_API Color* ColorBufferGetAt(ColorBuffer* colorBuffer, int index) { return new Color(colorBuffer->GetAt(index)); }
    COLOR_API void ColorBufferSetAt(ColorBuffer* colorBuffer, int index, Color* color) { colorBuffer->SetAt(index, *color); }
    COLOR_API void ColorBufferGetXY(ColorBuffer* colorBuffer, int index, int* x, int* y) { colorBuffer->GetXY(index, *x, *y); }
    COLOR_API void ColorBufferGetIndex(ColorBuffer* colorBuffer, int x, int y, int* index) { colorBuffer->GetIndex(x, y, *index); }

    COLOR_API unsigned int GetColorIntFromBuffer(ColorBuffer* colorBuffer, int x, int y) { return colorBuffer->Get(x, y).ToInt(0); }
    COLOR_API void SetColorIntInBuffer(ColorBuffer* colorBuffer, int x, int y, unsigned int colorInt) { colorBuffer->Set(x, y, Color(colorInt)); }
    COLOR_API Color* GetColorFromBuffer(ColorBuffer* colorBuffer, int x, int y) { return new Color(colorBuffer->Get(x, y)); }
    COLOR_API void SetColorInBuffer(ColorBuffer* colorBuffer, int x, int y, Color* color) { colorBuffer->Set(x, y, *color); }
    #pragma endregion

    #pragma region Color Modification Functions
    COLOR_API void ShiftHueColorBuffer(ColorBuffer* colorBuffer, double degrees) { colorBuffer->ShiftHue(degrees); }
    COLOR_API void ShiftSaturationColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftSaturation(amount); }
    COLOR_API void ShiftLightnessColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftLightness(amount); }
    COLOR_API void ShiftValueColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftValue(amount); }
    COLOR_API void ShiftIntensityColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftIntensity(amount); }
    COLOR_API void ShiftWhiteLevelColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftWhiteLevel(amount); }
    COLOR_API void ShiftBlackLevelColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftBlackLevel(amount); }
    COLOR_API void ShiftContrastColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftContrast(amount); }
    COLOR_API void AdjustContrastColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->AdjustContrast(factor); }
    COLOR_API void AdjustColorBalanceColorBuffer(ColorBuffer* colorBuffer, double redFactor, double greenFactor, double blueFactor) { colorBuffer->AdjustColorBalance(redFactor, greenFactor, blueFactor); }

    COLOR_API void OverlayImageColorBuffer(ColorBuffer* destBuffer, ColorBuffer* overlayBuffer, int x, int y, double opacity) { destBuffer->OverlayImage(*overlayBuffer, x, y, opacity); }
    COLOR_API void InvertColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Invert(); }
    COLOR_API void GrayscaleColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Grayscale(); }
    COLOR_API void SepiaColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Sepia(factor); }
    COLOR_API void PixelateColorBuffer(ColorBuffer* colorBuffer, int pixelSize) { colorBuffer->Pixelate(pixelSize); }
    COLOR_API void BlurColorBuffer(ColorBuffer* colorBuffer, int radius) { colorBuffer->Blur(radius); }
    COLOR_API void GaussianBlurColorBuffer(ColorBuffer* colorBuffer, int radius) { colorBuffer->GaussianBlur(radius); }
    COLOR_API void SharpenColorBuffer(ColorBuffer* colorBuffer, float amount) { colorBuffer->Sharpen(amount); }
    COLOR_API void CrossProcessColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->CrossProcess(factor); }
    COLOR_API void MoonlightColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Moonlight(factor); }
    COLOR_API void VintageFilmColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->VintageFilm(factor); }
    COLOR_API void TechnicolorColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Technicolor(factor); }
    COLOR_API void PolaroidColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Polaroid(factor); }
    COLOR_API void ComplementColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Complement(); }
    COLOR_API void ApplyEmbossColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Emboss(); }
    COLOR_API void ApplyEdgeDetectColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->EdgeDetect(); }
    COLOR_API void ApplyVignetteColorBuffer(ColorBuffer* colorBuffer, double strength, double radius) { colorBuffer->Vignette(strength, radius); }
    COLOR_API void ApplyTwoColorNoiseColorBuffer(ColorBuffer* colorBuffer, double density, Color* saltColor, Color* pepperColor) { colorBuffer->TwoColorNoise(density, *saltColor, *pepperColor); }
    COLOR_API void ApplyGaussianNoiseColorBuffer(ColorBuffer* colorBuffer, double mean, double stdDev) { colorBuffer->GaussianNoise(mean, stdDev); }
    COLOR_API void ApplyPerlinNoiseColorBuffer(ColorBuffer* colorBuffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { colorBuffer->PerlinNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplySimplexNoiseColorBuffer(ColorBuffer* colorBuffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { colorBuffer->SimplexNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyFractalBrownianMotionColorBuffer(ColorBuffer* colorBuffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { colorBuffer->FractalBrownianMotion(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyVoronoiDiagramColorBuffer(ColorBuffer* colorBuffer, int numberOfPoints, double falloff, double strength) { colorBuffer->Voronoi(numberOfPoints, falloff, strength); }
    COLOR_API void PlasmaEffectColorBuffer(ColorBuffer* colorBuffer, double frequency, double phase) { colorBuffer->Plasma(frequency, phase); }
    COLOR_API void DiamondSquareEffectColorBuffer(ColorBuffer* colorBuffer, double roughness, double waterLevel, double levelsPerStop) { colorBuffer->DiamondSquare(roughness, waterLevel, levelsPerStop); }
    COLOR_API void PosterizeColorBuffer(ColorBuffer* colorBuffer, int levels) { colorBuffer->Posterize(levels); }
    #pragma endregion

    #pragma region Utility
    COLOR_API void FlipColorBuffer(ColorBuffer* colorBuffer, bool horizontal) { colorBuffer->Flip(horizontal); }
    COLOR_API void CropColorBuffer(ColorBuffer* colorBuffer, int x, int y, int width, int height) { colorBuffer->Crop(x, y, width, height); }
    COLOR_API Color* AverageColorBuffer(ColorBuffer* colorBuffer) { return new Color(colorBuffer->CalculateAverageColor(0, 0, colorBuffer->GetWidth(), colorBuffer->GetHeight())); }
    COLOR_API ColorBuffer* CopyColorBufferRegion(ColorBuffer* colorBuffer, int x, int y, int w, int h) { return colorBuffer->CopyRegion(x, y, w, h); }
    COLOR_API ColorBuffer* CopyColorBuffer(ColorBuffer* colorBuffer) { return colorBuffer->Copy(); }

    COLOR_API int ColorBufferFind(ColorBuffer* buffer, Color* color) { return buffer->Find(*color); };
    COLOR_API int ColorBufferFindLast(ColorBuffer* buffer, Color* color) { return buffer->FindLast(*color); }
    COLOR_API void ColorBufferForEach(ColorBuffer* buffer, void (*func)(Color*)) { buffer->ForEach([func](const Color& color) { func(const_cast<Color*>(&color)); }); }
    COLOR_API void ColorBufferSwap(ColorBuffer* buffer, int index1, int index2) { buffer->Swap(index1, index2); }
    COLOR_API ColorBuffer* ColorBufferFilter(ColorBuffer* buffer, bool (*predicate)(Color*)) { return new ColorBuffer(buffer->Filter([predicate](const Color& color) { return predicate(const_cast<Color*>(&color)); })); }
    COLOR_API int ColorBufferCount(ColorBuffer* buffer, Color* color) { return buffer->Count(*color); }
    COLOR_API void ColorBufferShuffle(ColorBuffer* buffer) { buffer->Shuffle(); }
    COLOR_API void ColorBufferClear(ColorBuffer* buffer) { buffer->Clear(); }
    COLOR_API void ColorBufferSort(ColorBuffer* buffer, int (*compare)(Color*, Color*)) { buffer->Sort([compare](const Color& a, const Color& b) { return compare(const_cast<Color*>(&a), const_cast<Color*>(&b)) < 0; }); }
    COLOR_API void ColorBufferAppendRight(ColorBuffer* buffer, ColorBuffer* other) { buffer->AppendRight(*other); }
    COLOR_API void ColorBufferAppendBottom(ColorBuffer* buffer, ColorBuffer* other) { buffer->AppendBottom(*other); }

    COLOR_API void MapColorsInBuffer(ColorBuffer* colorBuffer, int x, int y, int width, int height, void* mapFunction)
    {
        auto func = reinterpret_cast<unsigned int(*)(int, int, unsigned int)>(mapFunction);
        colorBuffer->MapColors(x, y, width, height, func);
    }

    COLOR_API int* ColorBufferFindAll(ColorBuffer* buffer, Color* color, int* count)
    {
        auto result = buffer->FindAll(*color);
        *count = result.size();
        int* arr = new int[*count];
        std::copy(result.begin(), result.end(), arr);
        return arr;
    }

    COLOR_API void ColorBufferApplyMatrix(ColorBuffer* buffer,
        double m11, double m12, double m13, double m14, double m15,
        double m21, double m22, double m23, double m24, double m25,
        double m31, double m32, double m33, double m34, double m35,
        double m41, double m42, double m43, double m44, double m45,
        double m51, double m52, double m53, double m54, double m55)
    {
        ColorMatrix matrix;
        double m[25] = {
            m11, m12, m13, m14, m15,
            m21, m22, m23, m24, m25,
            m31, m32, m33, m34, m35,
            m41, m42, m43, m44, m45,
            m51, m52, m53, m54, m55
        };

        for(int i = 0; i < 25; i++) matrix.data[i/5][i%5] = m[i];

        #pragma omp parallel for
        for (int i = 0; i < buffer->GetWidth() * buffer->GetHeight(); i++)
        {
            Color color = buffer->GetAt(i);
            buffer->SetAt(i, color * matrix);
        }
    }

    COLOR_API HBITMAP ExportColorBufferAsHBitmap(ColorBuffer* colorBuffer, int targetWidth, int targetHeight) { return colorBuffer->ToHBITMAP(targetWidth, targetHeight); }
    #pragma endregion
}