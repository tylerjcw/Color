#define NOMINMAX
#pragma once

#include <functional>
#include <algorithm>
#include <windows.h>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

#pragma region Constants
#define COLOR_API __declspec(dllexport)
#define CONST_PI 3.14159265358979323846

#define ALICE_BLUE             Color(0xFFF0F8FF)
#define ANTIQUE_WHITE          Color(0xFFFAEBD7)
#define AQUA                   Color(0xFF00FFFF)
#define AQUAMARINE             Color(0xFF7FFFD4)
#define AZURE                  Color(0xFFF0FFFF)
#define BEIGE                  Color(0xFFF5F5DC)
#define BISQUE                 Color(0xFFFFE4C4)
#define BLACK                  Color(0xFF000000)
#define BLACK_TRANSPARENT      Color(0x00000000)
#define BLANCHED_ALMOND        Color(0xFFFFEBCD)
#define BLUE                   Color(0xFF0000FF)
#define BLUE_VIOLET            Color(0xFF8A2BE2)
#define BROWN                  Color(0xFFA52A2A)
#define BURLY_WOOD             Color(0xFFDEB887)
#define CADET_BLUE             Color(0xFF5F9EA0)
#define CHARTREUSE             Color(0xFF7FFF00)
#define CHOCOLATE              Color(0xFFD2691E)
#define CORAL                  Color(0xFFFF7F50)
#define CORNFLOWER_BLUE        Color(0xFF6495ED)
#define CORNSILK               Color(0xFFFFF8DC)
#define CRIMSON                Color(0xFFDC143C)
#define CYAN                   Color(0xFF00FFFF)
#define DARK_BLUE              Color(0xFF00008B)
#define DARK_CYAN              Color(0xFF008B8B)
#define DARK_GOLDEN_ROD        Color(0xFFB8860B)
#define DARK_GRAY              Color(0xFFA9A9A9)
#define DARK_GREEN             Color(0xFF006400)
#define DARK_KHAKI             Color(0xFFBDB76B)
#define DARK_MAGENTA           Color(0xFF8B008B)
#define DARK_OLIVE_GREEN       Color(0xFF556B2F)
#define DARK_ORANGE            Color(0xFFFF8C00)
#define DARK_ORCHID            Color(0xFF9932CC)
#define DARK_RED               Color(0xFF8B0000)
#define DARK_SALMON            Color(0xFFE9967A)
#define DARK_SEA_GREEN         Color(0xFF8FBC8F)
#define DARK_SLATE_BLUE        Color(0xFF483D8B)
#define DARK_SLATE_GRAY        Color(0xFF2F4F4F)
#define DARK_TURQUOISE         Color(0xFF00CED1)
#define DARK_VIOLET            Color(0xFF9400D3)
#define DEEP_PINK              Color(0xFFFF1493)
#define DEEP_SKY_BLUE          Color(0xFF00BFFF)
#define DIM_GRAY               Color(0xFF696969)
#define DODGER_BLUE            Color(0xFF1E90FF)
#define FIRE_BRICK             Color(0xFFB22222)
#define FLORAL_WHITE           Color(0xFFFFFAF0)
#define FOREST_GREEN           Color(0xFF228B22)
#define FUCHSIA                Color(0xFFFF00FF)
#define GAINSBORO              Color(0xFFDCDCDC)
#define GHOST_WHITE            Color(0xFFF8F8FF)
#define GOLD                   Color(0xFFFFD700)
#define GOLDEN_ROD             Color(0xFFDAA520)
#define GRAY                   Color(0xFF808080)
#define GREEN                  Color(0xFF008000)
#define GREEN_YELLOW           Color(0xFFADFF2F)
#define HONEYDEW               Color(0xFFF0FFF0)
#define HOT_PINK               Color(0xFFFF69B4)
#define INDIAN_RED             Color(0xFFCD5C5C)
#define INDIGO                 Color(0xFF4B0082)
#define IVORY                  Color(0xFFFFFFF0)
#define KHAKI                  Color(0xFFF0E68C)
#define LAVENDER               Color(0xFFE6E6FA)
#define LAVENDER_BLUSH         Color(0xFFFFF0F5)
#define LAWN_GREEN             Color(0xFF7CFC00)
#define LEMON_CHIFFON          Color(0xFFFFFACD)
#define LIGHT_BLUE             Color(0xFFADD8E6)
#define LIGHT_CORAL            Color(0xFFF08080)
#define LIGHT_CYAN             Color(0xFFE0FFFF)
#define LIGHT_GOLDENROD_YELLOW Color(0xFFFAFAD2)
#define LIGHT_GRAY             Color(0xFFD3D3D3)
#define LIGHT_GREEN            Color(0xFF90EE90)
#define LIGHT_GREY             Color(0xFFD3D3D3)
#define LIGHT_PINK             Color(0xFFFFB6C1)
#define LIGHT_SALMON           Color(0xFFFFA07A)
#define LIGHT_SEA_GREEN        Color(0xFF20B2AA)
#define LIGHT_SKY_BLUE         Color(0xFF87CEFA)
#define LIGHT_SLATE_GRAY       Color(0xFF778899)
#define LIGHT_STEEL_BLUE       Color(0xFFB0C4DE)
#define LIGHT_YELLOW           Color(0xFFFFFFE0)
#define LIME                   Color(0xFF00FF00)
#define LIME_GREEN             Color(0xFF32CD32)
#define LINEN                  Color(0xFFFAF0E6)
#define MAGENTA                Color(0xFFFF00FF)
#define MAROON                 Color(0xFF800000)
#define MEDIUM_AQUAMARINE      Color(0xFF66CDAA)
#define MEDIUM_BLUE            Color(0xFF0000CD)
#define MEDIUM_ORCHID          Color(0xFFBA55D3)
#define MEDIUM_PURPLE          Color(0xFF9370DB)
#define MEDIUM_SEA_GREEN       Color(0xFF3CB371)
#define MEDIUM_SLATE_BLUE      Color(0xFF7B68EE)
#define MEDIUM_SPRING_GREEN    Color(0xFF00FA9A)
#define MEDIUM_TURQUOISE       Color(0xFF48D1CC)
#define MEDIUM_VIOLET_RED      Color(0xFFC71585)
#define MIDNIGHT_BLUE          Color(0xFF191970)
#define MINT_CREAM             Color(0xFFF5FFFA)
#define MISTY_ROSE             Color(0xFFFFE4E1)
#define MOCCASIN               Color(0xFFFFE4B5)
#define NAVAJO_WHITE           Color(0xFFFFDEAD)
#define NAVY                   Color(0xFF000080)
#define OLD_LACE               Color(0xFFFDF5E6)
#define OLIVE                  Color(0xFF808000)
#define OLIVE_DRAB             Color(0xFF6B8E23)
#define ORANGE                 Color(0xFFFFA500)
#define ORANGE_RED             Color(0xFFFF4500)
#define ORCHID                 Color(0xFFDA70D6)
#define PALE_GOLDENROD         Color(0xFFEEE8AA)
#define PALE_GREEN             Color(0xFF98FB98)
#define PALE_TURQUOISE         Color(0xFFAFEEEE)
#define PALE_VIOLET_RED        Color(0xFFDB7093)
#define PAPAYA_WHIP            Color(0xFFFFEFD5)
#define PEACH_PUFF             Color(0xFFFFDAB9)
#define PERU                   Color(0xFFCD853F)
#define PINK                   Color(0xFFFFC0CB)
#define PLUM                   Color(0xFFDDA0DD)
#define POWDER_BLUE            Color(0xFFB0E0E6)
#define PURPLE                 Color(0xFF800080)
#define REBECCA_PURPLE         Color(0xFF663399)
#define RED                    Color(0xFFFF0000)
#define ROSY_BROWN             Color(0xFFBC8F8F)
#define ROYAL_BLUE             Color(0xFF4169E1)
#define SADDLE_BROWN           Color(0xFF8B4513)
#define SALMON                 Color(0xFFFA8072)
#define SANDY_BROWN            Color(0xFFF4A460)
#define SEA_GREEN              Color(0xFF2E8B57)
#define SEASHELL               Color(0xFFFFF5EE)
#define SIENNA                 Color(0xFFA0522D)
#define SILVER                 Color(0xFFC0C0C0)
#define SKY_BLUE               Color(0xFF87CEEB)
#define SLATE_BLUE             Color(0xFF6A5ACD)
#define SLATE_GRAY             Color(0xFF708090)
#define SNOW                   Color(0xFFFFFAFA)
#define SPRING_GREEN           Color(0xFF00FF7F)
#define STEEL_BLUE             Color(0xFF4682B4)
#define TAN                    Color(0xFFD2B48C)
#define TEAL                   Color(0xFF008080)
#define THISTLE                Color(0xFFD8BFD8)
#define TOMATO                 Color(0xFFFF6347)
#define TURQUOISE              Color(0xFF40E0D0)
#define VIOLET                 Color(0xFFEE82EE)
#define WHEAT                  Color(0xFFF5DEB3)
#define WHITE                  Color(0xFFFFFFFF)
#define WHITE_SMOKE            Color(0xFFF5F5F5)
#define WHITE_TRANSPARENT      Color(0x00FFFFFF)
#define YELLOW                 Color(0xFFFFFF00)
#define YELLOW_GREEN           Color(0xFF9ACD32)
#pragma endregion

