#define NOMINMAX

#include "Color.h"
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
            m_colorStops.push_back({colors[i], i * step});
        }
    }
    #pragma endregion

    #pragma region Gradient Functions
    template<typename Operation>
    void Gradient::ApplyToAllStops(Operation op)
    {
        for (auto& stop : m_colorStops)
        {
            Color color(stop.color);
            op(color);
            stop.color = color.ToInt();
        }
    }

    void Gradient::AddColorStop(unsigned int color, GradientPosition position)
    {
        m_colorStops.push_back({color, position});
        std::sort(m_colorStops.begin(), m_colorStops.end(), [](const ColorStop& a, const ColorStop& b) { return a.position.value < b.position.value; });
    }

    void Gradient::RemoveColorStop(float position)
    {
        m_colorStops.erase(
            std::remove_if(m_colorStops.begin(), m_colorStops.end(),
                [position](const ColorStop& stop) { return std::abs(stop.position.value - position) < 1e-6; }),
            m_colorStops.end());
    }

    Color Gradient::GetColorAt(GradientPosition position) const
    {
        if (position.value <= 0.0f || m_colorStops.size() == 1) return Color(m_colorStops.front().color);
        if (position.value >= 1.0f) return Color(m_colorStops.back().color);

        if (m_colorStops.empty()) return Color(0, 0, 0);
        if (m_colorStops.size() == 1) return Color(m_colorStops[0].color);

        // Find the two color stops to interpolate between
        auto it = std::lower_bound(m_colorStops.begin(), m_colorStops.end(), position.value, [](const ColorStop& stop, float pos) { return stop.position.value < pos; });
        if (it == m_colorStops.begin()) return Color(it->color);
        if (it == m_colorStops.end()) return Color((it - 1)->color);

        const ColorStop& stop1 = *(it - 1);
        const ColorStop& stop2 = *it;

        // Perform linear interpolation
        float t = (position.value - stop1.position.value) / (stop2.position.value - stop1.position.value);
        return Color::Mix(Color(stop1.color), Color(stop2.color), t);
    }

    Color Gradient::GetColorAtStep(int step) const
    {
        if (step <= 0) return Color(m_colorStops.front().color);
        if (step >= m_totalSteps - 1) return Color(m_colorStops.back().color);

        float position = static_cast<float>(step) / (m_totalSteps - 1);
        return GetColorAt(GradientPosition::FromNormalized(position));
    }

    void Gradient::Rotate(float angle)
    {
        float normalizedAngle = std::fmod(angle, 360.0f) / 360.0f;
        if (normalizedAngle < 0) normalizedAngle += 1.0f;

        for (auto& stop : m_colorStops)
        {
            stop.position.value = std::fmod(stop.position.value + normalizedAngle, 1.0f);
        }

        std::sort(m_colorStops.begin(), m_colorStops.end(), [](const ColorStop& a, const ColorStop& b) { return a.position.value < b.position.value; });
    }

    void Gradient::Reverse()
    {
        for (auto& stop : m_colorStops)
        {
            stop.position.value = 1.0f - stop.position.value;
        }
        std::reverse(m_colorStops.begin(), m_colorStops.end());
    }

    void Gradient::Shift(float amount)
    {
        for (auto& stop : m_colorStops)
        {
            stop.position.value = std::fmod(stop.position.value + amount, 1.0f);
            if (stop.position.value < 0) stop.position.value += 1.0f;
        }

        std::sort(m_colorStops.begin(), m_colorStops.end(), [](const ColorStop& a, const ColorStop& b) { return a.position.value < b.position.value; });
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
        switch(m_type) {
            case GradientType::Linear: typeStr = "linear"; break;
            case GradientType::Radial: typeStr = "radial"; break;
            case GradientType::Conical: typeStr = "conical"; break;
        }

        oss << "{\"type\":\"" << typeStr
            << "\",\"angle\":" << m_angle
            << "\",\"totalSteps\":" << m_totalSteps
            << ",\"colorStops\":[";

        // Rest of serialization remains the same
        for (size_t i = 0; i < m_colorStops.size(); ++i)
        {
            if (i > 0) oss << ",";
            oss << "{\"color\":\"" << std::hex << std::setw(8) << std::setfill('0') << m_colorStops[i].color
                << "\",\"position\":" << std::defaultfloat << m_colorStops[i].position.value << "}";
        }

        oss << "]}";
        return oss.str();
    }

    Gradient Gradient::Deserialize(const std::string& data)
    {
        std::string cleanData;
        std::copy_if(data.begin(), data.end(), std::back_inserter(cleanData),
            [](char c) { return !std::isspace(c); });

        std::istringstream iss(cleanData);

        int type = 0, totalSteps = 0;
        float angle = 0.0f;
        std::vector<std::pair<unsigned int, float>> colorStops;

        auto parseType = [](const std::string& type) -> GradientType
        {
            if (type == "linear") return GradientType::Linear;
            if (type == "radial") return GradientType::Radial;
            if (type == "conical") return GradientType::Conical;
            return GradientType::Linear; // default
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

            return std::make_pair(
                std::stoul(colorStr, nullptr, 16),
                std::stof(posStr)
            );
        };

        iss.ignore(std::numeric_limits<std::streamsize>::max(), '{');
        std::string token;
        while (std::getline(iss, token, ':'))
        {
            token.erase(remove_if(token.begin(), token.end(), [](char c) { return c == '"' || c == ' ' || c == ',' || c == '{'; }), token.end());

            if (token == "type")
            {
                std::string typeStr;
                std::getline(iss, typeStr, ',');
                typeStr.erase(remove_if(typeStr.begin(), typeStr.end(), [](char c) { return c == '"' || c == ' '; }), typeStr.end());
                type = static_cast<int>(parseType(typeStr));
            }
            else if (token == "angle") angle = std::stof(parseValue(iss));
            else if (token == "totalSteps") totalSteps = std::stoi(parseValue(iss));
            else if (token == "colorStops")
            {
                while (iss.peek() != ']') colorStops.push_back(parseColorStop(iss));
                break;
            }
        }

        std::vector<unsigned int> colors;
        std::transform(colorStops.begin(), colorStops.end(),
            std::back_inserter(colors), [](const auto& stop) { return stop.first; });

        Gradient gradient(totalSteps, colors);

        for (size_t i = 0; i < colorStops.size(); ++i) gradient.m_colorStops[i].position.value = colorStops[i].second;

        gradient.SetType(static_cast<GradientType>(type));
        gradient.SetAngle(angle);

        return gradient;
    }

    HBITMAP Gradient::CreateHBITMAP(int width, int height) const
    {
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;  // Top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = 0;  // 0 for BI_RGB
        bmi.bmiHeader.biXPelsPerMeter = 0;
        bmi.bmiHeader.biYPelsPerMeter = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;

        void* pvBits;
        HBITMAP hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);

        if (hBitmap)
        {
            DWORD* pixels = static_cast<DWORD*>(pvBits);
            float centerX = width / 2.0f;
            float centerY = height / 2.0f;
            float maxRadius = std::sqrt(centerX * centerX + centerY * centerY);

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    float position;
                    switch (m_type)
                    {
                        case GradientType::Linear:
                        {
                            float radians = m_angle * CONST_PI / 180.0f;
                            float dx = std::cos(radians);
                            float dy = std::sin(radians);
                            position = (x * dx + y * dy) / (width * std::abs(dx) + height * std::abs(dy));
                            break;
                        }
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
                            float rotationRadians = m_angle * CONST_PI / 180.0f; // Negative for clockwise rotation
                            angle -= rotationRadians; // Subtract rotation to move start/end points
                            position = (angle + CONST_PI) / (2 * CONST_PI);
                            position = std::fmod(position + 1.0f, 1.0f); // Normalize to [0,1]
                            break;
                        }
                    }

                    position = std::clamp(position, 0.0f, 1.0f);
                    position = std::round(position * (m_totalSteps - 1)) / (m_totalSteps - 1);
                    pixels[y * width + x] = GetColorAt(GradientPosition::FromNormalized(position)).ToInt();
                }
            }
        }

        return hBitmap;
    }
    #pragma endregion
}

