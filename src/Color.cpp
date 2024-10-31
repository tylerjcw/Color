#define NOMINMAX

#include "Color.h"
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
        *this = *this * matrix;
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
        outR /= 100.0;
        outG /= 100.0;
        outB /= 100.0;

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

        double r, g, b;
        if (h < 1) { r = c; g = x; b = 0; }
        else if (h < 2) { r = x; g = c; b = 0; }
        else if (h < 3) { r = 0; g = c; b = x; }
        else if (h < 4) { r = 0; g = x; b = c; }
        else if (h < 5) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        return Color(
            static_cast<unsigned char>((r + m) * 255),
            static_cast<unsigned char>((g + m) * 255),
            static_cast<unsigned char>((b + m) * 255),
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
            static_cast<unsigned char>((r + m) * 255),
            static_cast<unsigned char>((g + m) * 255),
            static_cast<unsigned char>((b + m) * 255),
            a
        );
    }

    //HSI Functions
    void Color::ToHSI(double& h, double& s, double& i) const
    {
        double nR, nG, nB;
        Normalize(nR, nG, nB);

        double max = std::max({nR, nG, nB});
        double min = std::min({nR, nG, nB});
        double chroma = max - min;

        i = (nR + nG + nB) / 3.0;
        s = (i > 0) ? 1 - (min / i) : 0;

        if (chroma == 0)
            h = 0;
        else if (nR >= nG && nR >= nB)
            h = (nG - nB) / chroma;
        else if (nG >= nR && nG >= nB)
            h = 2 + (nB - nR) / chroma;
        else
            h = 4 + (nR - nG) / chroma;

        h *= 60;
        if (h < 0) h += 360;

        i *= 255;
        s *= 100;
    }

    Color Color::FromHSI(double h, double s, double i, int a)
    {
        h = std::fmod(h, 360.0) / 60.0;
        s /= 100.0;
        i /= 255.0;

        double x = i * (1 - s);
        double y = i * (1 + s * std::cos(h * CONST_PI / 3) / std::cos((CONST_PI / 3) - (h * CONST_PI / 3)));
        double z = 3 * i - (x + y);

        double r, g, b;
        if (h < 1) { r = y; g = z; b = x; }
        else if (h < 2) { r = z; g = y; b = x; }
        else if (h < 3) { r = x; g = y; b = z; }
        else if (h < 4) { r = x; g = z; b = y; }
        else if (h < 5) { r = z; g = x; b = y; }
        else { r = y; g = x; b = z; }

        return Color(
            static_cast<unsigned char>(r * 255),
            static_cast<unsigned char>(g * 255),
            static_cast<unsigned char>(b * 255),
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
            static_cast<unsigned char>((r + m) * 255),
            static_cast<unsigned char>((g + m) * 255),
            static_cast<unsigned char>((b_ + m) * 255),
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
        x = (0.4360747 * newR + 0.3850649 * newG + 0.1430804 * newB) * 100;
        y = (0.2225045 * newR + 0.7168786 * newG + 0.0606169 * newB) * 100;
        z = (0.0139322 * newR + 0.0971045 * newG + 0.7141733 * newB) * 100;
    }

    Color Color::FromXYZ_D50(double x, double y, double z, int a)
    {
        x /= 100;
        y /= 100;
        z /= 100;

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

    Color Color::FromYPbPr(double y, double cb, double cr, int a)
    {
        double r = std::clamp(y + 0.0000 * cb + 1.4022 * cr, 0.0, 1.0);
        double g = std::clamp(y - 0.3456 * cb - 0.7145 * cr, 0.0, 1.0);
        double b = std::clamp(y + 1.7710 * cb + 0.0000 * cr, 0.0, 1.0);

        return Color(
            static_cast<unsigned char>(std::clamp(r * 255.0, 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(g * 255.0, 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(b * 255.0, 0.0, 255.0)),
            a
        );
    }

    // LCHab
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

    // LCHuv
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

    // YIQ
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

    // NCol
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

    // From Kelvin (°K)
    Color Color::FromTemp(double kelvin)
    {
        double temp = kelvin / 100;
        double r, g, b;

        if (temp <= 66)
            r = 255;
        else
            r = std::clamp(329.698727446 * std::pow(temp - 60, -0.1332047592), 0.0, 255.0);

        if (temp <= 66)
            g = std::clamp(99.4708025861 * std::log(static_cast<float>(temp)) - 161.1195681661, 0.0, 255.0);
        else
            g = std::clamp(288.1221695283 * std::pow(temp - 60, -0.0755148492), 0.0, 255.0);

        if (temp >= 66)
            b = 255;
        else if (temp <= 19)
            b = 0;
        else
            b = std::clamp(138.5177312231 * std::log(static_cast<float>(temp - 10)) - 305.0447927307, 0.0, 255.0);

        return Color(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b));
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
    void Color::ShiftHue(double degrees)
    {
        double cosA = cos(degrees * CONST_PI / 180.0);
        double sinA = sin(degrees * CONST_PI / 180.0);

        const double r = 0.299, g = 0.587, b = 0.114;
        ColorMatrix hueMatrix = {{
            {r + (1 - r) * cosA + 0.168 * sinA, g - g * cosA + 0.330 * sinA      , b - b * cosA - 0.497 * sinA      , 0, 0},
            {r - r * cosA - 0.328 * sinA      , g + (1 - g) * cosA + 0.035 * sinA, b - b * cosA + 0.292 * sinA      , 0, 0},
            {r - r * cosA + 1.250 * sinA      , g - g * cosA - 1.050 * sinA      , b + (1 - b) * cosA - 0.203 * sinA, 0, 0},
            {0                                , 0                                , 0                                , 1, 0},
            {0                                , 0                                , 0                                , 0, 1}
        }};

        *this *= hueMatrix;
    }

    void Color::ShiftSaturation(double amount)
    {
        amount = std::clamp(amount / 100.0, -1.0, 1.0);
        double s = 1.0 + amount;
        const double r = 0.299, g = 0.587, b = 0.114;

        ColorMatrix satMatrix = {{
            {r + (1-r) * s, g - g * s    , b - b * s    , 0, 0},
            {r - r * s    , g + (1-g) * s, b - b * s    , 0, 0},
            {r - r * s    , g - g * s    , b + (1-b) * s, 0, 0},
            {0            , 0            , 0            , 1, 0},
            {0            , 0            , 0            , 0, 1}
        }};


        *this *= satMatrix;
    }

    void Color::ShiftLightness(double amount)
    {
        double l = 1 + std::clamp(amount / 100.0, -1.0, 1.0);
        const double r = 0.2126, g = 0.7152, b = 0.0722;

        ColorMatrix lightMatrix = {{
            {r + (1-r) * l, r - r * l    , r - r * l    , 0, 0},
            {g - g * l    , g + (1-g) * l, g - g * l    , 0, 0},
            {b - b * l    , b - b * l    , b + (1-b) * l, 0, 0},
            {0            , 0            , 0            , 1, 0},
            {0            , 0            , 0            , 0, 1}
        }};

        *this *= lightMatrix;
    }

    void Color::ShiftValue(double amount)
    {
        double v = 1 + std::clamp(amount / 100.0, -1.0, 1.0);

        ColorMatrix valueMatrix = {{
            {v, 0, 0, 0, 0},
            {0, v, 0, 0, 0},
            {0, 0, v, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 1}
        }};

        *this *= valueMatrix;
    }

    void Color::ShiftIntensity(double amount)
    {
        double i = std::clamp(amount / 100.0, -1.0, 1.0) / 3.0;
        double s = 1 + i;

        ColorMatrix intensityMatrix = {{
            {s, i, i, 0, 0},
            {i, s, i, 0, 0},
            {i, i, s, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 1}
        }};

        *this *= intensityMatrix;
    }

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

    void Color::Complement() { this->ShiftHue(180); }

    void Color::Grayscale()
    {
        ColorMatrix grayscaleMatrix = {{
            {0.299, 0.587, 0.114, 0, 0},
            {0.299, 0.587, 0.114, 0, 0},
            {0.299, 0.587, 0.114, 0, 0},
            {0    , 0    , 0    , 1, 0},
            {0    , 0    , 0    , 0, 1}
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

        ShiftValue(-10);
        ShiftBlue(20);
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

        ShiftSaturation(-15);
        ShiftRed(10);
        ShiftGreen(5);
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

        ShiftSaturation(30);
        ShiftContrast(20);
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

        ShiftContrast(25);
        ShiftRed(10);
        ShiftGreen(5);
    }

    // Mix, Average, Multiply
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
        if (strcmp(type, "RGB")  == 0)        return "rgba({R}, {G}, {B}, {A})";
        if (strcmp(type, "HSL")  == 0)        return "hsl({H}, {S}%, {L}%)";
        if (strcmp(type, "HSV")  == 0)        return "hsv({H}, {S}%, {V}%)";
        if (strcmp(type, "HSI")  == 0)        return "hsi({H}, {S}%, {I})";
        if (strcmp(type, "HWB")  == 0)        return "hwb({H}, {W}%, {B}%)";
        if (strcmp(type, "CMYK") == 0)        return "cmyk({C}, {M}, {Y}, {K})";
        if (strcmp(type, "XYZ")  == 0)        return "xyz({X}, {Y}, {Z})";
        if (strcmp(type, "Lab")  == 0)        return "lab({L}, {A}, {B})";
        if (strcmp(type, "YIQ")  == 0)        return "yiq({Y}, {I}, {Q})";
        if (strcmp(type, "Hex")  == 0)        return "0x{A}{R}{G}{B}";
        if (strcmp(type, "NCol") == 0)        return "ncol({H}, {W}, {B})";
        if (strcmp(type, "SRGB") == 0)        return "srgb({R}, {G}, {B})";
        if (strcmp(type, "ProPhotoRGB") == 0) return "prophoto({R}, {G}, {B})";
        if (strcmp(type, "AdobeRGB") == 0)    return "adobe({R}, {G}, {B})";
        if (strcmp(type, "Oklab") == 0)       return "oklab({L}, {A}, {B})";
        if (strcmp(type, "Luv")  == 0)        return "luv({L}, {U}, {V})";
        if (strcmp(type, "LCHab") == 0)       return "lchab({L}, {C}, {H})";
        if (strcmp(type, "YPbPr") == 0)       return "ypbpr({Y}, {Cb}, {Cr})";
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
        std::string result = format.empty() ? Color::GetDefaultFormat(type) : format;

        auto replaceFormatter = [&](const std::string& placeholder, double value)
        {
            size_t pos = result.find(placeholder);
            if (pos != std::string::npos)
            {
                size_t endPos = result.find('}', pos);
                std::string formatSpec = result.substr(pos, endPos - pos + 1);
                size_t precisionPos = formatSpec.find(':');
                int precision = 0;
                if (precisionPos != std::string::npos)
                {
                    precision = std::stoi(formatSpec.substr(precisionPos + 1));
                }
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(precision) << value;
                result.replace(pos, endPos - pos + 1, ss.str());
            }
        };

        if (strcmp(type, "RGB") == 0)
        {
            int r, g, b, a;
            ToRGB(r, g, b, a);
            replaceFormatter("{R", static_cast<double>(r));
            replaceFormatter("{G", static_cast<double>(g));
            replaceFormatter("{B", static_cast<double>(b));
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "LinearSRGB") == 0)
        {
            double r, g, b;
            ToLinearSRGB(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "HSL") == 0)
        {
            double h, s, l;
            ToHSL(h, s, l);
            replaceFormatter("{H", h);
            replaceFormatter("{S", s);
            replaceFormatter("{L", l);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "HSV") == 0)
        {
            double h, s, v;
            ToHSV(h, s, v);
            replaceFormatter("{H", h);
            replaceFormatter("{S", s);
            replaceFormatter("{V", v);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "HSI") == 0)
        {
            double h, s, i;
            ToHSI(h, s, i);
            replaceFormatter("{H", h);
            replaceFormatter("{S", s);
            replaceFormatter("{I", i);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "HWB") == 0)
        {
            double h, w, b;
            ToHWB(h, w, b);
            replaceFormatter("{H", h);
            replaceFormatter("{W", w);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "CMYK") == 0)
        {
            double c, m, y, k;
            ToCMYK(c, m, y, k);
            replaceFormatter("{C", c);
            replaceFormatter("{M", m);
            replaceFormatter("{Y", y);
            replaceFormatter("{K", k);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "XYZ") == 0)
        {
            double x, y, z;
             ToXYZ_D65(x, y, z);
            replaceFormatter("{X", x);
            replaceFormatter("{Y", y);
            replaceFormatter("{Z", z);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "Lab") == 0)
        {
            double l, a, b;
            ToLab(l, a, b);
            replaceFormatter("{L", l);
            replaceFormatter("{A", a);
            replaceFormatter("{B", b);
            replaceFormatter("{T", static_cast<double>(a));
        }
        else if (strcmp(type, "YIQ") == 0)
        {
            double y, i, q;
            ToYIQ(y, i, q);
            replaceFormatter("{Y", y);
            replaceFormatter("{I", i);
            replaceFormatter("{Q", q);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "YPbPr") == 0)
        {
            double y, cb, cr;
            ToYPbPr(y, cb, cr);
            replaceFormatter("{Y", y);
            replaceFormatter("{Cb", cb);
            replaceFormatter("{Cr", cr);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "LCHab") == 0)
        {
            double l, c, h;
            ToLCHab(l, c, h);
            replaceFormatter("{L", l);
            replaceFormatter("{C", c);
            replaceFormatter("{H", h);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "LCHuv") == 0)
        {
            double l, c, h;
            ToLCHuv(l, c, h);
            replaceFormatter("{L", l);
            replaceFormatter("{C", c);
            replaceFormatter("{H", h);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "AdobeRGB") == 0)
        {
            double r, g, b;
            ToAdobeRGB(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "ProPhotoRGB") == 0)
        {
            double r, g, b;
            ToProPhotoRGB(r, g, b);
            replaceFormatter("{R", r);
            replaceFormatter("{G", g);
            replaceFormatter("{B", b);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "Luv") == 0)
        {
            double L, u, v;
            ToLuv(L, u, v);
            replaceFormatter("{L", L);
            replaceFormatter("{U", u);
            replaceFormatter("{V", v);
            replaceFormatter("{A", static_cast<double>(a));
        }
        else if (strcmp(type, "Hex") == 0)
        {
            std::string a, r, g, b;
            ToHex(a, r, g, b);
            result = Color::ReplaceAll(result, "{A}", a);
            result = Color::ReplaceAll(result, "{R}", r);
            result = Color::ReplaceAll(result, "{G}", g);
            result = Color::ReplaceAll(result, "{B}", b);
        }
        else if (strcmp(type, "NCol") == 0)
        {
            std::string n;
            double c, l;
            ToNCol(n, c, l);
            result = Color::ReplaceAll(result, "{H}", n);
            replaceFormatter("{W", c);
            replaceFormatter("{B", (100 - l));
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

extern "C"
{
    using namespace KTLib;

    #pragma region Predefined Colors
    COLOR_API Color* CreateAliceBlueColor() { return new Color(Color::AliceBlue()); }
    COLOR_API Color* CreateAntiqueWhiteColor() { return new Color(Color::AntiqueWhite()); }
    COLOR_API Color* CreateAquaColor() { return new Color(Color::Aqua()); }
    COLOR_API Color* CreateAquamarineColor() { return new Color(Color::Aquamarine()); }
    COLOR_API Color* CreateAzureColor() { return new Color(Color::Azure()); }
    COLOR_API Color* CreateBeigeColor() { return new Color(Color::Beige()); }
    COLOR_API Color* CreateBisqueColor() { return new Color(Color::Bisque()); }
    COLOR_API Color* CreateBlackColor() { return new Color(Color::Black()); }
    COLOR_API Color* CreateBlackTransparentColor() { return new Color(Color::BlackTransparent()); }
    COLOR_API Color* CreateBlanchedAlmondColor() { return new Color(Color::BlanchedAlmond()); }
    COLOR_API Color* CreateBlueColor() { return new Color(Color::Blue()); }
    COLOR_API Color* CreateBlueVioletColor() { return new Color(Color::BlueViolet()); }
    COLOR_API Color* CreateBrownColor() { return new Color(Color::Brown()); }
    COLOR_API Color* CreateBurlyWoodColor() { return new Color(Color::BurlyWood()); }
    COLOR_API Color* CreateCadetBlueColor() { return new Color(Color::CadetBlue()); }
    COLOR_API Color* CreateChartreuseColor() { return new Color(Color::Chartreuse()); }
    COLOR_API Color* CreateChocolateColor() { return new Color(Color::Chocolate()); }
    COLOR_API Color* CreateCoralColor() { return new Color(Color::Coral()); }
    COLOR_API Color* CreateCornflowerBlueColor() { return new Color(Color::CornflowerBlue()); }
    COLOR_API Color* CreateCornsilkColor() { return new Color(Color::Cornsilk()); }
    COLOR_API Color* CreateCrimsonColor() { return new Color(Color::Crimson()); }
    COLOR_API Color* CreateCyanColor() { return new Color(Color::Cyan()); }
    COLOR_API Color* CreateDarkBlueColor() { return new Color(Color::DarkBlue()); }
    COLOR_API Color* CreateDarkCyanColor() { return new Color(Color::DarkCyan()); }
    COLOR_API Color* CreateDarkGoldenrodColor() { return new Color(Color::DarkGoldenRod()); }
    COLOR_API Color* CreateDarkGrayColor() { return new Color(Color::DarkGray()); }
    COLOR_API Color* CreateDarkGreenColor() { return new Color(Color::DarkGreen()); }
    COLOR_API Color* CreateDarkKhakiColor() { return new Color(Color::DarkKhaki()); }
    COLOR_API Color* CreateDarkMagentaColor() { return new Color(Color::DarkMagenta()); }
    COLOR_API Color* CreateDarkOliveGreenColor() { return new Color(Color::DarkOliveGreen()); }
    COLOR_API Color* CreateDarkOrangeColor() { return new Color(Color::DarkOrange()); }
    COLOR_API Color* CreateDarkOrchidColor() { return new Color(Color::DarkOrchid()); }
    COLOR_API Color* CreateDarkRedColor() { return new Color(Color::DarkRed()); }
    COLOR_API Color* CreateDarkSalmonColor() { return new Color(Color::DarkSalmon()); }
    COLOR_API Color* CreateDarkSeaGreenColor() { return new Color(Color::DarkSeaGreen()); }
    COLOR_API Color* CreateDarkSlateBlueColor() { return new Color(Color::DarkSlateBlue()); }
    COLOR_API Color* CreateDarkSlateGrayColor() { return new Color(Color::DarkSlateGray()); }
    COLOR_API Color* CreateDarkTurquoiseColor() { return new Color(Color::DarkTurquoise()); }
    COLOR_API Color* CreateDarkVioletColor() { return new Color(Color::DarkViolet()); }
    COLOR_API Color* CreateDeepPinkColor() { return new Color(Color::DeepPink()); }
    COLOR_API Color* CreateDeepSkyBlueColor() { return new Color(Color::DeepSkyBlue()); }
    COLOR_API Color* CreateDimGrayColor() { return new Color(Color::DimGray()); }
    COLOR_API Color* CreateDodgerBlueColor() { return new Color(Color::DodgerBlue()); }
    COLOR_API Color* CreateFireBrickColor() { return new Color(Color::FireBrick()); }
    COLOR_API Color* CreateFloralWhiteColor() { return new Color(Color::FloralWhite()); }
    COLOR_API Color* CreateForestGreenColor() { return new Color(Color::ForestGreen()); }
    COLOR_API Color* CreateFuchsiaColor() { return new Color(Color::Fuchsia()); }
    COLOR_API Color* CreateGainsboroColor() { return new Color(Color::Gainsboro()); }
    COLOR_API Color* CreateGhostWhiteColor() { return new Color(Color::GhostWhite()); }
    COLOR_API Color* CreateGoldColor() { return new Color(Color::Gold()); }
    COLOR_API Color* CreateGoldenRodColor() { return new Color(Color::GoldenRod()); }
    COLOR_API Color* CreateGrayColor() { return new Color(Color::Gray()); }
    COLOR_API Color* CreateGreenColor() { return new Color(Color::Green()); }
    COLOR_API Color* CreateGreenYellowColor() { return new Color(Color::GreenYellow()); }
    COLOR_API Color* CreateHoneydewColor() { return new Color(Color::Honeydew()); }
    COLOR_API Color* CreateHotPinkColor() { return new Color(Color::HotPink()); }
    COLOR_API Color* CreateIndianRedColor() { return new Color(Color::IndianRed()); }
    COLOR_API Color* CreateIndigoColor() { return new Color(Color::Indigo()); }
    COLOR_API Color* CreateIvoryColor() { return new Color(Color::Ivory()); }
    COLOR_API Color* CreateKhakiColor() { return new Color(Color::Khaki()); }
    COLOR_API Color* CreateLavenderBlushColor() { return new Color(Color::LavenderBlush()); }
    COLOR_API Color* CreateLavenderColor() { return new Color(Color::Lavender()); }
    COLOR_API Color* CreateLawnGreenColor() { return new Color(Color::LawnGreen()); }
    COLOR_API Color* CreateLemonChiffonColor() { return new Color(Color::LemonChiffon()); }
    COLOR_API Color* CreateLightBlueColor() { return new Color(Color::LightBlue()); }
    COLOR_API Color* CreateLightCoralColor() { return new Color(Color::LightCoral()); }
    COLOR_API Color* CreateLightCyanColor() { return new Color(Color::LightCyan()); }
    COLOR_API Color* CreateLightGoldenrodYellowColor() { return new Color(Color::LightGoldenrodYellow()); }
    COLOR_API Color* CreateLightGrayColor() { return new Color(Color::LightGray()); }
    COLOR_API Color* CreateLightGreenColor() { return new Color(Color::LightGreen()); }
    COLOR_API Color* CreateLightGreyColor() { return new Color(Color::LightGrey()); }
    COLOR_API Color* CreateLightPinkColor() { return new Color(Color::LightPink()); }
    COLOR_API Color* CreateLightSalmonColor() { return new Color(Color::LightSalmon()); }
    COLOR_API Color* CreateLightSeaGreenColor() { return new Color(Color::LightSeaGreen()); }
    COLOR_API Color* CreateLightSkyBlueColor() { return new Color(Color::LightSkyBlue()); }
    COLOR_API Color* CreateLightSlateGrayColor() { return new Color(Color::LightSlateGray()); }
    COLOR_API Color* CreateLightSteelBlueColor() { return new Color(Color::LightSteelBlue()); }
    COLOR_API Color* CreateLightYellowColor() { return new Color(Color::LightYellow()); }
    COLOR_API Color* CreateLimeColor() { return new Color(Color::Lime()); }
    COLOR_API Color* CreateLimeGreenColor() { return new Color(Color::LimeGreen()); }
    COLOR_API Color* CreateLinenColor() { return new Color(Color::Linen()); }
    COLOR_API Color* CreateMagentaColor() { return new Color(Color::Magenta()); }
    COLOR_API Color* CreateMaroonColor() { return new Color(Color::Maroon()); }
    COLOR_API Color* CreateMediumAquamarineColor() { return new Color(Color::MediumAquamarine()); }
    COLOR_API Color* CreateMediumBlueColor() { return new Color(Color::MediumBlue()); }
    COLOR_API Color* CreateMediumOrchidColor() { return new Color(Color::MediumOrchid()); }
    COLOR_API Color* CreateMediumPurpleColor() { return new Color(Color::MediumPurple()); }
    COLOR_API Color* CreateMediumSeaGreenColor() { return new Color(Color::MediumSeaGreen()); }
    COLOR_API Color* CreateMediumSlateBlueColor() { return new Color(Color::MediumSlateBlue()); }
    COLOR_API Color* CreateMediumSpringGreenColor() { return new Color(Color::MediumSpringGreen()); }
    COLOR_API Color* CreateMediumTurquoiseColor() { return new Color(Color::MediumTurquoise()); }
    COLOR_API Color* CreateMediumVioletRedColor() { return new Color(Color::MediumVioletRed()); }
    COLOR_API Color* CreateMidnightBlueColor() { return new Color(Color::MidnightBlue()); }
    COLOR_API Color* CreateMintCreamColor() { return new Color(Color::MintCream()); }
    COLOR_API Color* CreateMistyRoseColor() { return new Color(Color::MistyRose()); }
    COLOR_API Color* CreateMoccasinColor() { return new Color(Color::Moccasin()); }
    COLOR_API Color* CreateNavajoWhiteColor() { return new Color(Color::NavajoWhite()); }
    COLOR_API Color* CreateNavyColor() { return new Color(Color::Navy()); }
    COLOR_API Color* CreateOldLaceColor() { return new Color(Color::OldLace()); }
    COLOR_API Color* CreateOliveColor() { return new Color(Color::Olive()); }
    COLOR_API Color* CreateOliveDrabColor() { return new Color(Color::OliveDrab()); }
    COLOR_API Color* CreateOrangeColor() { return new Color(Color::Orange()); }
    COLOR_API Color* CreateOrangeRedColor() { return new Color(Color::OrangeRed()); }
    COLOR_API Color* CreateOrchidColor() { return new Color(Color::Orchid()); }
    COLOR_API Color* CreatePaleGoldenrodColor() { return new Color(Color::PaleGoldenrod()); }
    COLOR_API Color* CreatePaleGreenColor() { return new Color(Color::PaleGreen()); }
    COLOR_API Color* CreatePaleTurquoiseColor() { return new Color(Color::PaleTurquoise()); }
    COLOR_API Color* CreatePaleVioletRedColor() { return new Color(Color::PaleVioletRed()); }
    COLOR_API Color* CreatePapayaWhipColor() { return new Color(Color::PapayaWhip()); }
    COLOR_API Color* CreatePeachPuffColor() { return new Color(Color::PeachPuff()); }
    COLOR_API Color* CreatePeruColor() { return new Color(Color::Peru()); }
    COLOR_API Color* CreatePinkColor() { return new Color(Color::Pink()); }
    COLOR_API Color* CreatePlumColor() { return new Color(Color::Plum()); }
    COLOR_API Color* CreatePowderBlueColor() { return new Color(Color::PowderBlue()); }
    COLOR_API Color* CreatePurpleColor() { return new Color(Color::Purple()); }
    COLOR_API Color* CreateRebeccaPurpleColor() { return new Color(Color::RebeccaPurple()); }
    COLOR_API Color* CreateRedColor() { return new Color(Color::Red()); }
    COLOR_API Color* CreateRosyBrownColor() { return new Color(Color::RosyBrown()); }
    COLOR_API Color* CreateRoyalBlueColor() { return new Color(Color::RoyalBlue()); }
    COLOR_API Color* CreateSaddleBrownColor() { return new Color(Color::SaddleBrown()); }
    COLOR_API Color* CreateSalmonColor() { return new Color(Color::Salmon()); }
    COLOR_API Color* CreateSandyBrownColor() { return new Color(Color::SandyBrown()); }
    COLOR_API Color* CreateSeaGreenColor() { return new Color(Color::SeaGreen()); }
    COLOR_API Color* CreateSeashellColor() { return new Color(Color::Seashell()); }
    COLOR_API Color* CreateSiennaColor() { return new Color(Color::Sienna()); }
    COLOR_API Color* CreateSilverColor() { return new Color(Color::Silver()); }
    COLOR_API Color* CreateSkyBlueColor() { return new Color(Color::SkyBlue()); }
    COLOR_API Color* CreateSlateBlueColor() { return new Color(Color::SlateBlue()); }
    COLOR_API Color* CreateSlateGrayColor() { return new Color(Color::SlateGray()); }
    COLOR_API Color* CreateSnowColor() { return new Color(Color::Snow()); }
    COLOR_API Color* CreateSpringGreenColor() { return new Color(Color::SpringGreen()); }
    COLOR_API Color* CreateSteelBlueColor() { return new Color(Color::SteelBlue()); }
    COLOR_API Color* CreateTanColor() { return new Color(Color::Tan()); }
    COLOR_API Color* CreateTealColor() { return new Color(Color::Teal()); }
    COLOR_API Color* CreateThistleColor() { return new Color(Color::Thistle()); }
    COLOR_API Color* CreateTomatoColor() { return new Color(Color::Tomato()); }
    COLOR_API Color* CreateTransparentColor() { return new Color(Color::Transparent()); }
    COLOR_API Color* CreateTurquoiseColor() { return new Color(Color::Turquoise()); }
    COLOR_API Color* CreateVioletColor() { return new Color(Color::Violet()); }
    COLOR_API Color* CreateWheatColor() { return new Color(Color::Wheat()); }
    COLOR_API Color* CreateWhiteColor() { return new Color(Color::White()); }
    COLOR_API Color* CreateWhiteSmokeColor() { return new Color(Color::WhiteSmoke()); }
    COLOR_API Color* CreateWhiteTransparentColor() { return new Color(Color::WhiteTransparent()); }
    COLOR_API Color* CreateYellowColor() { return new Color(Color::Yellow()); }
    COLOR_API Color* CreateYellowGreenColor() { return new Color(Color::YellowGreen()); }
    #pragma endregion

    #pragma region Conversion Methods
    COLOR_API void ColorToRGB(Color* color, int* r, int* g, int* b, int* a) { color->ToRGB(*r, *g, *b, *a); }

    COLOR_API Color* ColorFromHSI(double h, double s, double i, double a) { return new Color(Color::FromHSI(h, s, i, a)); }
    COLOR_API void ColorToHSI(Color* color, double* h, double* s, double* i, int* a)
    {
        color->ToHSI(*h, *s, *i);
        *a = color->a;
    }

    COLOR_API Color* ColorFromHSL(double h, double s, double l, int a) { return new Color(Color::FromHSL(h, s, l, a)); }
    COLOR_API void ColorToHSL(Color* color, double* h, double* s, double* l, int* a)
    {
        color->ToHSL(*h, *s, *l);
        *a = color->a;
    }

    COLOR_API Color* ColorFromHWB(double h, double w, double b, double a) { return new Color(Color::FromHWB(h, w, b, a)); }
    COLOR_API void ColorToHWB(Color* color, double* h, double* w, double* b, int* a)
    {
        color->ToHWB(*h, *w, *b);
        *a = color->a;
    }

    COLOR_API Color* ColorFromCMYK(double c, double m, double y, double k, double a) { return new Color(Color::FromCMYK(c, m, y, k, a)); }
    COLOR_API void ColorToCMYK(Color* color, double* c, double* m, double* y, double* k, int* a)
    {
        color->ToCMYK(*c, *m, *y, *k);
        *a = color->a;
    }

    COLOR_API Color* ColorFromXYZ_D50(double x, double y, double z, double a) { return new Color(Color::FromXYZ_D50(x, y, z, a)); }
    COLOR_API void ColorToXYZ_D50(Color* color, double* x, double* y, double* z, int* a)
    {
        color->ToXYZ_D50(*x, *y, *z);
        *a = color->a;
    }

    COLOR_API Color* ColorFromXYZ_D65(double x, double y, double z, double a) { return new Color(Color::FromXYZ_D65(x, y, z, a)); }
    COLOR_API void ColorToXYZ_D65(Color* color, double* x, double* y, double* z, int* a)
    {
        color-> ToXYZ_D65(*x, *y, *z);
        *a = color->a;
    }

    COLOR_API Color* ColorFromLab(double l, double a, double b, double alpha) { return new Color(Color::FromLab(l, a, b, alpha)); }
    COLOR_API void ColorToLab(Color* color, double* l, double* a, double* b, int* t)
    {
        color->ToLab(*l, *a, *b);
        *t = color->a;
    }

    COLOR_API Color* ColorFromYIQ(double y, double i, double q, double a) { return new Color(Color::FromYIQ(y, i, q, a)); }
    COLOR_API void ColorToYIQ(Color* color, double* y, double* i, double* q, int* a)
    {
        color->ToYIQ(*y, *i, *q);
        *a = color->a;
    }

    COLOR_API Color* ColorFromNCol(const char* h, double w, double b, int a) { return new Color(Color::FromNCol(std::string(h), w, b, a)); }
    COLOR_API void ColorToNCol(Color* color, char* n, double* c, double* l, int* a)
    {
        std::string nStr;
        color->ToNCol(nStr, *c, *l);
        *a = color->a;

        strncpy(n, nStr.c_str(), 15);
        n[15] = '\0';
    }

    COLOR_API Color* ColorFromTemp(double kelvin) { return new Color(Color::FromTemp(kelvin)); }

    COLOR_API void ColorToHex(Color* color, char* a, char* r, char* g, char* b)
    {
        std::string aStr, rStr, gStr, bStr;
        color->ToHex(aStr, rStr, gStr, bStr);

        strncpy(a, aStr.c_str(), 2);
        a[2] = '\0';
        strncpy(r, rStr.c_str(), 2);
        r[2] = '\0';
        strncpy(g, gStr.c_str(), 2);
        g[2] = '\0';
        strncpy(b, bStr.c_str(), 2);
        b[2] = '\0';
    }

    COLOR_API Color* ColorFromLinearSRGB(double r, double g, double b, int a) { return new Color(Color::FromLinearSRGB(r, g, b, a)); }
    COLOR_API void ColorToLinearSRGB(Color* color, double* r, double* g, double* b, int* a)
    {
        color->ToLinearSRGB(*r, *g, *b);
        *a = color->a;
    }

    COLOR_API Color* ColorFromHSV(double h, double s, double v, int a) { return new Color(Color::FromHSV(h, s, v, a)); }
    COLOR_API void ColorToHSV(Color* color, double* h, double* s, double* v, int* a)
    {
        color->ToHSV(*h, *s, *v);
        *a = color->a;
    }

    COLOR_API Color* ColorFromYPbPr(double y, double cb, double cr, int a) { return new Color(Color::FromYPbPr(y, cb, cr, a)); }
    COLOR_API void ColorToYPbPr(Color* color, double* y, double* cb, double* cr, int* a)
    {
        color->ToYPbPr(*y, *cb, *cr);
        *a = color->a;
    }

    COLOR_API Color* ColorFromLCHab(double l, double c, double h, int a) { return new Color(Color::FromLCHab(l, c, h, a)); }
    COLOR_API void ColorToLCHab(Color* color, double* l, double* c, double* h, int* a)
    {
        color->ToLCHab(*l, *c, *h);
        *a = color->a;
    }

    COLOR_API Color* ColorFromLCHuv(double l, double c, double h, int a) { return new Color(Color::FromLCHuv(l, c, h, a)); }
    COLOR_API void ColorToLCHuv(Color* color, double* l, double* c, double* h, int* a)
    {
        color->ToLCHuv(*l, *c, *h);
        *a = color->a;
    }

    COLOR_API Color* ColorFromAdobeRGB(double r, double g, double b, int a) { return new Color(Color::FromAdobeRGB(r, g, b, a)); }
    COLOR_API void ColorToAdobeRGB(Color* color, double* r, double* g, double* b, int* a)
    {
        color->ToAdobeRGB(*r, *g, *b);
        *a = color->a;
    }

    COLOR_API Color* ColorFromProPhotoRGB(double r, double g, double b, int a) { return new Color(Color::FromProPhotoRGB(r, g, b, a)); }
    COLOR_API void ColorToProPhotoRGB(Color* color, double* r, double* g, double* b, int* a)
    {
        color->ToProPhotoRGB(*r, *g, *b);
        *a = color->a;
    }

    COLOR_API Color* ColorFromLuv(double L, double u, double v, int a) { return new Color(Color::FromLuv(L, u, v, a)); }
    COLOR_API void ColorToLuv(Color* color, double* L, double* u, double* v, int* a)
    {
        color->ToLuv(*L, *u, *v);
        *a = color->a;
    }
    #pragma endregion

    #pragma region Color Scheme Generation
    COLOR_API Color** ColorMonochromatic(Color* color, int count)
    {
        auto colors = Color::Monochromatic(color, count);
        Color** result = static_cast<Color**>(malloc(colors.size() * sizeof(Color*)));
        for (size_t i = 0; i < colors.size(); ++i) result[i] = new Color(colors[i]);

        return result;
    }

    COLOR_API Color** ColorAnalogous(Color* color, double angle, int count)
    {
        auto colors = Color::Analogous(color, angle, count);
        Color** result = static_cast<Color**>(malloc(colors.size() * sizeof(Color*)));
        for (size_t i = 0; i < colors.size(); ++i) result[i] = new Color(colors[i]);

        return result;
    }

    COLOR_API Color** ColorTriadic(Color* color, double angle)
    {
        auto colors = Color::Triadic(color, angle);
        Color** result = static_cast<Color**>(malloc(colors.size() * sizeof(Color*)));
        for (size_t i = 0; i < colors.size(); ++i) result[i] = new Color(colors[i]);

        return result;
    }

    COLOR_API Color** ColorTetradic(Color* color, double angle)
    {
        auto colors = Color::Tetradic(color, angle);
        Color** result = static_cast<Color**>(malloc(colors.size() * sizeof(Color*)));
        for (size_t i = 0; i < colors.size(); ++i) result[i] = new Color(colors[i]);

        return result;
    }
    #pragma endregion

    #pragma region Color Manipulation
    COLOR_API void InvertColor(Color* color) { color->Invert(); }
    COLOR_API void GrayscaleColor(Color* color)  { color->Grayscale(); }
    COLOR_API void SepiaColor(Color* color, double factor) { color->Sepia(factor); }
    COLOR_API void CrossProcessColor(Color* color, double factor) { color->CrossProcess(factor); }
    COLOR_API void MoonlightColor(Color* color, double factor) { color->Moonlight(factor); }
    COLOR_API void VintageFilmColor(Color* color, double factor) { color->VintageFilm(factor); }
    COLOR_API void TechnicolorColor(Color* color, double factor) { color->Technicolor(factor); }
    COLOR_API void PolaroidColor(Color* color, double factor) { color->Polaroid(factor); }
    COLOR_API void ComplementColor(Color* color) { color->Complement(); }

    COLOR_API void ShiftHueColor(Color* color, double degrees)       { color->ShiftHue(degrees); }
    COLOR_API void ShiftSaturationColor(Color* color, double amount) { color->ShiftSaturation(amount); }
    COLOR_API void ShiftLightnessColor(Color* color, double amount)  { color->ShiftLightness(amount); }
    COLOR_API void ShiftValueColor(Color* color, double amount)      { color->ShiftValue(amount); }
    COLOR_API void ShiftIntensityColor(Color* color, double amount)  { color->ShiftIntensity(amount); }
    COLOR_API void ShiftWhiteLevelColor(Color* color, double amount) { color->ShiftWhiteLevel(amount); }
    COLOR_API void ShiftBlackLevelColor(Color* color, double amount) { color->ShiftBlackLevel(amount); }
    COLOR_API void ShiftContrastColor(Color* color, double amount)   { color->ShiftContrast(amount); }

    COLOR_API Color* ColorMix(Color* color1, Color* color2, double amount) { return new Color(Color::Mix(*color1, *color2, amount)); }
    COLOR_API Color* ColorScreen(Color* color1, Color* color2) { return new Color(Color::Screen(*color1, *color2)); }
    COLOR_API Color* ColorMultiply(Color* color1, Color* color2) { return new Color(Color::Multiply(*color1, *color2)); }
    COLOR_API Color* ColorOverlay(Color* color1, Color* color2) { return new Color(Color::Overlay(*color1, *color2)); }
    COLOR_API Color* ColorAverage(Color** colors, int count) { return new Color(Color::Average(const_cast<const Color**>(colors), count)); }

    COLOR_API Color* ColorMultiplyMatrix(Color* color,
        double m11, double m12, double m13, double m14, double m15,
        double m21, double m22, double m23, double m24, double m25,
        double m31, double m32, double m33, double m34, double m35,
        double m41, double m42, double m43, double m44, double m45,
        double m51, double m52, double m53, double m54, double m55)
    {
        ColorMatrix matrix;
        // Fill 5x5 matrix using single loop
        double m[25] = {m11,m12,m13,m14,m15, m21,m22,m23,m24,m25, m31,m32,m33,m34,m35, m41,m42,m43,m44,m45, m51,m52,m53,m54,m55};

        for(int i = 0; i < 25; i++) matrix.data[i/5][i%5] = m[i];

        return new Color(*color * matrix);
    }

    COLOR_API Color* ColorAddMatrix(Color* color,
        double m11, double m12, double m13,
        double m21, double m22, double m23,
        double m31, double m32, double m33)
    {
        ColorMatrix matrix;
        matrix.data[0][0] = m11; matrix.data[0][1] = m12; matrix.data[0][2] = m13;
        matrix.data[1][0] = m21; matrix.data[1][1] = m22; matrix.data[1][2] = m23;
        matrix.data[2][0] = m31; matrix.data[2][1] = m32; matrix.data[2][2] = m33;
        return new Color(*color + matrix);
    }

    COLOR_API Color* ColorSubtractMatrix(Color* color,
        double m11, double m12, double m13,
        double m21, double m22, double m23,
        double m31, double m32, double m33)
    {
        ColorMatrix matrix;
        matrix.data[0][0] = m11; matrix.data[0][1] = m12; matrix.data[0][2] = m13;
        matrix.data[1][0] = m21; matrix.data[1][1] = m22; matrix.data[1][2] = m23;
        matrix.data[2][0] = m31; matrix.data[2][1] = m32; matrix.data[2][2] = m33;
        return new Color(*color - matrix);
    }
    #pragma endregion

    #pragma region Utility
    COLOR_API double GetColorLuminance(Color* color) { return color->GetLuminance(); }
    COLOR_API bool IsColorLight(Color* color) { return color->IsLight(); }
    COLOR_API bool IsColorDark(Color* color) { return color->IsDark(); }
    COLOR_API double ColorGetContrast(Color* color1, Color* color2) { return color1->GetContrast(*color2); }
    COLOR_API bool IsColorAccessible(Color* color, Color* background, int level) { return color->IsAccessible(*background, static_cast<Color::AccessibilityLevel>(level)); }
    COLOR_API Color* CreateRandomColor(int alphaRand) { return new Color(Color::Random(alphaRand == 0 ? false : true)); }
    COLOR_API const char* GetColorFormatString(Color* color) { return color->GetFormatString().c_str(); }
    COLOR_API void SetColorFormatString(Color* color, const char* format) { color->SetFormatString(std::string(format)); }

    COLOR_API unsigned int ColorToInt(Color* color, int format) { return color->ToInt(format); }
    COLOR_API int GetColorAlpha(Color* color) { return color->a; }
    COLOR_API void SetColorAlpha(Color* color, int a) { color->SetAlpha(a); }
    COLOR_API int GetColorRed(Color* color) { return color->r; }
    COLOR_API void SetColorRed(Color* color, int r) { color->SetRed(r); }
    COLOR_API int GetColorGreen(Color* color) { return color->g; }
    COLOR_API void SetColorGreen(Color* color, int g) { color->SetGreen(g); }
    COLOR_API int GetColorBlue(Color* color) { return color->b; }
    COLOR_API void SetColorBlue(Color* color, int b) { color->SetBlue(b); }
    COLOR_API void ColorNormalize(Color* color, double* r, double* g, double* b) { color->Normalize(*r, *g, *b); }

    COLOR_API Color* CreateColorFromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { return new Color(r, g, b, a); }
    COLOR_API Color* CreateColorFromInt(unsigned int argb) { return new Color(argb); }
    COLOR_API void DeleteColor(Color* color) { delete color; }

    COLOR_API void ColorToString(Color* color, const char* type, const char* format, char* fullStr)
    {
        std::string result = color->ToString(type, format);
        strncpy(fullStr, result.c_str(), 255);
        fullStr[255] = '\0';
    }

    COLOR_API void FreeColorArray(Color** colors, int count)
    {
        for (int i = 0; i < count; ++i)
            delete colors[i];

        free(colors);
    }

    typedef void (*PixelCallback)(int x, int y, unsigned int color, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    COLOR_API void ProcessImageBuffer(unsigned char* buffer, int width, int height, int stride, PixelCallback callback)
    {
        int totalPixels = width * height;
        for (int i = 0; i < totalPixels; ++i)
        {
            int x = i % width;
            int y = i / width;
            int index = y * stride + x * 4;

            unsigned char b = buffer[index];
            unsigned char g = buffer[index + 1];
            unsigned char r = buffer[index + 2];
            unsigned char a = buffer[index + 3];

            unsigned int color = (a << 24) | (r << 16) | (g << 8) | b;

            callback(x, y, color, r, g, b, a);
        }
    }
    #pragma endregion
}