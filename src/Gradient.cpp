#include "../include/Gradient.hpp"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace KTLib
{
    #pragma region Constructors
    Gradient::Gradient(int totalSteps, const std::vector<unsigned int>& colors) : m_totalSteps(totalSteps), m_type(GradientType::Linear)
    {
        if (colors.size() < 2)
        {
            throw std::invalid_argument("At least two colors are required to create a gradient.");
        }

        float step = 1.0f / (colors.size() - 1);
        for (size_t i = 0; i < colors.size(); ++i)
        {
            m_colorStops.push_back(ColorStop{i * step, Color(colors[i])});
        }
    }

    Gradient::Gradient(int totalSteps) : m_totalSteps(totalSteps)
    {
        m_colorStops.push_back(ColorStop(0.0f, Color::Red()));
        m_colorStops.push_back(ColorStop(0.17f, Color::Yellow()));
        m_colorStops.push_back(ColorStop(0.33f, Color::Lime()));
        m_colorStops.push_back(ColorStop(0.5f, Color::Aqua()));
        m_colorStops.push_back(ColorStop(0.67f, Color::Blue()));
        m_colorStops.push_back(ColorStop(0.83f, Color::Fuchsia()));
        m_colorStops.push_back(ColorStop(1.0f, Color::Red()));
    }
    #pragma endregion

    #pragma region Gradient Functions
    void Gradient::AddColorStop(const Color& color, float position)
    {
        m_colorStops.push_back({position, color});
        std::sort(m_colorStops.begin(), m_colorStops.end(), [](const ColorStop& a, const ColorStop& b) { return a.position < b.position; });
        processColorStops();
    }

    Color Gradient::interpolateColors(const Color& color1, const Color& color2, float t) const { return Color::Mix(color1, color2, t); }

    void Gradient::processColorStops()
    {
        m_colors.resize(m_totalSteps);

        for (int i = 0; i < m_totalSteps; ++i)
        {
            float position = static_cast<float>(i) / (m_totalSteps - 1);

            auto it = std::lower_bound(m_colorStops.begin(), m_colorStops.end(), position,
                [](const ColorStop& stop, float pos) { return stop.position < pos; });

            if (it == m_colorStops.begin())
                m_colors[i] = it->color;
            else if (it == m_colorStops.end())
                m_colors[i] = (it - 1)->color;
            else
            {
                const ColorStop& stop1 = *(it - 1);
                const ColorStop& stop2 = *it;
                float t = (position - stop1.position) / (stop2.position - stop1.position);
                m_colors[i] = interpolateColors(stop1.color, stop2.color, t);
            }
        }
    }

    float Gradient::CalculatePosition(float x, float y, float centerX, float centerY, float maxRadius) const
    {
        const float radians = m_angle * CONST_PI / 180.0f;

        // Apply focus point offset
        const float fx = centerX + (m_focusX * maxRadius);
        const float fy = centerY + (m_focusY * maxRadius);
        float dx = x - fx;
        float dy = y - fy;
        const float radius = std::sqrt(dx * dx + dy * dy);
        float angle = std::atan2(dy, dx) - radians;

        float position;
        switch(m_type)
        {
            case GradientType::Linear:
            {
                const float linearDx = std::cos(radians);
                const float linearDy = std::sin(radians);

                float projection = (x - centerX) * linearDx + (y - centerY) * linearDy;
                position = (projection / maxRadius + 1.0f) * 0.5f;

                float perpProjection = (x - centerX) * -linearDy + (y - centerY) * linearDx;
                float wavePhase = perpProjection / maxRadius * CONST_PI * m_wavelength;
                position += std::sin(wavePhase) * m_amplitude * 0.5f;

                position = position * m_repetitions;
                position -= std::floor(position);
                break;
            }
            case GradientType::Radial:
            {
                // Base position from radius
                position = radius / maxRadius;

                if (m_vertices >= 3)
                {
                    // Create polygon shape
                    const float vertexAngle = std::fmod(angle + 4 * CONST_PI, (2 * CONST_PI / m_vertices)) - (CONST_PI / m_vertices);
                    position *= std::pow(std::cos(vertexAngle), m_edgeSharpness);

                    // Add burst effect when wavelength and amplitude are non-zero
                    if (m_wavelength > 0 && m_amplitude > 0)
                    {
                        float burstAngle = angle * m_vertices;
                        float burstFactor = std::abs(std::cos(burstAngle / 2));
                        float wavePhase = radius / maxRadius * CONST_PI * m_wavelength;
                        float waveFactor = std::pow(std::cos(wavePhase), 2);
                        position *= 1.0f + (burstFactor * waveFactor * m_amplitude);
                    }
                }
                else
                {
                    // Circular gradient with wave effects
                    if (m_wavelength > 0 && m_amplitude > 0)
                    {
                        float wavePhase = radius / maxRadius * CONST_PI * m_wavelength;
                        float waveFactor = std::pow(std::cos(wavePhase), 2);
                        position *= 1.0f + (waveFactor * m_amplitude);
                    }
                }

                if (m_repetitions <= 1.0f)
                {
                    position = std::clamp(position, 0.0f, 1.0f);
                }
                else
                {
                    position = position * m_repetitions;
                    position -= std::floor(position);
                }

                break;
            }
            case GradientType::Conical:
            {
                float angle = std::atan2(dy, dx) - radians;
                if (angle < 0) angle += 4 * CONST_PI;

                if (m_wavelength > 0)
                {
                    float radialWave = std::sin(radius / maxRadius * CONST_PI * 2.0f * m_wavelength);
                    angle += radialWave * m_amplitude * CONST_PI;
                }

                position = std::fmod(angle * m_repetitions, 2 * CONST_PI) / (2 * CONST_PI);
                if (position < 0) position += 1.0f;

                break;
            }
        }

        // After calculating final position, quantize it to match total steps
        position = floor(position * m_totalSteps) / (m_totalSteps - 1);
        return std::clamp(position, 0.0f, 1.0f);
    }

    void Gradient::RemoveColorStop(float position)
    {
        m_colorStops.erase(
            std::remove_if(m_colorStops.begin(), m_colorStops.end(),
                [position](const ColorStop& stop) { return std::abs(stop.position - position) < 1e-6; }),
            m_colorStops.end());
    }

    Color Gradient::GetColorAt(float position) const
    {
        // Handle edge cases
        if (m_colorStops.empty()) return Color::Black();
        if (m_colorStops.size() == 1) return m_colorStops[0].color;

        // Find bounding stops
        auto upper = std::lower_bound(m_colorStops.begin(), m_colorStops.end(), position,
            [](const ColorStop& stop, float pos) { return stop.position < pos; });

        if (upper == m_colorStops.begin()) return upper->color;
        if (upper == m_colorStops.end()) return (--upper)->color;

        auto lower = upper - 1;

        // Calculate interpolation factor
        float t = (position - lower->position) / (upper->position - lower->position);

        // Use our existing interpolation function
        return interpolateColors(lower->color, upper->color, t);
    }

    Color Gradient::GetColorAtStep(int step) const
    {
        if (step <= 0) return Color(m_colorStops.front().color);
        if (step >= m_totalSteps - 1) return Color(m_colorStops.back().color);

        float position = static_cast<float>(step) / (m_totalSteps - 1);
        return GetColorAt(position);
    }

    void Gradient::Rotate(float angle)
    {
        float normalizedAngle = std::fmod(angle, 360.0f) / 360.0f;
        if (normalizedAngle < 0) normalizedAngle += 1.0f;

        for (auto& stop : m_colorStops) stop.position = std::fmod(stop.position + normalizedAngle, 1.0f);

        std::sort(m_colorStops.begin(), m_colorStops.end(), [](const ColorStop& a, const ColorStop& b) { return a.position < b.position; });
    }

    void Gradient::Reverse()
    {
        for (auto& stop : m_colorStops) stop.position = 1.0f - stop.position;
        std::reverse(m_colorStops.begin(), m_colorStops.end());
    }

    void Gradient::Shift(float amount)
    {
        for (auto& stop : m_colorStops)
        {
            stop.position = std::fmod(stop.position + amount, 1.0f);
            if (stop.position < 0) stop.position += 1.0f;
        }

        std::sort(m_colorStops.begin(), m_colorStops.end(), [](const ColorStop& a, const ColorStop& b) { return a.position < b.position; });
    }
    #pragma endregion

    #pragma region Color Modification Functions
    void Gradient::ShiftHue(double degrees) { ApplyToAllStops([degrees](Color& color) { color.ShiftHue(degrees); }); }

    void Gradient::ShiftSaturation(double amount) { ApplyToAllStops([amount](Color& color) { color.ShiftSaturation(amount); }); }

    void Gradient::ShiftLightness(double amount) { ApplyToAllStops([amount](Color& color) { color.ShiftLightness(amount); }); }

    void Gradient::ShiftWhiteLevel(double amount) { ApplyToAllStops([amount](Color& color) { color.ShiftWhiteLevel(amount); }); }

    void Gradient::ShiftBlackLevel(double amount) { ApplyToAllStops([amount](Color& color) { color.ShiftBlackLevel(amount); }); }

    void Gradient::ShiftValue(double amount) { ApplyToAllStops([amount](Color& color) { color.ShiftValue(amount); }); }

    void Gradient::Grayscale() { ApplyToAllStops([](Color& color) { color.Grayscale(); }); }

    void Gradient::Sepia(double factor) { ApplyToAllStops([factor](Color& color) { color.Sepia(factor); }); }

    void Gradient::Invert() { ApplyToAllStops([](Color& color) { color.Invert(); }); }

    void Gradient::Complement() { ApplyToAllStops([](Color& color) { color.Complement(); }); }
    #pragma endregion

    #pragma region Utility
    std::string Gradient::Serialize() const
    {
        std::ostringstream oss;

        std::string typeStr;
        switch(m_type)
        {
            case GradientType::Linear: typeStr = "linear"; break;
            case GradientType::Radial: typeStr = "radial"; break;
            case GradientType::Conical: typeStr = "conical"; break;
        }

        oss << "{\"type\":\"" << typeStr
            << "\",\"angle\":" << m_angle
            << ",\"totalSteps\":" << m_totalSteps
            << ",\"vertices\":" << m_vertices
            << ",\"focusX\":" << m_focusX
            << ",\"focusY\":" << m_focusY
            << ",\"edgeSharpness\":" << m_edgeSharpness
            << ",\"wavelength\":" << m_wavelength
            << ",\"amplitude\":" << m_amplitude
            << ",\"repetitions\":" << m_repetitions
            << ",\"colorStops\":[";

        for (size_t i = 0; i < m_colorStops.size(); ++i)
        {
            if (i > 0) oss << ",";
            oss << "{\"color\":" << m_colorStops[i].color.ToInt()
                << ",\"position\":" << m_colorStops[i].position << "}";
        }

        oss << "]}";
        return oss.str();
    }

    Gradient Gradient::Deserialize(const std::string& data)
    {
        std::string cleanData;
        std::copy_if(data.begin(), data.end(), std::back_inserter(cleanData), [](char c) { return !std::isspace(c); });

        std::istringstream iss(cleanData);

        int type = 0, totalSteps = 0;
        float angle = 0.0f, vertices = 0.0f, focusX = 0.0f, focusY = 0.0f;
        float edgeSharpness = 1.0f, wavelength = 1.0f, amplitude = 0.0f, repetitions = 1.0f;
        std::vector<std::pair<unsigned int, float>> colorStops;

        auto parseType = [](const std::string& type) -> GradientType
        {
            if (type == "linear") return GradientType::Linear;
            if (type == "radial") return GradientType::Radial;
            if (type == "conical") return GradientType::Conical;
            return GradientType::Linear;
        };

        auto parseValue = [](std::istringstream& stream)
        {
            std::string value;
            std::getline(stream, value, ',');
            return value;
        };

        auto parseColorStop = [](std::istringstream& stream)
        {
            stream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
            stream.ignore(std::numeric_limits<std::streamsize>::max(), '"');

            std::string colorStr;
            std::getline(stream, colorStr, '"');

            stream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
            std::string posStr;
            std::getline(stream, posStr, '}');

            return std::make_pair(std::stoul(colorStr, nullptr, 16), std::stof(posStr));
        };

        iss.ignore(std::numeric_limits<std::streamsize>::max(), '{');
        std::string token;
        while (std::getline(iss, token, ':'))
        {
            token.erase(remove_if(token.begin(), token.end(),
                       [](char c) { return c == '"' || c == ' ' || c == ',' || c == '{'; }),
                       token.end());

            if (token == "type") {
                std::string typeStr;
                std::getline(iss, typeStr, ',');
                typeStr.erase(remove_if(typeStr.begin(), typeStr.end(),
                             [](char c) { return c == '"' || c == ' '; }),
                             typeStr.end());
                type = static_cast<int>(parseType(typeStr));
            }
            else if (token == "angle") angle = std::stof(parseValue(iss));
            else if (token == "totalSteps") totalSteps = std::stoi(parseValue(iss));
            else if (token == "vertices") vertices = std::stof(parseValue(iss));
            else if (token == "focusX") focusX = std::stof(parseValue(iss));
            else if (token == "focusY") focusY = std::stof(parseValue(iss));
            else if (token == "edgeSharpness") edgeSharpness = std::stof(parseValue(iss));
            else if (token == "wavelength") wavelength = std::stof(parseValue(iss));
            else if (token == "amplitude") amplitude = std::stof(parseValue(iss));
            else if (token == "repetitions") repetitions = std::stof(parseValue(iss));
            else if (token == "colorStops") {
                while (iss.peek() != ']') colorStops.push_back(parseColorStop(iss));
                break;
            }
        }

        std::vector<unsigned int> colors;
        std::transform(colorStops.begin(), colorStops.end(),
                      std::back_inserter(colors),
                      [](const auto& stop) { return stop.first; });

        Gradient gradient(totalSteps, colors);

        for (size_t i = 0; i < colorStops.size(); ++i) {
            gradient.m_colorStops[i].position = colorStops[i].second;
        }

        gradient.SetType(static_cast<GradientType>(type));
        gradient.SetAngle(angle);
        gradient.SetVertices(vertices);
        gradient.SetFocus(focusX, focusY);
        gradient.SetEdgeSharpness(edgeSharpness);
        gradient.SetWavelength(wavelength);
        gradient.SetAmplitude(amplitude);
        gradient.SetRepetitions(repetitions);

        return gradient;
    }

    HBITMAP Gradient::CreateHBITMAP(int width, int height) const
    {
        HDC screenDC = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(screenDC);

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = width * height * 4;

        void* bits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(screenDC, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);

        if (hBitmap)
        {
            ColorBuffer buffer(*this, width, height);
            for (int i = 0; i < width * height; i++) static_cast<unsigned int*>(bits)[i] = buffer[i].ToInt(0);
        }

        DeleteDC(memDC);
        ReleaseDC(NULL, screenDC);

        return hBitmap;
    }
    #pragma endregion
}