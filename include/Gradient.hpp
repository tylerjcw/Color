#pragma once

#include "Color.hpp"
#include "ColorBuffer.hpp"

#include <vector>
#include <cmath>

namespace KTLib
{
    enum class GradientType
    {
        Linear,
        Radial,
        Conical
    };

    struct ColorStop
    {
        float position;
        Color color;
        ColorStop(float pos, Color col) : position(pos), color(col) { }
    };

    class Gradient
    {
        public:
            Gradient() : m_totalSteps(2), m_type(KTLib::GradientType::Linear) {}
            Gradient(int totalSteps);
            Gradient(const Gradient& other) : m_totalSteps(other.m_totalSteps), m_type(other.m_type), m_colorStops(other.m_colorStops) {}
            Gradient(int totalSteps, const std::vector<unsigned int>& colors);

            // Assignment operator
            Gradient& operator=(const Gradient& other) {
                if (this != &other) {
                    m_totalSteps = other.m_totalSteps;
                    m_type = other.m_type;
                    m_colorStops = other.m_colorStops;
                }
                return *this;
            }

            void ShiftHue(double degrees);
            void ShiftSaturation(double amount);
            void ShiftLightness(double amount);
            void ShiftValue(double amount);
            void ShiftWhiteLevel(double amount);
            void ShiftBlackLevel(double amount);
            void Grayscale();
            void Sepia(double factor);
            void Invert();
            void Complement();
            void AddColorStop(const Color& color, float position);
            float CalculatePosition(float x, float y, float centerX, float centerY, float maxRadius) const;
            void RemoveColorStop(float position);
            const std::vector<ColorStop>& GetColorStops() const { return m_colorStops; }
            Color GetColorAt(float position) const;
            Color GetColorAtStep(int step) const;
            void Rotate(float angle);
            void Reverse();
            void Shift(float amount);
            std::string Serialize() const;
            static Gradient Deserialize(const std::string& data);
            HBITMAP CreateHBITMAP(int width, int height) const;
            void Draw(HWND hwnd, int x, int y, int width, int height) const;
            void SetTotalSteps(int totalSteps) { m_totalSteps = totalSteps; processColorStops(); }
            int GetTotalSteps() const { return m_totalSteps; }
            void SetType(GradientType type) { m_type = type; processColorStops(); }
            GradientType GetType() const { return m_type; }
            void SetAngle(float degrees) { m_angle = degrees; processColorStops(); }
            float GetAngle() const { return m_angle; }
            void SetVertices(int vertices) { m_vertices = std::max(0, vertices); processColorStops(); }
            int GetVertices() const { return m_vertices; }
            void SetFocus(float x, float y) { m_focusX = x; m_focusY = y; processColorStops(); }
            void GetFocus(float* x, float* y) const { *x = m_focusX; *y = m_focusY; }
            void SetEdgeSharpness(float sharpness) { m_edgeSharpness = sharpness; processColorStops(); }
            float GetEdgeSharpness() const { return m_edgeSharpness; }
            void SetWavelength(float wavelength) { m_wavelength = wavelength; processColorStops(); }
            float GetWavelength() const { return m_wavelength; }
            void SetAmplitude(float amplitude) { m_amplitude = amplitude; processColorStops(); }
            float GetAmplitude() const { return m_amplitude; }
            void SetRepetitions(float repetitions) { m_repetitions = std::max(0.0f, repetitions); processColorStops(); }
            float GetRepetitions() const { return m_repetitions; }

        private:
            int m_totalSteps;
            GradientType m_type = GradientType::Linear;
            std::vector<ColorStop> m_colorStops;
            std::vector<Color> m_colors;
            float m_angle = 0.0f;
            float m_vertices = 0.0f;      // 0 = disabled, >0 = enabled with n vertices
            float m_focusX = 0.0f;        // Radial center offset X (-1 to 1)
            float m_focusY = 0.0f;        // Radial center offset Y (-1 to 1)
            float m_edgeSharpness = 1.0f; // Controls edge transitions
            float m_wavelength = 1.0f;    // Number of waves across gradient
            float m_amplitude = 0.0f;     // Wave height (0-1+)
            float m_repetitions = 1.0f;   // Number of gradient repetitions
            void processColorStops();
            Color interpolateColors(const Color& color1, const Color& color2, float t) const;

            template<typename Operation>
            void ApplyToAllStops(Operation op) {
                for (auto& stop : m_colorStops) {
                    op(stop.color);
                }
            }
    };
}