extern "C"
{
    using namespace KTLib;

    #pragma region Constructors
    COLOR_API Gradient* CreateGradient(int totalSteps, unsigned int* colors, int colorCount)
    {
        std::vector<unsigned int> colorVec(colors, colors + colorCount);
        return new Gradient(totalSteps, colorVec);
    }
    #pragma endregion

    #pragma region Gradient Functions
    COLOR_API void DeleteGradient(Gradient* gradient) { delete gradient; }
    COLOR_API void GradientSetType(Gradient* gradient, int type) { gradient->SetType(static_cast<GradientType>(type)); }
    COLOR_API void GradientAddColorStop(Gradient* gradient, unsigned int color, float position) { gradient->AddColorStop(color, GradientPosition::FromNormalized(position)); }
    COLOR_API void GradientRemoveColorStop(Gradient* gradient, float position) { gradient->RemoveColorStop(position); }
    COLOR_API Color* GradientGetColorAt(Gradient* gradient, float position) { return new Color(gradient->GetColorAt(GradientPosition::FromNormalized(position))); }
    COLOR_API void GradientRotate(Gradient* gradient, float angle) { gradient->Rotate(angle); }
    COLOR_API void GradientReverse(Gradient* gradient) { gradient->Reverse(); }
    COLOR_API void GradientShift(Gradient* gradient, float amount) { gradient->Shift(amount); }
    #pragma endregion

    #pragma region Color Modification Functions
    COLOR_API void GradientShiftHue(Gradient* gradient, double degrees) { gradient->ShiftHue(degrees); }
    COLOR_API void GradientShiftSaturation(Gradient* gradient, double amount) { gradient->ShiftSaturation(amount); }
    COLOR_API void GradientShiftLightness(Gradient* gradient, double amount) { gradient->ShiftLightness(amount); }
    COLOR_API void GradientShiftWhiteness(Gradient* gradient, double amount) { gradient->ShiftWhiteLevel(amount); }
    COLOR_API void GradientShiftBlackness(Gradient* gradient, double amount) { gradient->ShiftBlackLevel(amount); }
    COLOR_API void GradientShiftValue(Gradient* gradient, double amount) { gradient->ShiftValue(amount); }
    COLOR_API void GradientGrayscale(Gradient* gradient) { gradient->Grayscale(); }
    COLOR_API void GradientSepia(Gradient* gradient, double factor) { gradient->Sepia(factor); }
    COLOR_API void GradientInvert(Gradient* gradient) { gradient->Invert(); }
    COLOR_API void GradientComplement(Gradient* gradient) { gradient->Complement(); }
    COLOR_API int GradientGetTotalSteps(Gradient* gradient) { return gradient->GetTotalSteps(); }
    COLOR_API Color* GradientGetColorAtStep(Gradient* gradient, int step) { return new Color(gradient->GetColorAtStep(step)); }
    #pragma endregion

    #pragma region Utility
    COLOR_API const char* GradientSerialize(Gradient* gradient)
    {
        static std::string serializedData;
        serializedData = gradient->Serialize();
        return serializedData.c_str();
    }

    COLOR_API Gradient* GradientDeserialize(const char* data)
    {
        try
        {
            if (!data || !*data) return nullptr;

            auto gradient = new Gradient(Gradient::Deserialize(data));
            return gradient;
        }
        catch (const std::exception& e)
        {
            return nullptr;
        }
    }

    COLOR_API HBITMAP GradientCreateHBITMAP(Gradient* gradient, int width, int height) { return gradient->CreateHBITMAP(width, height); }
    #pragma endregion
}