#pragma region Definitions
namespace KTLib
{
    class Gradient;
    struct ColorMatrix;
    struct GradientPosition;
    struct ColorStop;

    enum class GradientType
    {
        Linear,
        Radial,
        Conical
    };

    class COLOR_API Color
    {
        private:
            std::string formatString;

        public:
            union
            {
                uint32_t argb;
                struct { uint8_t b, g, r, a; };
            };

            Color() : argb(0) {}
            Color(uint32_t argb) : argb(argb) {}
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : b(b), g(g), r(r), a(a) {}

            template<typename T>
            T Convert() const { return T(*this); }

            static constexpr double DefaultGamma = 2.2;
            double gamma = DefaultGamma;

            #pragma region Operators
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

            inline Color operator-() const { return Color(255 - r, 255 - g, 255 - b, a); }

            inline bool operator==(const Color& other) const { return argb == other.argb; }
            inline bool operator!=(const Color& other) const { return argb != other.argb; }
            inline bool operator<(const Color& other) const  { return argb < other.argb;  }
            inline bool operator>(const Color& other) const  { return argb > other.argb;  }
            inline bool operator<=(const Color& other) const { return argb <= other.argb; }
            inline bool operator>=(const Color& other) const { return argb >= other.argb; }
            #pragma endregion

