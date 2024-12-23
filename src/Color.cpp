#include "../include/Color.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <random>

namespace KTLib
{
    #pragma region Operators
    Color Color::operator+(int value) const
    {
        return Color(
            std::clamp(r + value, 0, 255),
            std::clamp(g + value, 0, 255),
            std::clamp(b + value, 0, 255),
            std::clamp(a + value, 0, 255)
        );
    }

    Color Color::operator+(const Color& other) const {
        return Color(
            std::clamp(r + other.r, 0, 255),
            std::clamp(g + other.g, 0, 255),
            std::clamp(b + other.b, 0, 255),
            std::clamp(a + other.a, 0, 255)
        );
    }

    Color Color::operator+(const ColorMatrix& matrix) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        double newR = nR + matrix[0][0] + nG * matrix[0][1] + nB * matrix[0][2];
        double newG = nG + matrix[1][0] + nR * matrix[1][1] + nB * matrix[1][2];
        double newB = nB + matrix[2][0] + nR * matrix[2][1] + nG * matrix[2][2];

        return Color(
            std::clamp(newR * 255.0, 0.0, 255.0),
            std::clamp(newG * 255.0, 0.0, 255.0),
            std::clamp(newB * 255.0, 0.0, 255.0),
            a
        );
    }

    Color& Color::operator+=(int value)
    {
        r = std::clamp(r + value, 0, 255);
        g = std::clamp(g + value, 0, 255);
        b = std::clamp(b + value, 0, 255);
        a = std::clamp(a + value, 0, 255);
        return *this;
    }

    Color& Color::operator+=(const Color& other)
    {
        r = std::clamp(r + other.r, 0, 255);
        g = std::clamp(g + other.g, 0, 255);
        b = std::clamp(b + other.b, 0, 255);
        a = std::clamp(a + other.a, 0, 255);
        return *this;
    }

    Color& Color::operator+=(const ColorMatrix& matrix)
    {
        *this = *this + matrix;
        return *this;
    }

    Color Color::operator-(int value) const
    {
        return Color(
            std::clamp(r - value, 0, 255),
            std::clamp(g - value, 0, 255),
            std::clamp(b - value, 0, 255),
            std::clamp(a - value, 0, 255)
        );
    }

    Color Color::operator-(const Color& other) const
    {
        return Color(
            std::clamp(r - other.r, 0, 255),
            std::clamp(g - other.g, 0, 255),
            std::clamp(b - other.b, 0, 255),
            std::clamp(a - other.a, 0, 255)
        );
    }

    Color Color::operator-(const ColorMatrix& matrix) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        nR = nR - (matrix[0][0] + nG * matrix[0][1] + nB * matrix[0][2]);
        nG = nG - (matrix[1][0] + nR * matrix[1][1] + nB * matrix[1][2]);
        nB = nB - (matrix[2][0] + nR * matrix[2][1] + nG * matrix[2][2]);

        return Color(
            std::clamp(nR * 255.0, 0.0, 255.0),
            std::clamp(nG * 255.0, 0.0, 255.0),
            std::clamp(nB * 255.0, 0.0, 255.0),
            a
        );
    }

    Color& Color::operator-=(int value)
    {
        r = std::clamp(r - value, 0, 255);
        g = std::clamp(g - value, 0, 255);
        b = std::clamp(b - value, 0, 255);
        a = std::clamp(a - value, 0, 255);
        return *this;
    }

    Color& Color::operator-=(const Color& other)
    {
        r = std::clamp(r - other.r, 0, 255);
        g = std::clamp(g - other.g, 0, 255);
        b = std::clamp(b - other.b, 0, 255);
        a = std::clamp(a - other.a, 0, 255);
        return *this;
    }

    Color& Color::operator-=(const ColorMatrix& matrix)
    {
        *this = *this - matrix;
        return *this;
    }

    Color Color::operator*(double factor) const
    {
        return Color(
            std::clamp(r * factor, 0.0, 255.0),
            std::clamp(g * factor, 0.0, 255.0),
            std::clamp(b * factor, 0.0, 255.0),
            std::clamp(a * factor, 0.0, 255.0)
        );
    }

    Color Color::operator*(const Color& other) const
    {
        return Color(
            std::clamp((r * other.r) / 255, 0, 255),
            std::clamp((g * other.g) / 255, 0, 255),
            std::clamp((b * other.b) / 255, 0, 255),
            std::clamp((a * other.a) / 255, 0, 255)
        );
    }

    Color Color::operator*(const ColorMatrix& matrix) const
    {
        double newR, newG, newB, newA;
        this->Normalize(newR, newG, newB);
        newA = a / 255.0;

        double resultR = newR * matrix[0][0] + newG * matrix[0][1] + newB * matrix[0][2] + newA * matrix[0][3] + matrix[0][4];
        double resultG = newR * matrix[1][0] + newG * matrix[1][1] + newB * matrix[1][2] + newA * matrix[1][3] + matrix[1][4];
        double resultB = newR * matrix[2][0] + newG * matrix[2][1] + newB * matrix[2][2] + newA * matrix[2][3] + matrix[2][4];
        double resultA = newR * matrix[3][0] + newG * matrix[3][1] + newB * matrix[3][2] + newA * matrix[3][3] + matrix[3][4];

        return Color(
            static_cast<uint8_t>(std::clamp(resultR * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(resultG * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(resultB * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(resultA * 255.0, 0.0, 255.0))
        );
    }

    Color& Color::operator*=(double factor)
    {
        r = std::clamp(r * factor, 0.0, 255.0);
        g = std::clamp(g * factor, 0.0, 255.0);
        b = std::clamp(b * factor, 0.0, 255.0);
        a = std::clamp(a * factor, 0.0, 255.0);
        return *this;
    }

    Color& Color::operator*=(const Color& other)
    {
        r = std::clamp((r * other.r) / 255, 0, 255);
        g = std::clamp((g * other.g) / 255, 0, 255);
        b = std::clamp((b * other.b) / 255, 0, 255);
        a = std::clamp((a * other.a) / 255, 0, 255);
        return *this;
    }

    Color& Color::operator*=(const ColorMatrix& matrix)
    {
        double nr = r / 255.0;
        double ng = g / 255.0;
        double nb = b / 255.0;

        r = static_cast<uint8_t>(std::clamp((matrix[0][0]*nr + matrix[0][1]*ng + matrix[0][2]*nb + matrix[0][3]*a/255.0 + matrix[0][4]) * 255.0, 0.0, 255.0));
        g = static_cast<uint8_t>(std::clamp((matrix[1][0]*nr + matrix[1][1]*ng + matrix[1][2]*nb + matrix[1][3]*a/255.0 + matrix[1][4]) * 255.0, 0.0, 255.0));
        b = static_cast<uint8_t>(std::clamp((matrix[2][0]*nr + matrix[2][1]*ng + matrix[2][2]*nb + matrix[2][3]*a/255.0 + matrix[2][4]) * 255.0, 0.0, 255.0));
        a = static_cast<uint8_t>(std::clamp((matrix[3][0]*nr + matrix[3][1]*ng + matrix[3][2]*nb + matrix[3][3]*a/255.0 + matrix[3][4]) * 255.0, 0.0, 255.0));

        return *this;
    }

    Color Color::operator/(double factor) const
    {
        if (factor == 0) return *this;
        return Color(
            factor == 0 ? r : (r / factor),
            factor == 0 ? g : (g / factor),
            factor == 0 ? b : (b / factor),
            factor == 0 ? a : (a / factor)
        );
    }

    Color Color::operator/(const Color& other) const
    {
        return Color(
            other.r == 0 ? r : (r / other.r) * 255,
            other.g == 0 ? g : (g / other.g) * 255,
            other.b == 0 ? b : (b / other.b) * 255,
            other.a == 0 ? a : (a / other.a) * 255
        );
    }

    Color& Color::operator/=(double factor)
    {
        if (factor != 0)
        {
            r = factor == 0 ? r : (r / factor);
            g = factor == 0 ? g : (g / factor);
            b = factor == 0 ? b : (b / factor);
            a = factor == 0 ? a : (a / factor);
        }
        return *this;
    }

    Color& Color::operator/=(const Color& other)
    {
        r = other.r == 0 ? r : (r / other.r) * 255;
        g = other.g == 0 ? g : (g / other.g) * 255;
        b = other.b == 0 ? b : (b / other.b) * 255;
        a = other.a == 0 ? a : (a / other.a) * 255;
        return *this;
    }

    Color Color::operator&(const double& value) const
    {
        return *this * (1 - 0.5) + Color(value, value, value, 255) * 0.5;
    }

    Color Color::operator&(const Color& other) const
    {
        return *this * (1 - 0.5) + other * 0.5;
    }

    Color& Color::operator&=(const double& value)
    {
        *this = *this & value;
        return *this;
    }

    Color& Color::operator&=(const Color& other)
    {
        *this = *this & other;
        return *this;
    }
    #pragma endregion

    #pragma region Conversion Methods
    void Color::ToHex(std::string& outA, std::string& outR, std::string& outG, std::string& outB) const
    {
        char hexColor[9];
        snprintf(hexColor, sizeof(hexColor), "%02X%02X%02X%02X", a, r, g, b);
        outA = std::string(hexColor).substr(0, 2);
        outR = std::string(hexColor).substr(2, 2);
        outG = std::string(hexColor).substr(4, 2);
        outB = std::string(hexColor).substr(6, 2);
    }

    void Color::ToRGB(int& outR, int& outG, int& outB, int& outA) const
    {
        outR = r;
        outG = g;
        outB = b;
        outA = a;
    }

    // Linear sRGB Functions
    void Color::ToLinearSRGB(double& outR, double& outG, double& outB) const
    {
        this->Normalize(outR, outG, outB);
        auto linear_sRGB = [](double c) { return (c <= 0.04045) ? (c / 12.92) : std::pow((c + 0.055) / 1.055, 2.4); };

        outR = linear_sRGB(outR);
        outG = linear_sRGB(outG);
        outB = linear_sRGB(outB);
    }

    Color Color::FromLinearSRGB(double r, double g, double b, int a)
    {
        auto gammaCorrect = [](double c) { return (c > 0.0031308) ? (1.055 * std::pow(c, 1.0 / 2.4) - 0.055) : (12.92 * c); };
        r = gammaCorrect(r);
        g = gammaCorrect(g);
        b = gammaCorrect(b);

        return Color(
            std::clamp(r * 255.0, 0.0, 255.0),
            std::clamp(g * 255.0, 0.0, 255.0),
            std::clamp(b * 255.0, 0.0, 255.0),
            a
        );
    }

    // ProPhoto RGB Functions
    void Color::ToProPhotoRGB(double& outR, double& outG, double& outB) const
    {
        // Convert sRGB to XYZ D50
        this-> ToXYZ_D50(outR, outG, outB);

        // Convert XYZ D50 to linear ProPhoto RGB
        outR =  1.34578688164715830 * outR + -0.25557208737979464 * outG + -0.05110186497554526 * outB;
        outG = -0.54463070512490190 * outR +  1.50824774284514680 * outG +  0.02052744743642139 * outB;
        outB =  0.00000000000000000 * outR +  0.00000000000000000 * outG +  1.21196754563894520 * outB;

        // Apply ProPhoto RGB transfer function
        auto prophoto_tf = [](double v)
        {
            const double Et = 1.0 / 512.0;
            return (v < Et) ? v * 16.0 : std::pow(v, 1.0 / 1.8);
        };

        outR = prophoto_tf(outR);
        outG = prophoto_tf(outG);
        outB = prophoto_tf(outB);
    }

    Color Color::FromProPhotoRGB(double r, double g, double b, int a)
    {
        // Apply inverse ProPhoto RGB transfer function
        auto inverse_prophoto_tf = [](double v)
        {
            const double Et = 1.0 / 512.0;
            return (v < Et * 16.0) ? v / 16.0 : std::pow(v, 1.8);
        };

        r = inverse_prophoto_tf(r);
        g = inverse_prophoto_tf(g);
        b = inverse_prophoto_tf(b);

        // Convert linear ProPhoto RGB to XYZ D50 (and scale by 100)
        double x = (0.79776664490064230 * r + 0.13518129740053308 * g + 0.03134773412839220 * b) * 100;
        double y = (0.28807482881940130 * r + 0.71183523424187300 * g + 0.00008993693872564 * b) * 100;
        double z = (0.00000000000000000 * r + 0.00000000000000000 * g + 0.82510460251046020 * b) * 100;

        // Convert XYZ D50 to sRGB
        return FromXYZ_D50(x, y, z, a);
    }

    //Adobe RGB Functions
    void Color::ToAdobeRGB(double& outR, double& outG, double& outB) const
    {
        // First, convert to XYZ
        double x, y, z;
         ToXYZ_D65(x, y, z);

        x /= 100.0;
        y /= 100.0;
        z /= 100.0;

        // Then, convert XYZ to Adobe RGB
        outR = x * 2.04159 + y * -0.56501 + z * -0.34473;
        outG = x * -0.96924 + y * 1.87597 + z * 0.04156;
        outB = x * 0.01344 + y * -0.11836 + z * 1.01517;

        // Apply Adobe RGB gamma correction
        auto gamma_AdobeRGB = [](double c) { return c <= 0.0 ? 0.0 : std::pow(c, 1.0 / 2.19921875); };

        outR = gamma_AdobeRGB(outR);
        outG = gamma_AdobeRGB(outG);
        outB = gamma_AdobeRGB(outB);
    }

    Color Color::FromAdobeRGB(double r, double g, double b, int a)
    {
        // Apply inverse Adobe RGB gamma correction
        auto linear_AdobeRGB = [](double c) { return c <= 0.0 ? 0.0 : std::pow(c, 2.19921875); };

        r = linear_AdobeRGB(r);
        g = linear_AdobeRGB(g);
        b = linear_AdobeRGB(b);

        // Convert linear Adobe RGB to XYZ
        double x = (r * 0.57667 + g * 0.18556 + b * 0.18823) * 100;
        double y = (r * 0.29734 + g * 0.62736 + b * 0.07529) * 100;
        double z = (r * 0.02703 + g * 0.07069 + b * 0.99134) * 100;

        // Convert XYZ to sRGB
        return  FromXYZ_D65(x, y, z, a);
    }

    //HSL Functions
    void Color::ToHSL(double& h, double& s, double& l) const
    {
        double r = this->r / 255.0;
        double g = this->g / 255.0;
        double b = this->b / 255.0;

        double max = std::max({r, g, b});
        double min = std::min({r, g, b});
        double chroma = max - min;

        l = (max + min) / 2;
        s = (l == 0 || l == 1) ? 0 : chroma / (1 - std::abs(2 * l - 1));

        if (chroma == 0)
            h = 0;
        else if (max == r)
            h = (g - b) / chroma;
        else if (max == g)
            h = 2 + (b - r) / chroma;
        else
            h = 4 + (r - g) / chroma;

        h *= 60;
        if (h < 0) h += 360;

        s *= 100;
        l *= 100;
    }

    Color Color::FromHSL(double h, double s, double l, int a)
    {
        h = std::fmod(h, 360.0) / 60.0;
        s /= 100.0;
        l /= 100.0;

        double c = (1 - std::abs(2 * l - 1)) * s;
        double x = c * (1 - std::abs(std::fmod(h, 2.0) - 1));
        double m = l - c / 2;

        const int hi = static_cast<int>(h);
        const double rgb[6][3] = {
            {c, x, 0}, // h < 1
            {x, c, 0}, // h < 2
            {0, c, x}, // h < 3
            {0, x, c}, // h < 4
            {x, 0, c}, // h < 5
            {c, 0, x}  // h >= 5
        };

        return Color(
            static_cast<uint8_t>((rgb[hi][0] + m) * 255),
            static_cast<uint8_t>((rgb[hi][1] + m) * 255),
            static_cast<uint8_t>((rgb[hi][2] + m) * 255),
            a
        );
    }

    //HSV Functions
    void Color::ToHSV(double& h, double& s, double& v) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        double max = std::max({nR, nG, nB});
        double min = std::min({nR, nG, nB});
        double chroma = max - min;

        v = max;
        s = (max == 0) ? 0 : chroma / max;

        if (chroma == 0)
            h = 0;
        else if (max == nR)
            h = (nG - nB) / chroma;
        else if (max == nG)
            h = 2 + (nB - nR) / chroma;
        else
            h = 4 + (nR - nG) / chroma;

        h *= 60;
        if (h < 0) h += 360;

        s *= 100;
        v *= 100;
    }

    Color Color::FromHSV(double h, double s, double v, int a)
    {
        h = std::fmod(h, 360.0) / 60.0;
        s /= 100.0;
        v /= 100.0;

        double c = v * s;
        double x = c * (1 - std::abs(std::fmod(h, 2.0) - 1));
        double m = v - c;

        double r, g, b;
        if (h < 1) { r = c; g = x; b = 0; }
        else if (h < 2) { r = x; g = c; b = 0; }
        else if (h < 3) { r = 0; g = c; b = x; }
        else if (h < 4) { r = 0; g = x; b = c; }
        else if (h < 5) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        return Color(
            static_cast<uint8_t>((r + m) * 255),
            static_cast<uint8_t>((g + m) * 255),
            static_cast<uint8_t>((b + m) * 255),
            a
        );
    }

    //HSI Functions
    void Color::ToHSI(double& h, double& s, double& i) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        // Intensity: 0-255 range
        i = (r + g + b) / 3.0;

        if (i > 0) {
            double m = std::min({nR, nG, nB});
            s = 1.0 - (m / ((nR + nG + nB) / 3.0));
        } else {
            s = 0;
        }

        double denominator = std::sqrt(nR*nR + nG*nG + nB*nB - nR*nG - nR*nB - nG*nB);
        if (denominator != 0) {
            double numerator = nR - 0.5*nG - 0.5*nB;
            h = std::acos(numerator / denominator) * 180.0 / CONST_PI;
            if (nB > nG) {
                h = 360.0 - h;
            }
        } else {
            h = 0;
        }

        // Scale saturation to 0-100 range
        s *= 100;
    }

    Color Color::FromHSI(double h, double s, double i, int a)
    {
        // Convert saturation back to 0-1 range
        s /= 100.0;

        double r, g, b;

        if (h == 0) {
            r = i + 2*i*s;
            g = i - i*s;
            b = i - i*s;
        }
        else if (h < 120) {
            double radH = h * CONST_PI / 180.0;
            r = i + i*s*std::cos(radH)/std::cos(CONST_PI/3 - radH);
            g = i + i*s*(1 - std::cos(radH)/std::cos(CONST_PI/3 - radH));
            b = i - i*s;
        }
        else if (h == 120) {
            r = i - i*s;
            g = i + 2*i*s;
            b = i - i*s;
        }
        else if (h < 240) {
            double radH = h * CONST_PI / 180.0;
            r = i - i*s;
            g = i + i*s*std::cos(radH - 2*CONST_PI/3)/std::cos(CONST_PI - radH);
            b = i + i*s*(1 - std::cos(radH - 2*CONST_PI/3)/std::cos(CONST_PI - radH));
        }
        else if (h == 240) {
            r = i - i*s;
            g = i - i*s;
            b = i + 2*i*s;
        }
        else { // 240 < h < 360
            double radH = h * CONST_PI / 180.0;
            r = i + i*s*(1 - std::cos(radH - 4*CONST_PI/3)/std::cos(5*CONST_PI/3 - radH));
            g = i - i*s;
            b = i + i*s*std::cos(radH - 4*CONST_PI/3)/std::cos(5*CONST_PI/3 - radH);
        }

        return Color(
            static_cast<int>(std::clamp(r, 0.0, 255.0)),
            static_cast<int>(std::clamp(g, 0.0, 255.0)),
            static_cast<int>(std::clamp(b, 0.0, 255.0)),
            a
        );
    }

    // HWB Functions
    void Color::ToHWB(double& h, double& w, double& b) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        double max = std::max({nR, nG, nB});
        double min = std::min({nR, nG, nB});
        double chroma = max - min;

        w = min;
        b = 1 - max;

        if (chroma == 0)
            h = 0;
        else if (max == nR)
            h = (nG - nB) / chroma;
        else if (max == nG)
            h = 2 + (nB - nR) / chroma;
        else
            h = 4 + (nR - nG) / chroma;

        h *= 60;
        if (h < 0) h += 360;

        w *= 100;
        b *= 100;
    }

    Color Color::FromHWB(double h, double w, double b, int a)
    {
        h = std::fmod(h, 360.0) / 60.0;
        w /= 100.0;
        b /= 100.0;

        double v = 1 - b;
        double s = v == 0 ? 0 : 1 - w / v;
        double c = v * s;
        double x = c * (1 - std::abs(std::fmod(h, 2.0) - 1));
        double m = v - c;

        double r, g, b_;
        if (h < 1) { r = c; g = x; b_ = 0; }
        else if (h < 2) { r = x; g = c; b_ = 0; }
        else if (h < 3) { r = 0; g = c; b_ = x; }
        else if (h < 4) { r = 0; g = x; b_ = c; }
        else if (h < 5) { r = x; g = 0; b_ = c; }
        else { r = c; g = 0; b_ = x; }

        return Color(
            static_cast<uint8_t>((r + m) * 255),
            static_cast<uint8_t>((g + m) * 255),
            static_cast<uint8_t>((b_ + m) * 255),
            a
        );
    }

    //HSP Functions
    void Color::ToHSP(double& h, double& s, double& p) const
    {
        const double Pr = 0.299;
        const double Pg = 0.587;
        const double Pb = 0.114;

        double r, g, b;
        Normalize(r, g, b);

        p = std::sqrt(r * r * Pr + g * g * Pg + b * b * Pb) * 255.0;

        if (r == g && g == b)
        {
            h = 0.0;
            s = 0.0;
            return;
        }

        if (r >= g && g >= b) h = 60.0 * (g - b) / (r - b);
        else if (g > r && r >= b) h = 60.0 * (2.0 - (r - b) / (g - b));
        else if (g >= b && b > r) h = 60.0 * (2.0 + (b - r) / (g - r));
        else if (b > g && g > r) h = 60.0 * (4.0 - (g - r) / (b - r));
        else if (b > r && r >= g) h = 60.0 * (4.0 + (r - g) / (b - g));
        else h = 60.0 * (6.0 - (b - g) / (r - g));

        double min = std::min(std::min(r, g), b);
        double max = std::max(std::max(r, g), b);
        s = (max - min) / max * 100.0;
    }

    Color Color::FromHSP(double h, double s, double p, int a)
    {
        h = std::fmod(h, 360.0);
        if (h < 0) h += 360.0;
        s = std::clamp(s, 0.0, 100.0) / 100.0;
        p = std::clamp(p, 0.0, 255.0) / 255.0;

        double r, g, b;
        if (s == 0.0)
        {
            r = g = b = p;
        }
        else
        {
            double sector = h / 60.0;
            int i = static_cast<int>(sector);
            double f = sector - i;

            double vs = s * p;
            double vsf = vs * f;
            double mid1 = p - vs + vsf;
            double mid2 = p - vsf;

            switch (i)
            {
                case 0: r = p;      g = mid1;   b = p - vs; break;
                case 1: r = mid2;   g = p;      b = p - vs; break;
                case 2: r = p - vs; g = p;      b = mid1;   break;
                case 3: r = p - vs; g = mid2;   b = p;      break;
                case 4: r = mid1;   g = p - vs; b = p;      break;
                case 5: r = p;      g = p - vs; b = mid2;   break;
            }
        }

        return Color(
            static_cast<uint8_t>(std::clamp(r * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(g * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(b * 255.0, 0.0, 255.0)),
            a
        );
    }

    //HCY Functions
    void Color::ToHCY(double& h, double& c, double& y) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        double sum = nR + nG + nB;
        if (sum == 0)
        {
            h = c = y = 0;
            return;
        }

        // Normalize RGB values by their sum
        nR /= sum;
        nG /= sum;
        nB /= sum;

        // Calculate hue using arccos formula
        h = std::acos(
            (0.5 * ((nR - nG) + (nR - nB))) /
            std::sqrt((nR - nG)*(nR - nG) + (nR - nB)*(nG - nB))
        );

        if (nB > nG) h = 2 * CONST_PI - h;

        // Convert radians to degrees
        h = h * 180.0 / CONST_PI;

        // Calculate chroma/saturation
        c = (1.0 - 3.0 * std::min({nR, nG, nB})) * 100.0;

        // Calculate intensity/luma
        y = sum * 255.0 / 3.0;
    }

    Color Color::FromHCY(double h, double c, double y, int a)
    {
        // Convert degrees to radians
        double hRad = std::fmod(h + 360.0, 360.0) * CONST_PI / 180.0;

        double s = std::clamp(c, 0.0, 100.0) / 100.0;
        double i = std::clamp(y, 0.0, 255.0) / 255.0;

        const double pi3 = CONST_PI / 3.0;
        double r, g, b;

        if (hRad < (2.0 * pi3))
        {
            b = i * (1.0 - s);
            r = i * (1.0 + (s * std::cos(hRad) / std::cos(pi3 - hRad)));
            g = i * (1.0 + (s * (1.0 - std::cos(hRad) / std::cos(pi3 - hRad))));
        }
        else if (hRad < (4.0 * pi3))
        {
            hRad -= 2.0 * pi3;
            r = i * (1.0 - s);
            g = i * (1.0 + (s * std::cos(hRad) / std::cos(pi3 - hRad)));
            b = i * (1.0 + (s * (1.0 - std::cos(hRad) / std::cos(pi3 - hRad))));
        }
        else
        {
            hRad -= 4.0 * pi3;
            g = i * (1.0 - s);
            b = i * (1.0 + (s * std::cos(hRad) / std::cos(pi3 - hRad)));
            r = i * (1.0 + (s * (1.0 - std::cos(hRad) / std::cos(pi3 - hRad))));
        }

        return Color(
            static_cast<uint8_t>(r * 255),
            static_cast<uint8_t>(g * 255),
            static_cast<uint8_t>(b * 255),
            a
        );
    }

    //HCG Functions
    void Color::ToHCG(double& h, double& c, double& g) const
    {
        double r, g_, b;
        Normalize(r, g_, b);

        double max = std::max({r, g_, b});
        double min = std::min({r, g_, b});
        double chroma = max - min;

        g = 0;
        if (chroma < 1) g = min / (1 - chroma) * 100;

        c = chroma * 100;

        if (chroma > 0)
        {
            if (max == r) h = std::fmod((g_ - b) / chroma, 6.0);
            else if (max == g_) h = 2.0 + (b - r) / chroma;
            else h = 4.0 + (r - g_) / chroma;

            h *= 60;
            h = std::fmod(h + 360, 360.0);
        }
        else
        {
            h = 0;
        }
    }

    Color Color::FromHCG(double h, double c, double g, int a)
    {
        h = std::fmod(h, 360.0) / 360.0;
        c = std::clamp(c, 0.0, 100.0) / 100.0;
        g = std::clamp(g, 0.0, 100.0) / 100.0;

        if (c == 0.0) return Color(g * 255, g * 255, g * 255, a);

        double hi = std::fmod(h, 1.0) * 6.0;
        double v = std::fmod(hi, 1.0);
        double w = 1.0 - v;

        double pure[3] = {0, 0, 0};
        switch (static_cast<int>(hi))
        {
            case 0: pure[0] = 1; pure[1] = v; pure[2] = 0; break;
            case 1: pure[0] = w; pure[1] = 1; pure[2] = 0; break;
            case 2: pure[0] = 0; pure[1] = 1; pure[2] = v; break;
            case 3: pure[0] = 0; pure[1] = w; pure[2] = 1; break;
            case 4: pure[0] = v; pure[1] = 0; pure[2] = 1; break;
            default: pure[0] = 1; pure[1] = 0; pure[2] = w;
        }

        double mg = (1.0 - c) * g;
        return Color(
            (c * pure[0] + mg) * 255,
            (c * pure[1] + mg) * 255,
            (c * pure[2] + mg) * 255,
            a
        );
    }

    // TSL (Tint, Saturation, Lightness) Functions
    void Color::ToTSL(double& t, double& s, double& l) const
    {
        double r = this->r;
        double g = this->g;
        double b = this->b;

        double sum = r + g + b;
        double r_ = (r / sum - 1.0/3.0);
        double g_ = (g / sum - 1.0/3.0);

        t = g_ != 0 ? 0.5 - std::atan2(g_, r_) / (2.0 * CONST_PI) : 0;
        s = std::sqrt(9.0/5.0 * (r_*r_ + g_*g_));
        l = (0.299*r + 0.587*g + 0.114*b) / 255.0;
    }

    Color Color::FromTSL(double t, double s, double l, int a)
    {
        double x = std::tan(2.0 * CONST_PI * (t - 0.25));
        x *= x;

        double r = std::sqrt(5.0 * s*s / (9.0 * (1.0/x + 1.0))) + 1.0/3.0;
        double g = std::sqrt(5.0 * s*s / (9.0 * (x + 1.0))) + 1.0/3.0;
        double k = l / (0.185 * r + 0.473 * g + 0.114);

        double B = k * (1.0 - r - g);
        double G = k * g;
        double R = k * r;

        return Color(
            static_cast<uint8_t>(std::clamp(R * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(G * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(B * 255.0, 0.0, 255.0)),
            a
        );
    }

    // CMY Functions
    void Color::ToCMY(double& c, double& m, double& y) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        c = (1.0 - nR) * 100.0;
        m = (1.0 - nG) * 100.0;
        y = (1.0 - nB) * 100.0;
    }

    Color Color::FromCMY(double c, double m, double y, int a)
    {
        c = std::clamp(c, 0.0, 100.0) / 100.0;
        m = std::clamp(m, 0.0, 100.0) / 100.0;
        y = std::clamp(y, 0.0, 100.0) / 100.0;

        return Color(
            (1.0 - c) * 255,
            (1.0 - m) * 255,
            (1.0 - y) * 255,
            a
        );
    }

    // CMYK Functions
    void Color::ToCMYK(double& c, double& m, double& y, double& k) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        k = 1 - std::max({nR, nG, nB});
        if (k == 1)
        {
            c = m = y = 0;
        }
        else
        {
            c = (1 - nR - k) / (1 - k);
            m = (1 - nG - k) / (1 - k);
            y = (1 - nB - k) / (1 - k);
        }

        c *= 100;
        m *= 100;
        y *= 100;
        k *= 100;
    }

    Color Color::FromCMYK(double c, double m, double y, double k, int a)
    {
        c = std::clamp(c, 0.0, 100.0) / 100.0;
        m = std::clamp(m, 0.0, 100.0) / 100.0;
        y = std::clamp(y, 0.0, 100.0) / 100.0;
        k = std::clamp(k, 0.0, 100.0) / 100.0;

        double r = 255 * (1 - c) * (1 - k);
        double g = 255 * (1 - m) * (1 - k);
        double b = 255 * (1 - y) * (1 - k);

        return Color(r, g, b, a);
    }

    // XYZ Functions
    void Color::ToXYZ_D50(double& x, double& y, double& z) const
    {
        // Convert sRGB to linear sRGB
        double newR, newG, newB;
        this->ToLinearSRGB(newR, newG, newB);

        // Convert linear sRGB to XYZ D50
        x = (0.4360747 * newR + 0.3850649 * newG + 0.1430804 * newB);
        y = (0.2225045 * newR + 0.7168786 * newG + 0.0606169 * newB);
        z = (0.0139322 * newR + 0.0971045 * newG + 0.7141733 * newB);
    }

    Color Color::FromXYZ_D50(double x, double y, double z, int a)
    {
        // Convert XYZ D50 to linear sRGB
        double r =  3.1338561 * x - 1.6168667 * y - 0.4906146 * z;
        double g = -0.9787684 * x + 1.9161415 * y + 0.0334540 * z;
        double b =  0.0719453 * x - 0.2289914 * y + 1.4052427 * z;

        return FromLinearSRGB(r, g, b, a);
    }

    void Color::ToXYZ_D65(double& x, double& y, double& z) const
    {
        double nR, nG, nB;
        ToLinearSRGB(nR, nG, nB);

        // Convert to XYZ
        x = (nR * 0.4124564 + nG * 0.3575761 + nB * 0.1804375) * 100;
        y = (nR * 0.2126729 + nG * 0.7151522 + nB * 0.0721750) * 100;
        z = (nR * 0.0193339 + nG * 0.1191920 + nB * 0.9503041) * 100;
    }

    Color Color::FromXYZ_D65(double X, double Y, double Z, int a)
    {
        X /= 100;
        Y /= 100;
        Z /= 100;

        // Apply the transformation to Linear sRGB
        double R = 3.2406255 * X + -1.5372080 * Y + -0.4986286 * Z;
        double G = -0.9689307 * X + 1.875761 * Y + 0.0415175 * Z;
        double B = 0.0557101 * X + -0.2040211 * Y + 1.0569959 * Z;

        return Color::FromLinearSRGB(R, G, B, a);
    }

    // UCS Color Space (Obsolete)
    void Color::ToUCS(double& u, double& v, double& w) const
    {
        double x, y, z;
        ToXYZ_D65(x, y, z);

        u = x * 2.0/3.0;
        v = y;
        w = 0.5 * (-x + 3.0*y + z);
    }

    Color Color::FromUCS(double u, double v, double w, int a)
    {
        double x = 1.5 * u;
        double y = v;
        double z = 1.5 * u - 3.0 * v + 2.0 * w;

        return FromXYZ_D65(x, y, z, a);
    }

    // UVW Functions
    void Color::ToUVW(double& u, double& v, double& w) const
    {
        double x, y, z;
        ToXYZ_D65(x, y, z);

        // D65 whitepoint values
        const double xn = 95.047;
        const double yn = 100.000;
        const double zn = 108.883;

        const double un = (4.0 * xn) / (xn + 15.0 * yn + 3.0 * zn);
        const double vn = (6.0 * yn) / (xn + 15.0 * yn + 3.0 * zn);

        double _u = 4.0 * x / (x + 15.0 * y + 3.0 * z);
        double _v = 6.0 * y / (x + 15.0 * y + 3.0 * z);

        w = 25.0 * std::pow(y, 1.0/3.0) - 17.0;
        u = 13.0 * w * (_u - un);
        v = 13.0 * w * (_v - vn);
    }

    Color Color::FromUVW(double u, double v, double w, int a)
    {
        if (w == 0) return Color(0, 0, 0, a);

        // D65 whitepoint values
        const double xn = 95.047;
        const double yn = 100.000;
        const double zn = 108.883;

        const double un = (4.0 * xn) / (xn + 15.0 * yn + 3.0 * zn);
        const double vn = (6.0 * yn) / (xn + 15.0 * yn + 3.0 * zn);

        double y = std::pow((w + 17.0) / 25.0, 3.0);

        double _u = u / (13.0 * w) + un;
        double _v = v / (13.0 * w) + vn;

        double x = (6.0 / 4.0) * y * _u / _v;
        double z = y * (2.0 / _v - 0.5 * _u / _v - 5.0);

        return FromXYZ_D65(x, y, z, a);
    }

    // XYY Functions
    void Color::ToXYY(double& x, double& y, double& Y) const
    {
        double X, Z;
        ToXYZ_D65(X, Y, Z);

        double sum = X + Y + Z;
        if (sum == 0)
        {
            x = 0;
            y = 0;
            return;
        }

        x = X / sum;
        y = Y / sum;
    }

    Color Color::FromXYY(double x, double y, double Y, int a)
    {
        double X, Z;

        if (y == 0)
        {
            X = Z = 0;
        }
        else
        {
            X = x * Y / y;
            Z = (1 - x - y) * Y / y;
        }

        return FromXYZ_D65(X, Y, Z, a);
    }

    // CIE Lab Methods
    void Color::ToLab(double& l, double& a, double& b) const
    {
        double x, y, z;
         ToXYZ_D65(x, y, z);

        // D65 reference white point
        const double xn = 95.047;
        const double yn = 100.000;
        const double zn = 108.883;

        auto f = [](double t) {
            return t > 0.008856 ? std::pow(t, 1.0/3.0) : (903.3 * t + 16) / 116;
        };

        double fx = f(x / xn);
        double fy = f(y / yn);
        double fz = f(z / zn);

        l = 116.0 * fy - 16.0;
        a = 500.0 * (fx - fy);
        b = 200.0 * (fy - fz);
    }

    Color Color::FromLab(double l, double a, double b, int alpha)
    {
        // D65 reference white point
        const double xn = 0.95047;
        const double yn = 1.00000;
        const double zn = 1.08883;

        auto f_inv = [](double t) {
            return t > 0.206893 ? t * t * t : (t - 16.0 / 116.0) / 7.787;
        };

        double fy = (l + 16.0) / 116.0;
        double fx = a / 500.0 + fy;
        double fz = fy - b / 200.0;

        double x = xn * f_inv(fx);
        double y = yn * f_inv(fy);
        double z = zn * f_inv(fz);

        return  FromXYZ_D65(x, y, z, alpha);
    }

    // CIE Luv
    void Color::ToLuv(double& L, double& u, double& v) const
    {
        double X, Y, Z;
         ToXYZ_D65(X, Y, Z);

        X /= 100;
        Y /= 100;
        Z /= 100;

        const double yn = 1.0;
        const double un = 0.19793943;
        const double vn = 0.46831096;

        double u_prime = (4 * X) / (X + 15 * Y + 3 * Z);
        double v_prime = (9 * Y) / (X + 15 * Y + 3 * Z);

        L = Y > 0.008856 ? 116 * std::pow(Y / yn, 1.0 / 3.0) - 16 : 903.3 * Y / yn;
        u = 13 * L * (u_prime - un);
        v = 13 * L * (v_prime - vn);
    }

    Color Color::FromLuv(double L, double u, double v, int a)
    {
        const double yn = 1.0;
        const double un = 0.19793943;
        const double vn = 0.46831096;

        double u_prime = u / (13 * L) + un;
        double v_prime = v / (13 * L) + vn;

        double Y = L > 8 ? yn * std::pow((L + 16) / 116, 3) : yn * L / 903.3;
        double X = Y * 9 * u_prime / (4 * v_prime);
        double Z = Y * (12 - 3 * u_prime - 20 * v_prime) / (4 * v_prime);

        return  FromXYZ_D65(X, Y, Z, a);
    }

    // YPbPr Functions
    void Color::ToYPbPr(double& y, double& pb, double& pr) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        y  = .2126 * nR + .7152 * nG + .0722 * nB;
        pb = (nB - y) / 1.8814;
        pr = (nR - y) / 1.4746;
    }

    Color Color::FromYPbPr(double y, double pb, double pr, int a)
    {
        double r = std::clamp(y + 0.0000 * pb + 1.4022 * pr, 0.0, 1.0);
        double g = std::clamp(y - 0.3456 * pb - 0.7145 * pr, 0.0, 1.0);
        double b = std::clamp(y + 1.7710 * pb + 0.0000 * pr, 0.0, 1.0);

        return Color(
            static_cast<uint8_t>(std::clamp(r * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(g * 255.0, 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(b * 255.0, 0.0, 255.0)),
            a
        );
    }

    // YCbCr Functions
    void Color::ToYCbCr(double& y, double& cb, double& cr, YCbCrType type) const
    {
        double r = this->r;
        double g = this->g;
        double b = this->b;

        switch(type)
        {
            case YCbCrType::BT601:
                y = 0.299 * r + 0.587 * g + 0.114 * b;
                cb = (b - y) / 1.772 + 128;
                cr = (r - y) / 1.402 + 128;
                break;

            case YCbCrType::BT709:
                y = 0.2126 * r + 0.7152 * g + 0.0722 * b;
                cb = (b - y) / 1.8556 + 128;
                cr = (r - y) / 1.5748 + 128;
                break;

            case YCbCrType::BT2020:
                y = 0.2627 * r + 0.6780 * g + 0.0593 * b;
                cb = (b - y) / 1.8814 + 128;
                cr = (r - y) / 1.4746 + 128;
                break;
        }
    }

    Color Color::FromYCbCr(double y, double cb, double cr, int a, YCbCrType type)
    {
        double r, g, b;

        switch(type) {
            case YCbCrType::BT601:
                cb -= 128;
                cr -= 128;
                r = y + 1.402 * cr;
                g = y - (0.299 * 1.402 / 0.587) * cr - (0.114 * 1.772 / 0.587) * cb;
                b = y + 1.772 * cb;
                break;

            case YCbCrType::BT709:
                cb -= 128;
                cr -= 128;
                r = y + 1.5748 * cr;
                g = y - (0.2126 * 1.5748 / 0.7152) * cr - (0.0722 * 1.8556 / 0.7152) * cb;
                b = y + 1.8556 * cb;
                break;

            case YCbCrType::BT2020:
                cb -= 128;
                cr -= 128;
                r = y + 1.4746 * cr;
                g = y - (0.2627 * 1.4746 / 0.6780) * cr - (0.0593 * 1.8814 / 0.6780) * cb;
                b = y + 1.8814 * cb;
                break;
        }

        return Color(
            std::clamp(r, 0.0, 255.0),
            std::clamp(g, 0.0, 255.0),
            std::clamp(b, 0.0, 255.0),
            a
        );
    }

    // YCgCo Functions
    void Color::ToYCgCo(double& y, double& cg, double& co) const
    {
        double r, g, b;
        Normalize(r, g, b);

        y = 0.25 * r + 0.5 * g + 0.25 * b;
        cg = -0.25 * r + 0.5 * g - 0.25 * b;
        co = 0.5 * r - 0.5 * b;
    }

    Color Color::FromYCgCo(double y, double cg, double co, int a)
    {
        double tmp = y - cg;

        return Color(
            (tmp + co) * 255,
            (y + cg) * 255,
            (tmp - co) * 255,
            a
        );
    }

    // YDbDr Functions
    void Color::ToYDbDr(double& y, double& db, double& dr) const
    {
        double r, g, b;
        Normalize(r, g, b);

        y = 0.299 * r + 0.587 * g + 0.114 * b;
        db = -0.450 * r - 0.883 * g + 1.333 * b;
        dr = -1.333 * r + 1.116 * g + 0.217 * b;
    }

    Color Color::FromYDbDr(double y, double db, double dr, int a)
    {
        double r = y + 0.000092303716148 * db - 0.525912630661865 * dr;
        double g = y - 0.129132898890509 * db + 0.267899328207599 * dr;
        double b = y + 0.664679059978955 * db - 0.000079202543533 * dr;

        return Color(
            r * 255,
            g * 255,
            b * 255,
            a
        );
    }

    // YcCbcCrc Functions
    void Color::ToYcCbcCrc(double& y, double& cb, double& cr) const
    {
        const double Kr = 0.2627;
        const double Kb = 0.0593;
        double r, g, b;
        Normalize(r, g, b);

        y = Kr * r + (1 - Kr - Kb) * g + Kb * b;
        cb = (b - y) / (2 * (1 - Kb));
        cr = (r - y) / (2 * (1 - Kr));
    }

    Color Color::FromYcCbcCrc(double y, double cb, double cr, int a)
    {
        const double Kr = 0.2627;
        const double Kb = 0.0593;

        double r = y + 2 * (1 - Kr) * cr;
        double b = y + 2 * (1 - Kb) * cb;
        double g = (y - Kr * r - Kb * b) / (1 - Kr - Kb);

        return Color(
            r * 255,
            g * 255,
            b * 255,
            a
        );
    }

    // YUV Functions
    void Color::ToYUV(double& y, double& u, double& v) const
    {
        double r, g, b;
        Normalize(r, g, b);

        y = (r * 0.299) + (g * 0.587) + (b * 0.114);
        u = (r * -0.14713) + (g * -0.28886) + (b * 0.436);
        v = (r * 0.615) + (g * -0.51499) + (b * -0.10001);
    }

    Color Color::FromYUV(double y, double u, double v, int a)
    {
        double r = (y * 1.0) + (u * 0.0) + (v * 1.13983);
        double g = (y * 1.0) + (u * -0.39465) + (v * -0.58060);
        double b = (y * 1.0) + (u * 2.02311) + (v * 0.0);

        r = std::clamp(r, 0.0, 1.0);
        g = std::clamp(g, 0.0, 1.0);
        b = std::clamp(b, 0.0, 1.0);

        return Color(
            r * 255,
            g * 255,
            b * 255,
            a
        );
    }

    // YES Functions
    void Color::ToYES(double& y, double& e, double& s) const
    {
        double r, g, b;
        Normalize(r, g, b);

        const double m[9] = {
            0.253, 0.684, 0.063,
            0.500, -0.50, 0.0,
            0.250, 0.250, -0.5
        };

        y = r * m[0] + g * m[1] + b * m[2];
        e = r * m[3] + g * m[4] + b * m[5];
        s = r * m[6] + g * m[7] + b * m[8];
    }

    Color Color::FromYES(double y, double e, double s, int a)
    {
        const double m[9] = {
            1.0, 1.431, 0.126,
            1.0, -0.569, 0.126,
            1.0, 0.431, -1.874
        };

        double r = y * m[0] + e * m[1] + s * m[2];
        double g = y * m[3] + e * m[4] + s * m[5];
        double b = y * m[6] + e * m[7] + s * m[8];

        return Color(
            r * 255,
            g * 255,
            b * 255,
            a
        );
    }

    // JPEG Functions
    void Color::ToJPEG(double& y, double& cb, double& cr) const
    {
        y = 0.299 * r + 0.587 * g + 0.114 * b;
        cb = 128 - 0.168736 * r - 0.331264 * g + 0.5 * b;
        cr = 128 + 0.5 * r - 0.418688 * g - 0.081312 * b;
    }

    Color Color::FromJPEG(double y, double cb, double cr, int a)
    {
        return Color(
            static_cast<uint8_t>(std::clamp(y + 1.402 * (cr - 128), 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128), 0.0, 255.0)),
            static_cast<uint8_t>(std::clamp(y + 1.772 * (cb - 128), 0.0, 255.0)),
            a
        );
    }

    // LCHab Functions
    void Color::ToLCHab(double& l, double& c, double& h) const
    {
        double lab_l, lab_a, lab_b;
        ToLab(lab_l, lab_a, lab_b);

        l = lab_l;
        c = std::sqrt((lab_a * lab_a) + (lab_b * lab_b));
        h = std::atan2(lab_b, lab_a) * (180.0 / CONST_PI);
        if (h < 0) h += 360.0;
    }

    Color Color::FromLCHab(double l, double c, double h, int a)
    {
        double lab_a = c * std::cos(h * (CONST_PI / 180.0));
        double lab_b = c * std::sin(h * (CONST_PI / 180.0));

        return FromLab(l, lab_a, lab_b, a);
    }

    // LCHuv Functions
    void Color::ToLCHuv(double &l, double& c, double &h) const
    {
        double L, u, v;
        ToLuv(L, u, v);

        l = L;
        c = std::sqrt(u*u + v*v);
        h = std::atan2(v, u) * (180.0 / CONST_PI); // Radians to degrees
        h = h >= 0 ? h : h + 360; // Between 0-360
    }

    Color Color::FromLCHuv(double l, double c, double h, int a)
    {
        double u, v;

        double h_rad = h * (CONST_PI / 180);

        u = c * std::cos(h_rad);
        v = c * std::sin(h_rad);

        return FromLuv(l, u, v, a);
    }

    // YIQ Functions
    void Color::ToYIQ(double& y, double& i, double& q) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        y = 0.299 * nR + 0.587 * nG + 0.114 * nB;
        i = 0.596 * nR - 0.274 * nG - 0.322 * nB;
        q = 0.211 * nR - 0.523 * nG + 0.312 * nB;
    }

    Color Color::FromYIQ(double y, double i, double q, int a)
    {
        double r = y + 0.956 * i + 0.621 * q;
        double g = y - 0.272 * i - 0.647 * q;
        double b = y - 1.106 * i + 1.703 * q;

        r = std::clamp(r, 0.0, 1.0) * 255;
        g = std::clamp(g, 0.0, 1.0) * 255;
        b = std::clamp(b, 0.0, 1.0) * 255;

        return Color(r, g, b, a);
    }

    // NCol Functions
    void Color::ToNCol(std::string& n, double& c, double& l) const
    {
        double h, w, b;
        ToHWB(h, w, b);

        const char* hueNames = "RYGCBM";
        int hueIndex = static_cast<int>(h / 60) % 6;
        double huePercent = std::fmod(h, 60) / 60 * 100;

        n = std::string(1, hueNames[hueIndex]) + std::to_string(static_cast<int>(huePercent));
        c = w;
        l = (100 - b);
    }

    Color Color::FromNCol(const std::string& n, double c, double l, int a)
    {
        const char* hueNames = "RYGCBM";
        char hueChar = n[0];
        int hueIndex = std::strchr(hueNames, hueChar) - hueNames;
        double huePercent = std::stod(n.substr(1));

        double h = std::fmod(hueIndex * 60 + huePercent * 0.6, 360);
        double w = std::clamp(c, 0.0, 100.0);
        double b = std::clamp(l, 0.0, 100.0);

        return FromHWB(h, w, b, a);
    }

    // Rec.2020 Functions
    void Color::ToRec2020(double& outR, double& outG, double& outB) const
    {
        double x, y, z;
        ToXYZ_D65(x, y, z);
        x /= 100.0; y /= 100.0; z /= 100.0;

        outR = 1.7166511879712674 * x - 0.3556707837763924 * y - 0.2533662813736599 * z;
        outG = -0.6666843518324892 * x + 1.6164812366349395 * y + 0.0157685458139111 * z;
        outB = 0.0176398574453108 * x - 0.0427706133302187 * y + 0.9421031212179456 * z;

        // Apply Rec.2020 transfer function
        auto gamma = [](double x) {
            return x < 0.018053968510807 ? 4.5 * x : 1.09929682680944 * std::pow(x, 0.45) - 0.09929682680944;
        };
        outR = gamma(outR);
        outG = gamma(outG);
        outB = gamma(outB);
    }

    Color Color::FromRec2020(double r, double g, double b, int a)
    {
        // Inverse Rec.2020 transfer function
        auto invGamma = [](double x) {
            return x < 0.08145 ? x / 4.5 : std::pow((x + 0.09929682680944) / 1.09929682680944, 1.0 / 0.45);
        };
        r = invGamma(r);
        g = invGamma(g);
        b = invGamma(b);

        // Convert to XYZ
        double x = 0.6369580483012914 * r + 0.1446169035862083 * g + 0.1688809751641721 * b;
        double y = 0.2627002120112671 * r + 0.6779980715188708 * g + 0.0593017164698621 * b;
        double z = 0.0000000000000000 * r + 0.0280726930490874 * g + 1.0609850577107972 * b;

        return FromXYZ_D65(x * 100, y * 100, z * 100, a);
    }

    // Display P3 Functions
    void Color::ToDisplayP3(double& outR, double& outG, double& outB) const
    {
        double x, y, z;
        ToXYZ_D65(x, y, z);
        x /= 100.0; y /= 100.0; z /= 100.0;

        outR = 2.4934969119414254 * x - 0.9313836179191239 * y - 0.4027107844507168 * z;
        outG = -0.8294889695615747 * x + 1.7626640603183463 * y + 0.0236994711743375 * z;
        outB = 0.0358458302437845 * x - 0.0761723892680418 * y + 0.9568845240076871 * z;

        // Apply Display P3 transfer function (same as sRGB)
        auto gamma = [](double x) {
            return x <= 0.0031308 ? 12.92 * x : 1.055 * std::pow(x, 1.0/2.4) - 0.055;
        };
        outR = gamma(outR);
        outG = gamma(outG);
        outB = gamma(outB);
    }

    Color Color::FromDisplayP3(double r, double g, double b, int a)
    {
        // Inverse Display P3 transfer function
        auto invGamma = [](double x) {
            return x <= 0.04045 ? x / 12.92 : std::pow((x + 0.055) / 1.055, 2.4);
        };
        r = invGamma(r);
        g = invGamma(g);
        b = invGamma(b);

        // Convert to XYZ
        double x = 0.4865709486482162 * r + 0.2656676931690931 * g + 0.1982172852343625 * b;
        double y = 0.2289745640697488 * r + 0.6917385218365064 * g + 0.0792869140937449 * b;
        double z = 0.0000000000000000 * r + 0.0451133818589026 * g + 1.0439443689009766 * b;

        return FromXYZ_D65(x * 100, y * 100, z * 100, a);
    }

    // OKLab Functions
    void Color::ToOKLab(double& outL, double& outa, double& outb) const
    {
        // First convert to XYZ
        double x, y, z;
        ToXYZ_D65(x, y, z);
        x /= 100.0; y /= 100.0; z /= 100.0;

        // Convert to LMS
        double l = 0.8189330101 * x + 0.3618667424 * y - 0.1288597137 * z;
        double m = 0.0329845436 * x + 0.9293118715 * y + 0.0361456387 * z;
        double s = 0.0482003018 * x + 0.2643662691 * y + 0.6338517070 * z;

        // Non-linear compression
        l = std::cbrt(l);
        m = std::cbrt(m);
        s = std::cbrt(s);

        // Convert to OKLab
        outL = 0.2104542553 * l + 0.7936177850 * m - 0.0040720468 * s;
        outa = 1.9779984951 * l - 2.4285922050 * m + 0.4505937099 * s;
        outb = 0.0259040371 * l + 0.7827717662 * m - 0.8086757660 * s;
    }

    Color Color::FromOKLab(double l, double a, double b, int alpha)
    {
        // Convert from OKLab to LMS
        double l_ = l + 0.3963377774 * a + 0.2158037573 * b;
        double m_ = l - 0.1055613458 * a - 0.0638541728 * b;
        double s_ = l - 0.0894841775 * a - 1.2914855480 * b;

        // Reverse non-linear compression
        l_ = l_ * l_ * l_;
        m_ = m_ * m_ * m_;
        s_ = s_ * s_ * s_;

        // Convert to XYZ
        double x = +4.0767416621 * l_ - 3.3077115913 * m_ + 0.2309699292 * s_;
        double y = -1.2684380046 * l_ + 2.6097574011 * m_ - 0.3413193965 * s_;
        double z = -0.0041960863 * l_ - 0.7034186147 * m_ + 1.7076147010 * s_;

        return FromXYZ_D65(x * 100, y * 100, z * 100, alpha);
    }

    // OKLCH Functions
    void Color::ToOKLCH(double& outL, double& outC, double& outH) const
    {
        double l, a, b;
        ToOKLab(l, a, b);

        outL = l;
        outC = std::sqrt(a * a + b * b);
        outH = std::atan2(b, a) * 180.0 / CONST_PI;
        if (outH < 0) outH += 360.0;
    }

    Color Color::FromOKLCH(double l, double c, double h, int alpha)
    {
        double a = c * std::cos(h * CONST_PI / 180.0);
        double b = c * std::sin(h * CONST_PI / 180.0);

        return FromOKLab(l, a, b, alpha);
    }

    // ACEScg Functions
    void Color::ToACEScg(double& r, double& g, double& b) const {
        double nR, nG, nB;
        ToLinearSRGB(nR, nG, nB);

        r = 0.613132402633762 * nR + 0.339538018878689 * nG + 0.047329578487549 * nB;
        g = 0.070124380833467 * nR + 0.916394011313437 * nG + 0.013481607853096 * nB;
        b = 0.020587657528185 * nR + 0.109574571610682 * nG + 0.869837770861133 * nB;
    }

    Color Color::FromACEScg(double r, double g, double b, int a) {
        double nR = 1.641731751583133 * r - 0.605038145564156 * g - 0.036693606018977 * b;
        double nG = -0.125142085797839 * r + 1.096842503381956 * g + 0.028299582415883 * b;
        double nB = -0.020535295320694 * r - 0.029778933472654 * g + 1.050314228793348 * b;

        return FromLinearSRGB(nR, nG, nB, a);
    }

    // From Kelvin (°K)
    Color Color::FromTemp(double kelvin)
    {
        // Clamp to physically meaningful range
        kelvin = std::clamp(kelvin, 1000.0, 40000.0);

        // Calculate CIE 1931 xy chromaticity coordinates from CCT
        double x;
        if (kelvin <= 4000)
        {
            const double c1 = -0.2661239e9;
            const double c2 = -0.2343589e6;
            const double c3 = 0.8776956e3;
            x = ((c1 / kelvin + c2) / kelvin + c3) / kelvin + 0.179910;
        }
        else
        {
            const double c1 = -3.0258469e9;
            const double c2 = 2.1070379e6;
            const double c3 = 0.2226347e3;
            x = ((c1 / kelvin + c2) / kelvin + c3) / kelvin + 0.240390;
        }

        // Calculate y and convert to XYZ
        double y = -3.000 * x * x + 2.870 * x - 0.275;
        double Y = 1.0;
        double X = (Y / y) * x;
        double Z = (Y / y) * (1 - x - y);

        return FromXYZ_D65(X, Y, Z);
    }

    double Color::ToTemp() const
    {
        // Convert to xy chromaticity
        double x, y, z;
        ToXYZ_D65(x, y, z);
        double sum = x + y + z;
        x /= sum;
        y /= sum;

        // McCamy's approximation
        double n = (x - 0.3320) / (0.1858 - y);
        return 449.0 * pow(n, 3) + 3525.0 * pow(n, 2) + 6823.3 * n + 5520.33;
    }

    double Color::ToDuv() const
    {
        // Convert to xy chromaticity
        double x, y, z;
        ToXYZ_D65(x, y, z);
        double sum = x + y + z;
        x /= sum;
        y /= sum;

        // Convert to uv coordinates
        double denom = -2.0 * x + 12.0 * y + 3.0;
        double u = (4.0 * x) / denom;
        double v = (6.0 * y) / denom;

        // Calculate distance
        double Lfp = std::sqrt((u - 0.292) * (u - 0.292) + (v - 0.24) * (v - 0.24));
        double a = std::acos((u - 0.292) / Lfp);

        // Polynomial coefficients for Planckian locus
        static const double k[] = {-0.471106, 1.925865, -2.4243787, 1.5317403, -0.5179722, 0.0893944, -0.00616793};

        // Calculate Planckian locus distance using Horner's method
        double Lbb = k[6];
        for(int i = 5; i >= 0; --i) Lbb = Lbb * a + k[i];

        return Lfp - Lbb;
    }
    #pragma endregion

    #pragma region Color Scheme Generation
    std::vector<Color> Color::Monochromatic(const Color* input, int count)
    {
        std::vector<Color> colors;
        double h, s, l, a = input->a;
        input->ToHSL(h, s, l);

        double range = 40.0; // Adjustable range for variation
        double minL = std::max(0.0, l - range);
        double maxL = std::min(100.0, l + range);
        double step = (maxL - minL) / (count - 1);

        for (int i = 0; i < count; ++i) colors.push_back(FromHSL(h, s, minL + (i * step), a));

        return colors;
    }

    std::vector<Color> Color::Analogous(const Color* input, double angle, int count)
    {
        std::vector<Color> colors;
        double h, s, l, a = input->a;
        input->ToHSL(h, s, l);

        double totalAngle = angle * (count - 1);
        double startAngle = h - totalAngle / 2;

        for (int i = 0; i < count; ++i) colors.push_back(FromHSL(std::fmod(startAngle + i * angle + 360.0, 360.0), s, l, a));
        return colors;
    }

    std::vector<Color> Color::Triadic(const Color* input, double angle)
    {
        std::vector<Color> colors;
        double h, s, l, a = input->a;
        input->ToHSL(h, s, l);

        colors.push_back(*input);
        colors.push_back(FromHSL(std::fmod(h + angle      , 360.0), s, l, a));
        colors.push_back(FromHSL(std::fmod(h + (2 * angle), 360.0), s, l, a));

        return colors;
    }

    std::vector<Color> Color::Tetradic(const Color* input, double angle)
    {
        std::vector<Color> colors;
        double h, s, l, a = input->a;
        input->ToHSL(h, s, l);

        colors.push_back(*input);
        colors.push_back(FromHSL(std::fmod(h + angle    , 360.0), s, l, a));
        colors.push_back(FromHSL(std::fmod(h + 2 * angle, 360.0), s, l, a));
        colors.push_back(FromHSL(std::fmod(h + 3 * angle, 360.0), s, l, a));

        return colors;
    }

    #pragma endregion

    #pragma region Color Manipulation
    void Color::ShiftColorComponent(Color* color, double amount, void (Color::*toFunc)(double&, double&, double&) const, Color (*fromFunc)(double, double, double, int), int componentIndex, double minRange, double maxRange)
    {
        double v1, v2, v3;
        (color->*toFunc)(v1, v2, v3);
        double* components[] = {&v1, &v2, &v3};

        double range = maxRange - minRange;
        double normalized = (*components[componentIndex] - minRange) / range;
        normalized = std::clamp(normalized + amount/100.0, 0.0, 1.0);
        *components[componentIndex] = normalized * range + minRange;

        *color = fromFunc(v1, v2, v3, color->a);
    }

    void Color::ShiftTemp(double amount) { *this = FromTemp(ToTemp() + amount); }

    void Color::ShiftHue(double degrees) { ShiftColorComponent(this, degrees, &Color::ToHSL, &Color::FromHSL, 0, 0, 360); }

    void Color::ShiftSaturation(double amount) { ShiftColorComponent(this, amount, &Color::ToHSL, &Color::FromHSL, 1); }

    void Color::ShiftLightness(double amount) { ShiftColorComponent(this, amount, &Color::ToHSL, &Color::FromHSL, 2); }

    void Color::ShiftValue(double amount) { ShiftColorComponent(this, amount, &Color::ToHSV, &Color::FromHSV, 2); }

    void Color::ShiftIntensity(double amount) { ShiftColorComponent(this, amount, &Color::ToHSI, &Color::FromHSI, 2, 0, 255); }

    void Color::ShiftPerception(double amount) { ShiftColorComponent(this, amount, &Color::ToHSP, &Color::FromHSP, 2, 0, 255); }

    void Color::ShiftChroma(double amount) { ShiftColorComponent(this, amount, &Color::ToHCY, &Color::FromHCY, 1); }

    void Color::ShiftLuma(double amount) { ShiftColorComponent(this, amount, &Color::ToHCY, &Color::FromHCY, 2, 0, 255); }

    void Color::ShiftGray(double amount) { ShiftColorComponent(this, amount, &Color::ToHCG, &Color::FromHCG, 2); }

    void Color::Complement() { this->ShiftHue(180); }

    void Color::ShiftWhiteLevel(double amount)
    {
        double w = std::clamp(amount / 100.0, -1.0, 1.0);

        ColorMatrix whiteLevelMatrix = {{
            {1, w, w, 0, 0},
            {w, 1, w, 0, 0},
            {w, w, 1, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 1}
        }};

        *this *= whiteLevelMatrix;
    }

    void Color::ShiftBlackLevel(double amount)
    {
        double b = 1.0 - std::clamp(amount / 100.0, -1.0, 1.0);

        ColorMatrix blackLevelMatrix = {{
            {b, 0, 0, 0, 0},
            {0, b, 0, 0, 0},
            {0, 0, b, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 1}
        }};

        *this *= blackLevelMatrix;
    }

    void Color::ShiftContrast(double amount)
    {
        amount = std::clamp(amount / 100, -1.0, 1.0);
        double factor = 1.0 / (1.0 - amount) - 1.0;

        ColorMatrix contrastMatrix = {{
            {1.0 + factor, 0           , 0           , 0, 0},
            {0           , 1.0 + factor, 0           , 0, 0},
            {0           , 0           , 1.0 + factor, 0, 0},
            {0           , 0           , 0           , 1, 0},
            {0           , 0           , 0           , 0, 1}
        }};

        ColorMatrix offsetMatrix = {{
            {0, 0, 0, 0, -factor/2},
            {0, 0, 0, 0, -factor/2},
            {0, 0, 0, 0, -factor/2},
            {0, 0, 0, 1, 0        },
            {0, 0, 0, 0, 1        }
        }};

        *this *= contrastMatrix;
        *this += offsetMatrix;
    }

    void Color::Grayscale(double factor)
    {
        ColorMatrix grayscaleMatrix = {{
            {0.299 * factor, 0.587 * factor, 0.114 * factor, 0, 0},
            {0.299 * factor, 0.587 * factor, 0.114 * factor, 0, 0},
            {0.299 * factor, 0.587 * factor, 0.114 * factor, 0, 0},
            {0             , 0             , 0             , 1, 0},
            {0             , 0             , 0             , 0, 1}
        }};

        *this *= grayscaleMatrix;
    }

    void Color::Sepia(double factor)
    {
        ColorMatrix sepiaMatrix = {{
            {0.393 * factor, 0.769 * factor, 0.189 * factor, 0, 0},
            {0.349 * factor, 0.686 * factor, 0.168 * factor, 0, 0},
            {0.272 * factor, 0.534 * factor, 0.131 * factor, 0, 0},
            {0             , 0             , 0             , 1, 0},
            {0             , 0             , 0             , 0, 1}
        }};

        *this *= sepiaMatrix;
    }

    void Color::CrossProcess(double factor)
    {
        ColorMatrix crossProcessMatrix = {{
            {1.0 * factor, 0.2 * factor,  0.1 * factor, 0, 0},
            {0.0 * factor, 1.1 * factor, -0.1 * factor, 0, 0},
            {0.1 * factor, 0.1 * factor,  1.0 * factor, 0, 0},
            {0           , 0           ,  0           , 1, 0},
            {0           , 0           ,  0           , 0, 1}
        }};

        *this *= crossProcessMatrix;
    }

    void Color::Moonlight(double factor)
    {
        ColorMatrix moonlightMatrix = {{
            {0.7 * factor, 0.0 * factor, 0.3 * factor, 0, 0},
            {0.0 * factor, 0.9 * factor, 0.1 * factor, 0, 0},
            {0.2 * factor, 0.1 * factor, 1.1 * factor, 0, 0},
            {0           , 0           , 0           , 1, 0},
            {0           , 0           , 0           , 0, 1}
        }};

        *this *= moonlightMatrix;
    }

    void Color::VintageFilm(double factor)
    {
        ColorMatrix vintageMatrix = {{
            {0.9 * factor, 0.1 * factor, 0.1 * factor, 0, 0},
            {0.1 * factor, 0.9 * factor, 0.1 * factor, 0, 0},
            {0.1 * factor, 0.2 * factor, 0.7 * factor, 0, 0},
            {0           , 0           , 0           , 1, 0},
            {0           , 0           , 0           , 0, 1}
        }};

        *this *= vintageMatrix;
    }

    void Color::Technicolor(double factor)
    {
        ColorMatrix technicolorMatrix = {{
            { 1.6 * factor, -0.4 * factor, -0.2 * factor, 0, 0},
            {-0.2 * factor,  1.4 * factor, -0.2 * factor, 0, 0},
            {-0.2 * factor, -0.4 * factor,  1.6 * factor, 0, 0},
            { 0           ,  0           ,  0           , 1, 0},
            { 0           ,  0           ,  0           , 0, 1}
        }};

        *this *= technicolorMatrix;
    }

    void Color::Polaroid(double factor)
    {
        ColorMatrix polaroidMatrix = {{
            { 1.438 * factor, -0.062 * factor, -0.062 * factor, 0, 0},
            {-0.122 * factor,  1.378 * factor, -0.122 * factor, 0, 0},
            {-0.016 * factor, -0.016 * factor,  1.483 * factor, 0, 0},
            { 0             ,  0             ,  0             , 1, 0},
            { 0             ,  0             ,  0             , 0, 1}
        }};

        *this *= polaroidMatrix;
    }

    // Mix, Average, Screen, Multiply, Overlay
    Color Color::Mix(const Color& color1, const Color& color2, double weight)
    {
        weight = std::clamp(weight, 0.0, 1.0);
        return color1 * (1 - weight) + color2 * weight;
    }

    Color Color::Average(const Color** colors, size_t count)
    {
        if (count == 0 || colors == nullptr) return Color();

        uint32_t r = 0, g = 0, b = 0, a = 0;
        for (size_t i = 0; i < count; ++i)
        {
            r += colors[i]->r;
            g += colors[i]->g;
            b += colors[i]->b;
            a += colors[i]->a;
        }

        return Color(r / count, g / count, b / count, a / count);
    }

    Color Color::Screen(const Color& color1, const Color& color2)
    {
        return Color(
            255 - ((255 - color1.r) * (255 - color2.r)) / 255,
            255 - ((255 - color1.g) * (255 - color2.g)) / 255,
            255 - ((255 - color1.b) * (255 - color2.b)) / 255,
            255 - ((255 - color1.a) * (255 - color2.a)) / 255
        );
    }

    Color Color::Multiply(const Color& color1, const Color& color2)
    {
        return color1 * color2;
    }

    Color Color::Overlay(const Color& color1, const Color& color2)
    {
        auto overlay = [](uint8_t a, uint8_t b)
        {
            return (a < 128) ? (2 * a * b) / 255 : 255 - 2 * ((255 - a) * (255 - b)) / 255;
        };

        double a1 = color1.a / 255.0, a2 = color2.a / 255.0;
        double outAlpha = 255 * (a1 + a2 - (a1 * a2));

        return Color(
            overlay(color1.r, color2.r),
            overlay(color1.g, color2.g),
            overlay(color1.b, color2.b),
            outAlpha
        );
    }
    #pragma endregion

    #pragma region Utility
    double Color::GetHue() const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        double max = std::max({nR, nG, nB});
        double min = std::min({nR, nG, nB});
        double delta = max - min;

        if (delta == 0) return 0.0;

        double hue;
        if (nR == max)
            hue = std::fmod((nG - nB) / delta, 6.0);
        else if (nG == max)
            hue = 2 + (nB - nR) / delta;
        else
            hue = 4 + (nR - nG) / delta;

        hue += 60;
        if (hue < 0)
            hue += 360;

        return hue;
    }

    double Color::GetLuminance() const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        nR = (nR <= 0.03928) ? nR / 12.92 : std::pow((nR + 0.055) / 1.055, 2.4);
        nG = (nG <= 0.03928) ? nG / 12.92 : std::pow((nG + 0.055) / 1.055, 2.4);
        nB = (nB <= 0.03928) ? nB / 12.92 : std::pow((nB + 0.055) / 1.055, 2.4);

        return 0.2126 * nR + 0.7152 * nG + 0.0722 * nB;
    }

    double Color::GetContrast(const Color& other) const
    {
        double l1 = GetLuminance();
        double l2 = other.GetLuminance();
        if (l1 > l2)
            return (l1 + 0.05) / (l2 + 0.05);
        return (l2 + 0.05) / (l1 + 0.05);
    }

    bool Color::IsAccessible(const Color& background, const AccessibilityLevel level) const
    {
        double contrast = GetContrast(background);
        switch (level)
        {
            case AccessibilityLevel::AA:
                return contrast >= 4.5;
            case AccessibilityLevel::AAA:
                return contrast >= 7.0;
            default:
                return false;
        }
    }

    int Color::ToInt(int format) const
    {
        switch (format)
        {
            case 0:  return (a << 24) | (r << 16) | (g << 8) | b; // 0xAARRGGBB
            case 1:  return (r << 16) | (g << 8 ) | (b     )    ; // 0xRRGGBB
            case 2:  return (a << 24) | (b << 16) | (g << 8) | r; // 0xAABBGGRR
            case 3:  return (b << 16) | (g << 8 ) | (r     )    ; // 0xBBGGRR
            case 4:  return (r << 24) | (g << 16) | (b << 8) | a; // 0xRRGGBBAA
            case 5:  return (b << 24) | (g << 16) | (r << 8) | a; // 0xBBGGRRAA
            default: return (a << 24) | (r << 16) | (g << 8) | b; // Default to 0xAARRGGBB
        }
    }

    Color Color::Random(bool alpharand)
    {
        static std::mt19937 gen(std::random_device{}());
        static std::uniform_int_distribution<> dis(0, 255);

        return Color(
            dis(gen),
            dis(gen),
            dis(gen),
            alpharand ? dis(gen) : 255
        );
    }

    std::string Color::GetDefaultFormat(const char* type)
    {
        if (strcmp(type, "Hex")  == 0)        return "0x{A}{R}{G}{B}";
        if (strcmp(type, "RGB")  == 0)        return "rgba({R}, {G}, {B}, {A})";
        if (strcmp(type, "HSL")  == 0)        return "hsl({H}, {S}%, {L}%)";
        if (strcmp(type, "HSV")  == 0)        return "hsv({H}, {S}%, {V}%)";
        if (strcmp(type, "HSI")  == 0)        return "hsi({H}, {S}%, {I})";
        if (strcmp(type, "HWB")  == 0)        return "hwb({H}, {W}%, {B}%)";
        if (strcmp(type, "HSP") == 0)         return "hsp({H}, {S}, {P})";
        if (strcmp(type, "HCY") == 0)         return "hcy({H}, {C}, {Y})";
        if (strcmp(type, "HCG") == 0)         return "hcg({H}, {C}, {G})";
        if (strcmp(type, "TSL") == 0)         return "tsl({T}, {S}, {L})";
        if (strcmp(type, "CMY") == 0)         return "cmy({C}, {M}, {Y})";
        if (strcmp(type, "CMYK") == 0)        return "cmyk({C}, {M}, {Y}, {K})";
        if (strcmp(type, "XYZ")  == 0)        return "xyz({X}, {Y}, {Z})";
        if (strcmp(type, "UCS")  == 0)        return "ucs({U}, {C}, {S})";
        if (strcmp(type, "UVW")  == 0)        return "uvw({U}, {V}, {W})";
        if (strcmp(type, "XYY") == 0)         return "xyy({X}, {Y1}, {Y2})";
        if (strcmp(type, "Lab")  == 0)        return "lab({L}, {A}, {B})";
        if (strcmp(type, "NCol") == 0)        return "ncol({H}, {W}, {B})";
        if (strcmp(type, "SRGB") == 0)        return "srgb({R}, {G}, {B})";
        if (strcmp(type, "ProPhotoRGB") == 0) return "prophoto({R}, {G}, {B})";
        if (strcmp(type, "AdobeRGB") == 0)    return "adobe({R}, {G}, {B})";
        if (strcmp(type, "Oklab") == 0)       return "oklab({L}, {A}, {B})";
        if (strcmp(type, "Luv")  == 0)        return "luv({L}, {U}, {V})";
        if (strcmp(type, "LCHab") == 0)       return "lchab({L}, {C}, {H})";
        if (strcmp(type, "YPbPr") == 0)       return "ypbpr({Y}, {Pb}, {Pr})";
        if (strcmp(type, "YCbCr") == 0)       return "ycbcr({Y}, {Cb}, {Cr})";
        if (strcmp(type, "YCgCo") == 0)       return "ycgco({Y}, {Cg}, {Co})";
        if (strcmp(type, "YcCbcCrc") == 0)    return "yccbccrc({Yc}, {Cbc}, {Crc})";
        if (strcmp(type, "YDbDr") == 0)       return "ydbdr({Y}, {Db}, {Dr})";
        if (strcmp(type, "YUV") == 0)         return "yuv({Y}, {U}, {V})";
        if (strcmp(type, "YES") == 0)         return "yes({Y}, {E}, {S})";
        if (strcmp(type, "JPEG") == 0)         return "jpeg({Y}, {Cb}, {Cr})";
        if (strcmp(type, "YIQ")  == 0)        return "yiq({Y}, {I}, {Q})";
        if (strcmp(type, "Rec2020") == 0)     return "rec2020({R}, {G}, {B})";
        if (strcmp(type, "DisplayP3") == 0)   return "displayp3({R}, {G}, {B})";
        if (strcmp(type, "OKLab") == 0)       return "oklab({L}, {A}, {B})";
        if (strcmp(type, "OKLCH") == 0)       return "oklch({L}, {C}, {H})";
        if (strcmp(type, "ACEScg") == 0)      return "acescg({R}, {G}, {B})";
        throw std::invalid_argument("Unsupported color type");
    }

    std::string Color::ReplaceAll(std::string str, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    std::string Color::ToString(const char* type, const std::string& format) const
    {
        std::string lowerType = type;
        std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
        std::string result = format.empty() ? Color::GetDefaultFormat(lowerType.c_str()) : format;

        auto replaceFormatter = [&](const std::string& placeholder, double value) {
            std::string lowerPlaceholder = placeholder;
            std::transform(lowerPlaceholder.begin(), lowerPlaceholder.end(), lowerPlaceholder.begin(), ::tolower);

            std::string lowerResult = result;
            std::transform(lowerResult.begin(), lowerResult.end(), lowerResult.begin(), ::tolower);

            size_t pos = lowerResult.find(lowerPlaceholder);
            if (pos != std::string::npos) {
                size_t endPos = result.find('}', pos);
                std::string formatSpec = result.substr(pos, endPos - pos + 1);
                size_t precisionPos = formatSpec.find(':');
                int precision = 0;
                if (precisionPos != std::string::npos) {
                    precision = std::stoi(formatSpec.substr(precisionPos + 1));
                }
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(precision) << value;
                result.replace(pos, endPos - pos + 1, ss.str());
            }
        };

        if (strcmp(lowerType.c_str(), "rgb") == 0)
        {
            int r, g, b, a;
            ToRGB(r, g, b, a);
            replaceFormatter("{R", static_cast<double>(r));
            replaceFormatter("{G", static_cast<double>(g));
            replaceFormatter("{B", static_cast<double>(b));
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "linearsrgb") == 0)
        {
            double r, g, b;
            ToLinearSRGB(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hsl") == 0)
        {
            double h, s, l;
            ToHSL(h, s, l);
            replaceFormatter("{H", h);
            replaceFormatter("{S", s);
            replaceFormatter("{L", l);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hsv") == 0)
        {
            double h, s, v;
            ToHSV(h, s, v);
            replaceFormatter("{H", h);
            replaceFormatter("{S", s);
            replaceFormatter("{V", v);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hsi") == 0)
        {
            double h, s, i;
            ToHSI(h, s, i);
            replaceFormatter("{H", h);
            replaceFormatter("{S", s);
            replaceFormatter("{I", i);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hwb") == 0)
        {
            double h, w, b;
            ToHWB(h, w, b);
            replaceFormatter("{H", h);
            replaceFormatter("{W", w);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hsp") == 0)
        {
            double h, s, p;
            ToHSP(h, s, p);
            replaceFormatter("{H", h);
            replaceFormatter("{S", s);
            replaceFormatter("{P", p);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hcy") == 0)
        {
            double h, c, y;
            ToHCY(h, c, y);
            replaceFormatter("{H", h);
            replaceFormatter("{C", c);
            replaceFormatter("{Y", y);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hcg") == 0)
        {
            double h, c, g;
            ToHCG(h, c, g);
            replaceFormatter("{H", h);
            replaceFormatter("{C", c);
            replaceFormatter("{G", g);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "tsl") == 0)
        {
            double t, s, l;
            ToTSL(t, s, l);
            replaceFormatter("{T", t);
            replaceFormatter("{S", s);
            replaceFormatter("{L", l);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "cmy") == 0)
        {
            double c, m, y;
            ToCMY(c, m, y);
            replaceFormatter("{C", c);
            replaceFormatter("{M", m);
            replaceFormatter("{Y", y);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "cmyk") == 0)
        {
            double c, m, y, k;
            ToCMYK(c, m, y, k);
            replaceFormatter("{C", c);
            replaceFormatter("{M", m);
            replaceFormatter("{Y", y);
            replaceFormatter("{K", k);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "xyz_d50") == 0)
        {
            double x, y, z;
            ToXYZ_D50(x, y, z);
            replaceFormatter("{X", x);
            replaceFormatter("{Y", y);
            replaceFormatter("{Z", z);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "xyz_d65") == 0)
        {
            double x, y, z;
            ToXYZ_D65(x, y, z);
            replaceFormatter("{X", x);
            replaceFormatter("{Y", y);
            replaceFormatter("{Z", z);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "ucs") == 0)
        {
            double u, c, s;
            ToUCS(u, c, s);
            replaceFormatter("{U", u);
            replaceFormatter("{C", c);
            replaceFormatter("{S", s);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "uvw") == 0)
        {
            double u, v, w;
            ToUVW(u, v, w);
            replaceFormatter("{U", u);
            replaceFormatter("{V", v);
            replaceFormatter("{W", w);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "xyy") == 0)
        {
            double x, y1, y2;
            ToXYY(x, y1, y2);
            replaceFormatter("{X", x);
            replaceFormatter("{Y1", y1);
            replaceFormatter("{Y2", y2);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "lab") == 0)
        {
            double l, a, b;
            ToLab(l, a, b);
            replaceFormatter("{L", l);
            replaceFormatter("{A", a);
            replaceFormatter("{B", b);
            replaceFormatter("{T", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "ypbpr") == 0)
        {
            double y, cb, cr;
            ToYPbPr(y, cb, cr);
            replaceFormatter("{Y", y);
            replaceFormatter("{Pb", cb);
            replaceFormatter("{Pr", cr);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "ycbcr") == 0)
        {
            double y, cb, cr;
            ToYCbCr(y, cb, cr);
            replaceFormatter("{Y", y);
            replaceFormatter("{Cb", cb);
            replaceFormatter("{Cr", cr);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "ycgco") == 0)
        {
            double y, cg, co;
            ToYCgCo(y, cg, co);
            replaceFormatter("{Y", y);
            replaceFormatter("{Cg", cg);
            replaceFormatter("{Co", co);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "yccbccrc") == 0)
        {
            double yc, cbc, crc;
            ToYcCbcCrc(yc, cbc, crc);
            replaceFormatter("{Yc", yc);
            replaceFormatter("{Cbc", cbc);
            replaceFormatter("{Crc", crc);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "ydbdr") == 0)
        {
            double y, db, dr;
            ToYDbDr(y, db, dr);
            replaceFormatter("{Y", y);
            replaceFormatter("{Db", db);
            replaceFormatter("{Dr", dr);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "yuv") == 0)
        {
            double y, u, v;
            ToYUV(y, u, v);
            replaceFormatter("{Y", y);
            replaceFormatter("{U", u);
            replaceFormatter("{V", v);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "yes") == 0)
        {
            double y, e, s;
            ToYES(y, e, s);
            replaceFormatter("{Y", y);
            replaceFormatter("{E", e);
            replaceFormatter("{S", s);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "jpeg") == 0)
        {
            double y, cb, cr;
            ToJPEG(y, cb, cr);
            replaceFormatter("{Y", y);
            replaceFormatter("{Cb", cb);
            replaceFormatter("{Cr", cr);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "yiq") == 0)
        {
            double y, i, q;
            ToYIQ(y, i, q);
            replaceFormatter("{Y", y);
            replaceFormatter("{I", i);
            replaceFormatter("{Q", q);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "lchab") == 0)
        {
            double l, c, h;
            ToLCHab(l, c, h);
            replaceFormatter("{L", l);
            replaceFormatter("{C", c);
            replaceFormatter("{H", h);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "lchuv") == 0)
        {
            double l, c, h;
            ToLCHuv(l, c, h);
            replaceFormatter("{L", l);
            replaceFormatter("{C", c);
            replaceFormatter("{H", h);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "adobergb") == 0)
        {
            double r, g, b;
            ToAdobeRGB(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "prophotorgb") == 0)
        {
            double r, g, b;
            ToProPhotoRGB(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "luv") == 0)
        {
            double L, u, v;
            ToLuv(L, u, v);
            replaceFormatter("{L", L);
            replaceFormatter("{U", u);
            replaceFormatter("{V", v);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "hex") == 0)
        {
            std::string a, r, g, b;
            ToHex(a, r, g, b);
            result = Color::ReplaceAll(result, "{A}", a);
            result = Color::ReplaceAll(result, "{R}", r);
            result = Color::ReplaceAll(result, "{G}", g);
            result = Color::ReplaceAll(result, "{B}", b);
            result = Color::ReplaceAll(result, "{a}", a);
            result = Color::ReplaceAll(result, "{r}", r);
            result = Color::ReplaceAll(result, "{g}", g);
            result = Color::ReplaceAll(result, "{b}", b);
        }
        else if (strcmp(lowerType.c_str(), "ncol") == 0)
        {
            std::string n;
            double c, l;
            ToNCol(n, c, l);
            result = Color::ReplaceAll(result, "{H}", n);
            result = Color::ReplaceAll(result, "{h}", n);
            replaceFormatter("{W", c);
            replaceFormatter("{B", (100 - l));
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "rec2020") == 0)
        {
            double r, g, b;
            ToRec2020(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "displayp3") == 0)
        {
            double r, g, b;
            ToDisplayP3(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "oklab") == 0)
        {
            double l, a, b;
            ToOKLab(l, a, b);
            replaceFormatter("{L", l);
            replaceFormatter("{A", a);
            replaceFormatter("{B", b);
            replaceFormatter("{T", static_cast<double>(this->a));
        }
        else if (strcmp(lowerType.c_str(), "oklch") == 0)
        {
            double l, c, h;
            ToOKLCH(l, c, h);
            replaceFormatter("{L", l);
            replaceFormatter("{C", c);
            replaceFormatter("{H", h);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(lowerType.c_str(), "acescg") == 0)
        {
            double r, g, b;
            ToACEScg(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else
        {
            throw std::invalid_argument("Unsupported color type");
        }

        return result;
    }
    #pragma endregion
}