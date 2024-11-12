#include "../../include/exports/ColorExports.h"

extern "C"
{
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

    COLOR_API Color* ColorFromTemp(double kelvin) { return new Color(Color::FromTemp(kelvin)); }
    COLOR_API double ColorToTemp(Color* color)
    {
        return color->ToTemp();
    }

    COLOR_API double ColorToDuv(Color* color)
    {
        return color->ToDuv();
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
    COLOR_API bool IsColorLight(Color* color) { return color->IsLight() ? TRUE : FALSE; }
    COLOR_API bool IsColorDark(Color* color) { return color->IsDark() ? TRUE : FALSE; }
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