            int ToInt(int format = 0) const;
            inline int GetAlpha() const { return a; }
            inline void SetAlpha(int alpha) { a = static_cast<uint8_t>(alpha); }
            inline void ShiftAlpha(int delta) { a = std::clamp(a + delta, 0, 255); }
            inline int GetRed() const { return r; }
            inline void SetRed(int red) { r = static_cast<uint8_t>(red); }
            inline void ShiftRed(int delta) { r = std::clamp(r + delta, 0, 255); }
            inline int GetGreen() const { return g; }
            inline void SetGreen(int green) { g = static_cast<uint8_t>(green); }
            inline void ShiftGreen(int delta) { g = std::clamp(g + delta, 0, 255); }
            inline int GetBlue() const { return b; }
            inline void SetBlue(int blue) { b = static_cast<uint8_t>(blue); }
            inline void ShiftBlue(int delta) { b = std::clamp(b + delta, 0, 255); }
            inline void Normalize(double& outR, double& outG, double& outB) const { std::tie(outR, outG, outB) = std::tuple{r / 255.0, g / 255.0, b / 255.0}; }

            static Color Random(bool alpharand = false);

            static inline Color AliceBlue()            { return ALICE_BLUE; }
            static inline Color AntiqueWhite()         { return ANTIQUE_WHITE; }
            static inline Color Aqua()                 { return AQUA; }
            static inline Color Aquamarine()           { return AQUAMARINE; }
            static inline Color Azure()                { return AZURE; }
            static inline Color Beige()                { return BEIGE; }
            static inline Color Bisque()               { return BISQUE; }
            static inline Color Black()                { return BLACK; }
            static inline Color BlackTransparent()     { return BLACK_TRANSPARENT; }
            static inline Color BlanchedAlmond()       { return BLANCHED_ALMOND; }
            static inline Color Blue()                 { return BLUE; }
            static inline Color BlueViolet()           { return BLUE_VIOLET; }
            static inline Color Brown()                { return BROWN; }
            static inline Color BurlyWood()            { return BURLY_WOOD; }
            static inline Color CadetBlue()            { return CADET_BLUE; }
            static inline Color Chartreuse()           { return CHARTREUSE; }
            static inline Color Chocolate()            { return CHOCOLATE; }
            static inline Color Coral()                { return CORAL; }
            static inline Color CornflowerBlue()       { return CORNFLOWER_BLUE; }
            static inline Color Cornsilk()             { return CORNSILK; }
            static inline Color Crimson()              { return CRIMSON; }
            static inline Color Cyan()                 { return CYAN; }
            static inline Color DarkBlue()             { return DARK_BLUE; }
            static inline Color DarkCyan()             { return DARK_CYAN; }
            static inline Color DarkGoldenRod()        { return DARK_GOLDEN_ROD; }
            static inline Color DarkGray()             { return DARK_GRAY; }
            static inline Color DarkGreen()            { return DARK_GREEN; }
            static inline Color DarkKhaki()            { return DARK_KHAKI; }
            static inline Color DarkMagenta()          { return DARK_MAGENTA; }
            static inline Color DarkOliveGreen()       { return DARK_OLIVE_GREEN; }
            static inline Color DarkOrange()           { return DARK_ORANGE; }
            static inline Color DarkOrchid()           { return DARK_ORCHID; }
            static inline Color DarkRed()              { return DARK_RED; }
            static inline Color DarkSalmon()           { return DARK_SALMON; }
            static inline Color DarkSeaGreen()         { return DARK_SEA_GREEN; }
            static inline Color DarkSlateBlue()        { return DARK_SLATE_BLUE; }
            static inline Color DarkSlateGray()        { return DARK_SLATE_GRAY; }
            static inline Color DarkTurquoise()        { return DARK_TURQUOISE; }
            static inline Color DarkViolet()           { return DARK_VIOLET; }
            static inline Color DeepPink()             { return DEEP_PINK; }
            static inline Color DeepSkyBlue()          { return DEEP_SKY_BLUE; }
            static inline Color DimGray()              { return DIM_GRAY; }
            static inline Color DodgerBlue()           { return DODGER_BLUE; }
            static inline Color FireBrick()            { return FIRE_BRICK; }
            static inline Color FloralWhite()          { return FLORAL_WHITE; }
            static inline Color ForestGreen()          { return FOREST_GREEN; }
            static inline Color Fuchsia()              { return FUCHSIA; }
            static inline Color Gainsboro()            { return GAINSBORO; }
            static inline Color GhostWhite()           { return GHOST_WHITE; }
            static inline Color Gold()                 { return GOLD; }
            static inline Color GoldenRod()            { return GOLDEN_ROD; }
            static inline Color Gray()                 { return GRAY; }
            static inline Color Green()                { return GREEN; }
            static inline Color GreenYellow()          { return GREEN_YELLOW; }
            static inline Color Honeydew()             { return HONEYDEW; }
            static inline Color HotPink()              { return HOT_PINK; }
            static inline Color IndianRed()            { return INDIAN_RED; }
            static inline Color Indigo()               { return INDIGO; }
            static inline Color Ivory()                { return IVORY; }
            static inline Color Khaki()                { return KHAKI; }
            static inline Color Lavender()             { return LAVENDER; }
            static inline Color LavenderBlush()        { return LAVENDER_BLUSH; }
            static inline Color LawnGreen()            { return LAWN_GREEN; }
            static inline Color LemonChiffon()         { return LEMON_CHIFFON; }
            static inline Color LightBlue()            { return LIGHT_BLUE; }
            static inline Color LightCoral()           { return LIGHT_CORAL; }
            static inline Color LightCyan()            { return LIGHT_CYAN; }
            static inline Color LightGoldenrodYellow() { return LIGHT_GOLDENROD_YELLOW; }
            static inline Color LightGray()            { return LIGHT_GRAY; }
            static inline Color LightGreen()           { return LIGHT_GREEN; }
            static inline Color LightGrey()            { return LIGHT_GREY; }
            static inline Color LightPink()            { return LIGHT_PINK; }
            static inline Color LightSalmon()          { return LIGHT_SALMON; }
            static inline Color LightSeaGreen()        { return LIGHT_SEA_GREEN; }
            static inline Color LightSkyBlue()         { return LIGHT_SKY_BLUE; }
            static inline Color LightSlateGray()       { return LIGHT_SLATE_GRAY; }
            static inline Color LightSteelBlue()       { return LIGHT_STEEL_BLUE; }
            static inline Color LightYellow()          { return LIGHT_YELLOW; }
            static inline Color Lime()                 { return LIME; }
            static inline Color LimeGreen()            { return LIME_GREEN; }
            static inline Color Linen()                { return LINEN; }
            static inline Color Magenta()              { return MAGENTA; }
            static inline Color Maroon()               { return MAROON; }
            static inline Color MediumAquamarine()     { return MEDIUM_AQUAMARINE; }
            static inline Color MediumBlue()           { return MEDIUM_BLUE; }
            static inline Color MediumOrchid()         { return MEDIUM_ORCHID; }
            static inline Color MediumPurple()         { return MEDIUM_PURPLE; }
            static inline Color MediumSeaGreen()       { return MEDIUM_SEA_GREEN; }
            static inline Color MediumSlateBlue()      { return MEDIUM_SLATE_BLUE; }
            static inline Color MediumSpringGreen()    { return MEDIUM_SPRING_GREEN; }
            static inline Color MediumTurquoise()      { return MEDIUM_TURQUOISE; }
            static inline Color MediumVioletRed()      { return MEDIUM_VIOLET_RED; }
            static inline Color MidnightBlue()         { return MIDNIGHT_BLUE; }
            static inline Color MintCream()            { return MINT_CREAM; }
            static inline Color MistyRose()            { return MISTY_ROSE; }
            static inline Color Moccasin()             { return MOCCASIN; }
            static inline Color NavajoWhite()          { return NAVAJO_WHITE; }
            static inline Color Navy()                 { return NAVY; }
            static inline Color OldLace()              { return OLD_LACE; }
            static inline Color Olive()                { return OLIVE; }
            static inline Color OliveDrab()            { return OLIVE_DRAB; }
            static inline Color Orange()               { return ORANGE; }
            static inline Color OrangeRed()            { return ORANGE_RED; }
            static inline Color Orchid()               { return ORCHID; }
            static inline Color PaleGoldenrod()        { return PALE_GOLDENROD; }
            static inline Color PaleGreen()            { return PALE_GREEN; }
            static inline Color PaleTurquoise()        { return PALE_TURQUOISE; }
            static inline Color PaleVioletRed()        { return PALE_VIOLET_RED; }
            static inline Color PapayaWhip()           { return PAPAYA_WHIP; }
            static inline Color PeachPuff()            { return PEACH_PUFF; }
            static inline Color Peru()                 { return PERU; }
            static inline Color Pink()                 { return PINK; }
            static inline Color Plum()                 { return PLUM; }
            static inline Color PowderBlue()           { return POWDER_BLUE; }
            static inline Color Purple()               { return PURPLE; }
            static inline Color RebeccaPurple()        { return REBECCA_PURPLE; }
            static inline Color Red()                  { return RED; }
            static inline Color RosyBrown()            { return ROSY_BROWN; }
            static inline Color RoyalBlue()            { return ROYAL_BLUE; }
            static inline Color SaddleBrown()          { return SADDLE_BROWN; }
            static inline Color Salmon()               { return SALMON; }
            static inline Color SandyBrown()           { return SANDY_BROWN; }
            static inline Color SeaGreen()             { return SEA_GREEN; }
            static inline Color Seashell()             { return SEASHELL; }
            static inline Color Sienna()               { return SIENNA; }
            static inline Color Silver()               { return SILVER; }
            static inline Color SkyBlue()              { return SKY_BLUE; }
            static inline Color SlateBlue()            { return SLATE_BLUE; }
            static inline Color SlateGray()            { return SLATE_GRAY; }
            static inline Color Snow()                 { return SNOW; }
            static inline Color SpringGreen()          { return SPRING_GREEN; }
            static inline Color SteelBlue()            { return STEEL_BLUE; }
            static inline Color Tan()                  { return TAN; }
            static inline Color Teal()                 { return TEAL; }
            static inline Color Thistle()              { return THISTLE; }
            static inline Color Tomato()               { return TOMATO; }
            static inline Color Transparent()          { return BLACK_TRANSPARENT; }
            static inline Color Turquoise()            { return TURQUOISE; }
            static inline Color Violet()               { return VIOLET; }
            static inline Color Wheat()                { return WHEAT; }
            static inline Color White()                { return WHITE; }
            static inline Color WhiteSmoke()           { return WHITE_SMOKE; }
            static inline Color WhiteTransparent()     { return WHITE_TRANSPARENT; }
            static inline Color Yellow()               { return YELLOW; }
            static inline Color YellowGreen()          { return YELLOW_GREEN; }

