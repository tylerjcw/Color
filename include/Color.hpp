#pragma once

#include "Constants.h"

#include <unordered_set>
#include <functional>
#include <algorithm>
#include <windows.h>
#include <gdiplus.h>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

namespace KTLib
{
    struct ColorMatrix
    {
        double data[5][5];

        const double* operator[](int index) const { return data[index]; }

        ColorMatrix operator*(double factor) const
        {
            ColorMatrix result;
            #pragma omp parallel for
            for(int i = 0; i < 25; i++) result.data[i/5][i%5] = data[i/5][i%5] * factor;
            return result;
        }

        ColorMatrix operator*(const ColorMatrix& other) const
        {
            ColorMatrix result;
            #pragma omp parallel for collapse(2)
            for(int i = 0; i < 5; i++)
            {
                for(int j = 0; j < 5; j++)
                {
                    result.data[i][j] = 0;
                    for(int k = 0; k < 5; k++) result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
            return result;
        }

        ColorMatrix operator+(const ColorMatrix& other) const
        {
            ColorMatrix result;
            #pragma omp parallel for
            for(int i = 0; i < 25; i++) result.data[i/5][i%5] = data[i/5][i%5] + other.data[i/5][i%5];
            return result;
        }

        ColorMatrix operator-(const ColorMatrix& other) const
        {
            ColorMatrix result;
            #pragma omp parallel for
            for(int i = 0; i < 25; i++) result.data[i/5][i%5] = data[i/5][i%5] - other.data[i/5][i%5];
            return result;
        }

        ColorMatrix Transpose() const
        {
            ColorMatrix result;
            #pragma omp parallel for collapse(2)
            for(int i = 0; i < 5; i++)
                for(int j = 0; j < 5; j++)
                    result.data[i][j] = data[j][i];
            return result;
        }

        friend ColorMatrix operator*(double factor, const ColorMatrix& matrix) { return matrix * factor; }
    };

    enum class YCbCrType
    {
        BT601,
        BT709,
        BT2020
    };

    class Color
    {
        private:
            std::string formatString;
            std::string typeString;
        public:
            union
            {
                uint32_t argb;
                struct { uint8_t b, g, r, a; };
            };

            enum class AccessibilityLevel
            {
                AA,
                AAA
            };

            Color() : argb(0) { }
            Color(uint32_t argb) : argb(argb) { }
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : b(b), g(g), r(r), a(a) { }

            template<typename T> T Convert() const { return T(*this); }
            static constexpr double DefaultGamma = 2.2;
            double gamma = DefaultGamma;

            Color operator+(int value) const;
            Color operator+(const Color& other) const;
            Color operator+(const ColorMatrix& matrix) const;
            Color& operator+=(int value);
            Color& operator+=(const Color& other);
            Color& operator+=(const ColorMatrix& matrix);
            Color operator-(int value) const;
            Color operator-(const Color& other) const;
            Color operator-(const ColorMatrix& matrix) const;
            Color& operator-=(int value);
            Color& operator-=(const Color& other);
            Color& operator-=(const ColorMatrix& matrix);
            Color operator*(double factor) const;
            Color operator*(const Color& other) const;
            Color operator*(const ColorMatrix& matrix) const;
            Color& operator*=(double factor);
            Color& operator*=(const Color& other);
            Color& operator*=(const ColorMatrix& matrix);
            Color operator/(double factor) const;
            Color operator/(const Color& other) const;
            Color& operator/=(double factor);
            Color& operator/=(const Color& other);
            Color operator&(const double& value) const;
            Color operator&(const Color& other) const;
            Color& operator&=(const double& value);
            Color& operator&=(const Color& other);
            Color operator-() const { return Color(255 - r, 255 - g, 255 - b, a); }

            bool operator==(const Color& other) const { return argb == other.argb; }
            bool operator!=(const Color& other) const { return argb != other.argb; }
            bool operator<(const Color& other) const { return argb < other.argb; }
            bool operator>(const Color& other) const { return argb > other.argb; }
            bool operator<=(const Color& other) const { return argb <= other.argb; }
            bool operator>=(const Color& other) const { return argb >= other.argb; }

            friend std::ostream& operator<<(std::ostream& os, const Color& color) { return os << std::to_string(color.ToInt()); }

            int ToInt(int format = 0) const;
            uint64_t GetARGB() const { return argb; }
            void SetARGB(int newARGB) { argb = static_cast<uint64_t>(newARGB); }
            int GetAlpha() const { return a; }
            void SetAlpha(int alpha) { a = static_cast<uint8_t>(alpha); }
            void ShiftAlpha(int delta) { a = std::clamp(a + delta, 0, 255); }
            int GetRed() const { return r; }
            void SetRed(int red) { r = static_cast<uint8_t>(red); }
            void ShiftRed(int delta) { r = std::clamp(r + delta, 0, 255); }
            int GetGreen() const { return g; }
            void SetGreen(int green) { g = static_cast<uint8_t>(green); }
            void ShiftGreen(int delta) { g = std::clamp(g + delta, 0, 255); }
            int GetBlue() const { return b; }
            void SetBlue(int blue) { b = static_cast<uint8_t>(blue); }
            void ShiftBlue(int delta) { b = std::clamp(b + delta, 0, 255); }

            void Normalize(double& outR, double& outG, double& outB) const { std::tie(outR, outG, outB) = std::tuple{r / 255.0, g / 255.0, b / 255.0}; }

            static Color Random(bool alpharand = false);

            void Invert() { *this = -*this; }
            bool IsLight() const { return GetLuminance() > 0.5; }
            bool IsDark() const { return GetLuminance() <= 0.5; }
            const std::string& GetFormatString() const { return formatString; }
            void SetFormatString(const std::string& format) { formatString = format; }
            const std::string& GetTypeString() const { return typeString; }
            void SetTypeString(const std::string& type) { typeString = type; }
            std::string ToString(const char* type = "", const std::string& format = "") const;
            static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
            static std::string GetDefaultFormat(const char* type);

            void ToHex(std::string& outA, std::string& outR, std::string& outG, std::string& outB) const;
            void ToRGB(int& outR, int& outG, int& outB, int& outA) const;

            COLORREF ToCOLORREF() const { return RGB(r, g, b); }
            static Color FromCOLORREF(COLORREF colorref)  { return Color(GetRValue(colorref), GetGValue(colorref), GetBValue(colorref), 255); }

            Gdiplus::Color ToGdipColor() const  { return Gdiplus::Color(a, r, g, b); }
            static Color FromGdipColor(const Gdiplus::Color& color)  { return Color(color.GetR(), color.GetG(), color.GetB(), color.GetA()); }

            double GetHue() const;
            void ToLinearSRGB(double& outR, double& outG, double& outB) const;
            static Color FromLinearSRGB(double r, double g, double b, int a = 255);
            void ToProPhotoRGB(double& outR, double& outG, double& outB) const;
            static Color FromProPhotoRGB(double r, double g, double b, int a = 255);
            void ToAdobeRGB(double& outR, double& outG, double& outB) const;
            static Color FromAdobeRGB(double r, double g, double b, int a = 255);
            void ToHSL(double& h, double& s, double& l) const;
            static Color FromHSL(double h, double s, double l, int a = 255);
            void ToHSV(double& h, double& s, double& v) const;
            static Color FromHSV(double h, double s, double v, int a = 255);
            void ToHSI(double& h, double& s, double& i) const;
            static Color FromHSI(double h, double s, double i, int a = 255);
            void ToHWB(double& h, double& w, double& b) const;
            static Color FromHWB(double h, double w, double b, int a = 255);
            void ToHSP(double& h, double& s, double& p) const;
            static Color FromHSP(double h, double s, double p, int a = 255);
            void ToHCY(double& h, double& c, double& y) const;
            static Color FromHCY(double h, double c, double y, int a = 255);
            void ToHCG(double& h, double& c, double& g) const;
            static Color FromHCG(double h, double c, double g, int a = 255);
            void ToTSL(double& t, double& s, double& l) const;
            static Color FromTSL(double t, double s, double l, int a);
            void ToCMY(double& c, double& m, double& y) const;
            static Color FromCMY(double c, double m, double y, int a = 255);
            void ToCMYK(double& c, double& m, double& y, double& k) const;
            static Color FromCMYK(double c, double m, double y, double k, int a = 255);
            void ToXYZ_D50(double& x, double& y, double& z) const;
            static Color FromXYZ_D50(double x, double y, double z, int a = 255);
            void ToXYZ_D65(double& x, double& y, double& z) const;
            static Color FromXYZ_D65(double x, double y, double z, int a = 255);
            void ToUCS(double& u, double& v, double& w) const;
            static Color FromUCS(double u, double v, double w, int a);
            void ToUVW(double& u, double& v, double& w) const;
            static Color FromUVW(double u, double v, double w, int a);
            void ToXYY(double& x, double& y, double& Y) const;
            static Color FromXYY(double x, double y, double Y, int a = 255);
            void ToLab(double& l, double& a, double& b) const;
            static Color FromLab(double l, double a, double b, int alpha = 255);
            void ToLuv(double& L, double& u, double& v) const;
            static Color FromLuv(double L, double u, double v, int a = 255);
            void ToYIQ(double& y, double& i, double& q) const;
            static Color FromYIQ(double y, double i, double q, int a = 255);
            void ToYPbPr(double& y, double& cb, double& cr) const;
            static Color FromYPbPr(double y, double cb, double cr, int a = 255);
            void ToYCbCr(double& y, double& cb, double& cr, YCbCrType type = YCbCrType::BT709) const;
            static Color FromYCbCr(double y, double cb, double cr, int a = 255, YCbCrType type = YCbCrType::BT709);
            void ToYCgCo(double& y, double& cg, double& co) const;
            static Color FromYCgCo(double y, double cg, double co, int a = 255);
            void ToYDbDr(double& y, double& db, double& dr) const;
            static Color FromYDbDr(double y, double db, double dr, int a = 255);
            void ToYcCbcCrc(double& y, double& cb, double& cr) const;
            static Color FromYcCbcCrc(double y, double cb, double cr, int a = 255);
            void ToYUV(double& y, double& u, double& v) const;
            static Color FromYUV(double y, double u, double v, int a = 255);
            void ToYES(double& y, double& e, double& s) const;
            static Color FromYES(double y, double e, double s, int a = 255);
            void ToJPEG(double& y, double& cb, double& cr) const;
            static Color FromJPEG(double y, double cb, double cr, int a);
            void ToLCHab(double& l, double& c, double& h) const;
            static Color FromLCHab(double l, double c, double h, int a = 255);
            void ToLCHuv(double& l, double& c, double& h) const;
            static Color FromLCHuv(double l, double c, double h, int alpha = 255);
            void ToNCol(std::string& n, double& c, double& l) const;
            static Color FromNCol(const std::string& n, double c, double l, int a);
            void ToRec2020(double& outR, double& outG, double& outB) const;
            static Color FromRec2020(double r, double g, double b, int a = 255);
            void ToDisplayP3(double& outR, double& outG, double& outB) const;
            static Color FromDisplayP3(double r, double g, double b, int a = 255);
            void ToOKLab(double& outL, double& outa, double& outb) const;
            static Color FromOKLab(double l, double a, double b, int alpha = 255);
            void ToOKLCH(double& outL, double& outC, double& outH) const;
            static Color FromOKLCH(double l, double c, double h, int alpha = 255);
            void ToACEScg(double& r, double& g, double& b) const;
            static Color FromACEScg(double r, double g, double b, int a);
            double ToDuv() const;
            double ToTemp() const;
            static Color FromTemp(double temp);

            static std::vector<Color> Monochromatic(const Color* input, int count);
            static std::vector<Color> Analogous(const Color* input, double angle = 30, int count = 3);
            static std::vector<Color> Triadic(const Color* input, double angle = 30);
            static std::vector<Color> Tetradic(const Color* input, double angle = 90);
            static std::vector<Color> Square(const Color* input) { return Tetradic(input, 90); }

            void Complement();

            static Color Mix(const Color& color1, const Color& color2, double weight = 0.5);
            static Color Average(const Color** colors, size_t count);
            static Color Screen(const Color& color1, const Color& color2);
            static Color Multiply(const Color& color1, const Color& color2);
            static Color Overlay(const Color& color1, const Color& color2);

            static void ShiftColorComponent(Color* color, double amount, void (Color::*toFunc)(double&, double&, double&) const, Color (*fromFunc)(double, double, double, int), int componentIndex, double minRange = 0.0, double maxRange = 100.0);

            void   ShiftTemp(double amount);
            void   ShiftHue(double degrees);
            void   ShiftSaturation(double amount);
            void   ShiftLightness(double amount);
            void   ShiftValue(double amount);
            void   ShiftIntensity(double amount);
            void   ShiftPerception(double amount);
            void   ShiftWhiteLevel(double amount);
            void   ShiftBlackLevel(double amount);
            void   ShiftContrast(double amount);
            void   ShiftChroma(double amount);
            void   ShiftLuma(double amount);
            void   ShiftGray(double amount);
            void   Grayscale(double factor = 1);
            void   Sepia(double factor = 1);
            void   CrossProcess(double factor = 1);
            void   Moonlight(double factor = 1);
            void   VintageFilm(double factor = 1);
            void   Technicolor(double factor = 1);
            void   Polaroid(double factor = 1);
            double GetLuminance() const;
            double GetContrast(const Color& other) const;
            bool   IsAccessible(const Color& background, const AccessibilityLevel level = AccessibilityLevel::AA) const;

            static inline Color AliceBlue()            { return Color(0xFFF0F8FF); }
            static inline Color AntiqueWhite()         { return Color(0xFFFAEBD7); }
            static inline Color Aqua()                 { return Color(0xFF00FFFF); }
            static inline Color Aquamarine()           { return Color(0xFF7FFFD4); }
            static inline Color Azure()                { return Color(0xFFF0FFFF); }
            static inline Color Beige()                { return Color(0xFFF5F5DC); }
            static inline Color Bisque()               { return Color(0xFFFFE4C4); }
            static inline Color Black()                { return Color(0xFF000000); }
            static inline Color BlackTransparent()     { return Color(0x00000000); }
            static inline Color BlanchedAlmond()       { return Color(0xFFFFEBCD); }
            static inline Color Blue()                 { return Color(0xFF0000FF); }
            static inline Color BlueViolet()           { return Color(0xFF8A2BE2); }
            static inline Color Brown()                { return Color(0xFFA52A2A); }
            static inline Color BurlyWood()            { return Color(0xFFDEB887); }
            static inline Color CadetBlue()            { return Color(0xFF5F9EA0); }
            static inline Color Chartreuse()           { return Color(0xFF7FFF00); }
            static inline Color Chocolate()            { return Color(0xFFD2691E); }
            static inline Color Coral()                { return Color(0xFFFF7F50); }
            static inline Color CornflowerBlue()       { return Color(0xFF6495ED); }
            static inline Color Cornsilk()             { return Color(0xFFFFF8DC); }
            static inline Color Crimson()              { return Color(0xFFDC143C); }
            static inline Color Cyan()                 { return Color(0xFF00FFFF); }
            static inline Color DarkBlue()             { return Color(0xFF00008B); }
            static inline Color DarkCyan()             { return Color(0xFF008B8B); }
            static inline Color DarkGoldenRod()        { return Color(0xFFB8860B); }
            static inline Color DarkGray()             { return Color(0xFFA9A9A9); }
            static inline Color DarkGrey()             { return Color(0xFFA9A9A9); }
            static inline Color DarkGreen()            { return Color(0xFF006400); }
            static inline Color DarkKhaki()            { return Color(0xFFBDB76B); }
            static inline Color DarkMagenta()          { return Color(0xFF8B008B); }
            static inline Color DarkOliveGreen()       { return Color(0xFF556B2F); }
            static inline Color DarkOrange()           { return Color(0xFFFF8C00); }
            static inline Color DarkOrchid()           { return Color(0xFF9932CC); }
            static inline Color DarkRed()              { return Color(0xFF8B0000); }
            static inline Color DarkSalmon()           { return Color(0xFFE9967A); }
            static inline Color DarkSeaGreen()         { return Color(0xFF8FBC8F); }
            static inline Color DarkSlateBlue()        { return Color(0xFF483D8B); }
            static inline Color DarkSlateGray()        { return Color(0xFF2F4F4F); }
            static inline Color DarkSlateGrey()        { return Color(0xFF2F4F4F); }
            static inline Color DarkTurquoise()        { return Color(0xFF00CED1); }
            static inline Color DarkViolet()           { return Color(0xFF9400D3); }
            static inline Color DeepPink()             { return Color(0xFFFF1493); }
            static inline Color DeepSkyBlue()          { return Color(0xFF00BFFF); }
            static inline Color DimGray()              { return Color(0xFF696969); }
            static inline Color DimGrey()              { return Color(0xFF696969); }
            static inline Color DodgerBlue()           { return Color(0xFF1E90FF); }
            static inline Color FireBrick()            { return Color(0xFFB22222); }
            static inline Color FloralWhite()          { return Color(0xFFFFFAF0); }
            static inline Color ForestGreen()          { return Color(0xFF228B22); }
            static inline Color Fuchsia()              { return Color(0xFFFF00FF); }
            static inline Color Gainsboro()            { return Color(0xFFDCDCDC); }
            static inline Color GhostWhite()           { return Color(0xFFF8F8FF); }
            static inline Color Gold()                 { return Color(0xFFFFD700); }
            static inline Color GoldenRod()            { return Color(0xFFDAA520); }
            static inline Color Gray()                 { return Color(0xFF808080); }
            static inline Color Grey()                 { return Color(0xFF808080); }
            static inline Color Green()                { return Color(0xFF008000); }
            static inline Color GreenYellow()          { return Color(0xFFADFF2F); }
            static inline Color Honeydew()             { return Color(0xFFF0FFF0); }
            static inline Color HotPink()              { return Color(0xFFFF69B4); }
            static inline Color IndianRed()            { return Color(0xFFCD5C5C); }
            static inline Color Indigo()               { return Color(0xFF4B0082); }
            static inline Color Ivory()                { return Color(0xFFFFFFF0); }
            static inline Color Khaki()                { return Color(0xFFF0E68C); }
            static inline Color Lavender()             { return Color(0xFFE6E6FA); }
            static inline Color LavenderBlush()        { return Color(0xFFFFF0F5); }
            static inline Color LawnGreen()            { return Color(0xFF7CFC00); }
            static inline Color LemonChiffon()         { return Color(0xFFFFFACD); }
            static inline Color LightBlue()            { return Color(0xFFADD8E6); }
            static inline Color LightCoral()           { return Color(0xFFF08080); }
            static inline Color LightCyan()            { return Color(0xFFE0FFFF); }
            static inline Color LightGoldenrodYellow() { return Color(0xFFFAFAD2); }
            static inline Color LightGray()            { return Color(0xFFD3D3D3); }
            static inline Color LightGreen()           { return Color(0xFF90EE90); }
            static inline Color LightGrey()            { return Color(0xFFD3D3D3); }
            static inline Color LightPink()            { return Color(0xFFFFB6C1); }
            static inline Color LightSalmon()          { return Color(0xFFFFA07A); }
            static inline Color LightSeaGreen()        { return Color(0xFF20B2AA); }
            static inline Color LightSkyBlue()         { return Color(0xFF87CEFA); }
            static inline Color LightSlateGray()       { return Color(0xFF778899); }
            static inline Color LightSlateGrey()       { return Color(0xFF778899); }
            static inline Color LightSteelBlue()       { return Color(0xFFB0C4DE); }
            static inline Color LightYellow()          { return Color(0xFFFFFFE0); }
            static inline Color Lime()                 { return Color(0xFF00FF00); }
            static inline Color LimeGreen()            { return Color(0xFF32CD32); }
            static inline Color Linen()                { return Color(0xFFFAF0E6); }
            static inline Color Magenta()              { return Color(0xFFFF00FF); }
            static inline Color Maroon()               { return Color(0xFF800000); }
            static inline Color MediumAquamarine()     { return Color(0xFF66CDAA); }
            static inline Color MediumBlue()           { return Color(0xFF0000CD); }
            static inline Color MediumOrchid()         { return Color(0xFFBA55D3); }
            static inline Color MediumPurple()         { return Color(0xFF9370DB); }
            static inline Color MediumSlateGray()      { return Color(0xFF7B68EE); }
            static inline Color MediumSlateGrey()      { return Color(0xFF7B68EE); }
            static inline Color MediumSeaGreen()       { return Color(0xFF3CB371); }
            static inline Color MediumSlateBlue()      { return Color(0xFF7B68EE); }
            static inline Color MediumSpringGreen()    { return Color(0xFF00FA9A); }
            static inline Color MediumTurquoise()      { return Color(0xFF48D1CC); }
            static inline Color MediumVioletRed()      { return Color(0xFFC71585); }
            static inline Color MidnightBlue()         { return Color(0xFF191970); }
            static inline Color MintCream()            { return Color(0xFFF5FFFA); }
            static inline Color MistyRose()            { return Color(0xFFFFE4E1); }
            static inline Color Moccasin()             { return Color(0xFFFFE4B5); }
            static inline Color NavajoWhite()          { return Color(0xFFFFDEAD); }
            static inline Color Navy()                 { return Color(0xFF000080); }
            static inline Color OldLace()              { return Color(0xFFFDF5E6); }
            static inline Color Olive()                { return Color(0xFF808000); }
            static inline Color OliveDrab()            { return Color(0xFF6B8E23); }
            static inline Color Orange()               { return Color(0xFFFFA500); }
            static inline Color OrangeRed()            { return Color(0xFFFF4500); }
            static inline Color Orchid()               { return Color(0xFFDA70D6); }
            static inline Color PaleGoldenrod()        { return Color(0xFFEEE8AA); }
            static inline Color PaleGreen()            { return Color(0xFF98FB98); }
            static inline Color PaleTurquoise()        { return Color(0xFFAFEEEE); }
            static inline Color PaleVioletRed()        { return Color(0xFFDB7093); }
            static inline Color PapayaWhip()           { return Color(0xFFFFEFD5); }
            static inline Color PeachPuff()            { return Color(0xFFFFDAB9); }
            static inline Color Peru()                 { return Color(0xFFCD853F); }
            static inline Color Pink()                 { return Color(0xFFFFC0CB); }
            static inline Color Plum()                 { return Color(0xFFDDA0DD); }
            static inline Color PowderBlue()           { return Color(0xFFB0E0E6); }
            static inline Color Purple()               { return Color(0xFF800080); }
            static inline Color RebeccaPurple()        { return Color(0xFF663399); }
            static inline Color Red()                  { return Color(0xFFFF0000); }
            static inline Color RosyBrown()            { return Color(0xFFBC8F8F); }
            static inline Color RoyalBlue()            { return Color(0xFF4169E1); }
            static inline Color SaddleBrown()          { return Color(0xFF8B4513); }
            static inline Color Salmon()               { return Color(0xFFFA8072); }
            static inline Color SandyBrown()           { return Color(0xFFF4A460); }
            static inline Color SeaGreen()             { return Color(0xFF2E8B57); }
            static inline Color Seashell()             { return Color(0xFFFFF5EE); }
            static inline Color Sienna()               { return Color(0xFFA0522D); }
            static inline Color Silver()               { return Color(0xFFC0C0C0); }
            static inline Color SkyBlue()              { return Color(0xFF87CEEB); }
            static inline Color SlateBlue()            { return Color(0xFF6A5ACD); }
            static inline Color SlateGray()            { return Color(0xFF708090); }
            static inline Color SlateGrey()            { return Color(0xFF708090); }
            static inline Color Snow()                 { return Color(0xFFFFFAFA); }
            static inline Color SpringGreen()          { return Color(0xFF00FF7F); }
            static inline Color SteelBlue()            { return Color(0xFF4682B4); }
            static inline Color Tan()                  { return Color(0xFFD2B48C); }
            static inline Color Teal()                 { return Color(0xFF008080); }
            static inline Color Thistle()              { return Color(0xFFD8BFD8); }
            static inline Color Tomato()               { return Color(0xFFFF6347); }
            static inline Color Transparent()          { return Color(0xFF000000); }
            static inline Color Turquoise()            { return Color(0xFF40E0D0); }
            static inline Color Violet()               { return Color(0xFFEE82EE); }
            static inline Color Wheat()                { return Color(0xFFF5DEB3); }
            static inline Color White()                { return Color(0xFFFFFFFF); }
            static inline Color WhiteSmoke()           { return Color(0xFFF5F5F5); }
            static inline Color WhiteTransparent()     { return Color(0x00FFFFFF); }
            static inline Color Yellow()               { return Color(0xFFFFFF00); }
            static inline Color YellowGreen()          { return Color(0xFF9ACD32); }
    };
}

namespace std
{
    template<>
    struct hash<KTLib::Color>
    {
        template<typename = void>
        size_t operator()(const KTLib::Color& color) const noexcept { return hash<uint32_t>()(color.ToInt());}
    };
}