            inline void Invert()                      { *this = -*this; }
            bool IsLight() const               { return GetLuminance() > 0.5; }
            bool IsDark() const                { return GetLuminance() <= 0.5; }

            inline const std::string& GetFormatString() const { return formatString; }
            inline void SetFormatString(const std::string& format) { formatString = format; }

            std::string ToString(const char* type, const std::string& format = "") const;
            static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
            static std::string GetDefaultFormat(const char* type);
            void ToHex(std::string& outA, std::string& outR, std::string& outG, std::string& outB) const;
            void ToRGB(int& outR, int& outG, int& outB, int& outA) const;

            double GetHue() const;

            // Linear sRGB
            void ToLinearSRGB(double& outR, double& outG, double& outB) const;
            static Color FromLinearSRGB(double r, double g, double b, int a = 255);

            // ProPhoto RGB
            void ToProPhotoRGB(double& outR, double& outG, double& outB) const;
            static Color FromProPhotoRGB(double r, double g, double b, int a = 255);

            //Adobe RGB
            void ToAdobeRGB(double& outR, double& outG, double& outB) const;
            static Color FromAdobeRGB(double r, double g, double b, int a = 255);

            // HSL
            void ToHSL(double& h, double& s, double& l) const;
            static Color FromHSL(double h, double s, double l, int a = 255);

            // HSV
            void ToHSV(double& h, double& s, double& v) const;
            static Color FromHSV(double h, double s, double v, int a = 255);

            // HSI
            void ToHSI(double& h, double& s, double& i) const;
            static Color FromHSI(double h, double s, double i, int a = 255);

            // HWB
            void ToHWB(double& h, double& w, double& b) const;
            static Color FromHWB(double h, double w, double b, int a = 255);

            // CMYK
            void ToCMYK(double& c, double& m, double& y, double& k) const;
            static Color FromCMYK(double c, double m, double y, double k, int a = 255);

            // XYZ
            void ToXYZ_D50(double& x, double& y, double& z) const;
            static Color FromXYZ_D50(double x, double y, double z, int a = 255);

            void ToXYZ_D65(double& x, double& y, double& z) const;
            static Color FromXYZ_D65(double x, double y, double z, int a = 255);

            // CIE Lab
            void ToLab(double& l, double& a, double& b) const;
            static Color FromLab(double l, double a, double b, int alpha = 255);

            // CIE Luv
            void ToLuv(double& L, double& u, double& v) const;
            static Color FromLuv(double L, double u, double v, int a = 255);

            // YIQ
            void ToYIQ(double& y, double& i, double& q) const;
            static Color FromYIQ(double y, double i, double q, int a = 255);

            // YPbPr
            void ToYPbPr(double& y, double& cb, double& cr) const;
            static Color FromYPbPr(double y, double cb, double cr, int a = 255);

            // LCHab
            void ToLCHab(double& l, double& c, double& h) const;
            static Color FromLCHab(double l, double c, double h, int a = 255);

            // LCHuv
            void ToLCHuv(double& l, double& c, double& h) const;
            static Color FromLCHuv(double l, double c, double h, int alpha = 255);

            // NCol
            void ToNCol(std::string& n, double& c, double& l) const;
            static Color FromNCol(const std::string& n, double c, double l, int a);

            // CCT
            double ToTemp() const;
            static Color FromTemp(double temp);
            void ShiftTemp(double amount);

            //Duv
            double ToDuv() const;

            // Color Scheme Generation
            static std::vector<Color> Monochromatic(const Color* input, int count);
            static std::vector<Color> Analogous(const Color* input, double angle = 30, int count = 3);
            static std::vector<Color> Triadic(const Color* input, double angle = 30);
            static std::vector<Color> Tetradic(const Color* input, double angle = 90);
            inline static std::vector<Color> Square(const Color* input) { return Tetradic(input, 90); }
            void Complement();

            // Mix, Average, Multiply
            static Color Mix(const Color& color1, const Color& color2, double weight = 0.5);
            static Color Average(const Color** colors, size_t count);
            static Color Screen(const Color& color1, const Color& color2);
            static Color Multiply(const Color& color1, const Color& color2);
            static Color Overlay(const Color& color1, const Color& color2);

            static void ShiftColorComponent(Color* color, double amount, void (Color::*toFunc)(double&, double&, double&) const, Color (*fromFunc)(double, double, double, int), int componentIndex, bool isHue = false);
            void ShiftHue(double degrees);
            void ShiftSaturation(double amount);
            void ShiftLightness(double amount);
            void ShiftValue(double amount);
            void ShiftIntensity(double amount);
            void ShiftWhiteLevel(double amount);
            void ShiftBlackLevel(double amount);
            void ShiftContrast(double amount);
            void Grayscale();
            void Sepia(double factor);
            void CrossProcess(double factor);
            void Moonlight(double factor);
            void VintageFilm(double factor);
            void Technicolor(double factor);
            void Polaroid(double factor);
            double GetLuminance() const;
            double GetContrast(const Color& other) const;

            // Utility
            enum class AccessibilityLevel
            {
                AA,
                AAA
            };

            bool IsAccessible(const Color& background, const AccessibilityLevel level = AccessibilityLevel::AA) const;
    };

    class COLOR_API ColorBuffer
    {
        public:
            ColorBuffer() = default;
            ColorBuffer(int width, int height, Color col);
            ColorBuffer(unsigned int* buffer, int width, int height);
            ColorBuffer(Color** colors, int width, int height);
            ColorBuffer(const std::vector<std::vector<Color>>& colors);
            ColorBuffer(const Gradient& gradient, int width, int height);

            void Rotate(double angle);
            void Resize(int newWidth, int newHeight, int resizeImage, Color fillColor);
            void Scale(double scale);
            static ColorBuffer* FromHBITMAP(HBITMAP hBitmap, int targetWidth = 0, int targetHeight = 0);
            HBITMAP ToHBITMAP(int targetWidth = 0, int targetHeight = 0) const;

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
            Color CalculateAverageColor(int startX, int startY, int pixelWidth, int pixelHeight);

        private:
            alignas(32) std::vector<Color> m_colors;
            int m_width = 0;
            int m_height = 0;
    };

    class COLOR_API Gradient
    {
        public:
            Gradient(const Gradient& other) : m_totalSteps(other.m_totalSteps), m_type(other.m_type), m_colorStops(other.m_colorStops) {}
            Gradient() : m_totalSteps(2), m_type(KTLib::GradientType::Linear) {}
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
            void AddColorStop(unsigned int color, GradientPosition position);
            void RemoveColorStop(float position);
            Color GetColorAt(GradientPosition position) const;
            Color GetColorAtStep(int step) const;
            void Rotate(float angle);
            void Reverse();
            void Shift(float amount);
            std::string Serialize() const;
            static Gradient Deserialize(const std::string& data);
            HBITMAP CreateHBITMAP(int width, int height) const;
            int GetTotalSteps() const { return m_totalSteps; }
            void SetType(GradientType type) { m_type = type; }
            GradientType GetType() const { return m_type; }
            void SetAngle(float degrees) { m_angle = degrees; }
            float GetAngle() const { return m_angle; }

        private:
            int m_totalSteps;
            GradientType m_type;
            float m_angle = 0.0f;
            std::vector<ColorStop> m_colorStops;
            template<typename Operation>
            void ApplyToAllStops(Operation op);
    };

    struct ColorMatrix
    {
        double data[5][5];

        const double* operator[](int index) const { return data[index]; }

        ColorMatrix operator*(double factor) const
        {
            ColorMatrix result;
            for(int i = 0; i < 25; i++) result.data[i/5][i%5] = data[i/5][i%5] * factor;
            return result;
        }

        friend ColorMatrix operator*(double factor, const ColorMatrix& matrix) { return matrix * factor; }
    };

    struct GradientPosition
    {
        float value;
        static GradientPosition FromNormalized(float v) { return {std::clamp(v, 0.0f, 1.0f)}; }
    };

    struct ColorStop
    {
        unsigned int color;
        GradientPosition position;
    };
}
#pragma endregion