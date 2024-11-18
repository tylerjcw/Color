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

    COLOR_API Color* ColorFromHSP(double h, double s, double p, int a) { return new Color(Color::FromHSP(h, s, p, a)); }
    COLOR_API void ColorToHSP(Color* color, double* h, double* s, double* p, int* a)
    {
        color->ToHSP(*h, *s, *p);
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

    COLOR_API Color* ColorFromRec2020(double r, double g, double b, int a) { return new Color(Color::FromRec2020(r, g, b, a)); }
    COLOR_API void ColorToRec2020(Color* color, double* r, double* g, double* b, int* a)
    {
        color->ToRec2020(*r, *g, *b);
        *a = color->a;
    }

    COLOR_API Color* ColorFromDisplayP3(double r, double g, double b, int a) { return new Color(Color::FromDisplayP3(r, g, b, a)); }
    COLOR_API void ColorToDisplayP3(Color* color, double* r, double* g, double* b, int* a)
    {
        color->ToDisplayP3(*r, *g, *b);
        *a = color->a;
    }

    COLOR_API Color* ColorFromOKLab(double l, double a, double b, int alpha) { return new Color(Color::FromOKLab(l, a, b, alpha)); }
    COLOR_API void ColorToOKLab(Color* color, double* l, double* a, double* b, int* t)
    {
        color->ToOKLab(*l, *a, *b);
        *t = color->a;
    }

    COLOR_API Color* ColorFromOKLCH(double l, double c, double h, int alpha) { return new Color(Color::FromOKLCH(l, c, h, alpha)); }
    COLOR_API void ColorToOKLCH(Color* color, double* l, double* c, double* h, int* a)
    {
        color->ToOKLCH(*l, *c, *h);
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
    COLOR_API Color* ColorApplyMatrix(Color* color, ColorMatrix* matrix) { return new Color(*color * *matrix); }
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
    COLOR_API const char* GetColorTypeString(Color* color) { return color->GetTypeString().c_str(); }
    COLOR_API void SetColorTypeString(Color* color, const char* type) { color->SetTypeString(type); }

    COLOR_API unsigned int ColorToInt(Color* color, int format) { return color->ToInt(format); }
    COLOR_API int GetColorARGB(Color* color) { return color->argb; }
    COLOR_API void SetColorARG(Color* color, int argb) { color->SetARGB(argb); }
    COLOR_API int GetColorAlpha(Color* color) { return color->a; }
    COLOR_API void SetColorAlpha(Color* color, int a) { color->SetAlpha(a); }
    COLOR_API int GetColorRed(Color* color) { return color->r; }
    COLOR_API void SetColorRed(Color* color, int r) { color->SetRed(r); }
    COLOR_API int GetColorGreen(Color* color) { return color->g; }
    COLOR_API void SetColorGreen(Color* color, int g) { color->SetGreen(g); }
    COLOR_API int GetColorBlue(Color* color) { return color->b; }
    COLOR_API void SetColorBlue(Color* color, int b) { color->SetBlue(b); }
    COLOR_API void ColorNormalize(Color* color, double* r, double* g, double* b) { color->Normalize(*r, *g, *b); }

    COLOR_API Color* CreateColorFromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return new Color(r, g, b, a); }
    COLOR_API Color* CreateColorFromInt(uint32_t argb)
    {
        if (argb <= 0xFFFFFF) argb |= 0xFF000000;
        return new Color(argb);
    }

    COLOR_API Color* CreateColorFromHexString(const char* hex)
    {
        std::string hexStr(hex);
        // Remove 0x or # if present
        if (hexStr.substr(0, 2) == "0x") hexStr = hexStr.substr(2);
        else if (hexStr[0] == '#') hexStr = hexStr.substr(1);

        // Handle 6 or 8 character hex strings
        if (hexStr.length() == 6)
            hexStr = "FF" + hexStr;

        return new Color(std::stoul(hexStr, nullptr, 16));
    }

    COLOR_API Color* CreateColorFromName(const char* name)
    {
        std::string colorName(name);
        std::transform(colorName.begin(), colorName.end(), colorName.begin(), ::tolower);

        if (colorName == "aliceblue") return new Color(Color::AliceBlue());
        if (colorName == "antiquewhite") return new Color(Color::AntiqueWhite());
        if (colorName == "aqua") return new Color(Color::Aqua());
        if (colorName == "aquamarine") return new Color(Color::Aquamarine());
        if (colorName == "azure") return new Color(Color::Azure());
        if (colorName == "beige") return new Color(Color::Beige());
        if (colorName == "bisque") return new Color(Color::Bisque());
        if (colorName == "black") return new Color(Color::Black());
        if (colorName == "blacktransparent") return new Color(Color::BlackTransparent());
        if (colorName == "blanchedalmond") return new Color(Color::BlanchedAlmond());
        if (colorName == "blue") return new Color(Color::Blue());
        if (colorName == "blueviolet") return new Color(Color::BlueViolet());
        if (colorName == "brown") return new Color(Color::Brown());
        if (colorName == "burlywood") return new Color(Color::BurlyWood());
        if (colorName == "cadetblue") return new Color(Color::CadetBlue());
        if (colorName == "chartreuse") return new Color(Color::Chartreuse());
        if (colorName == "chocolate") return new Color(Color::Chocolate());
        if (colorName == "coral") return new Color(Color::Coral());
        if (colorName == "cornflowerblue") return new Color(Color::CornflowerBlue());
        if (colorName == "cornsilk") return new Color(Color::Cornsilk());
        if (colorName == "crimson") return new Color(Color::Crimson());
        if (colorName == "cyan") return new Color(Color::Cyan());
        if (colorName == "darkblue") return new Color(Color::DarkBlue());
        if (colorName == "darkcyan") return new Color(Color::DarkCyan());
        if (colorName == "darkgoldenrod") return new Color(Color::DarkGoldenRod());
        if (colorName == "darkgray") return new Color(Color::DarkGray());
        if (colorName == "darkgrey") return new Color(Color::DarkGrey());
        if (colorName == "darkgreen") return new Color(Color::DarkGreen());
        if (colorName == "darkkhaki") return new Color(Color::DarkKhaki());
        if (colorName == "darkmagenta") return new Color(Color::DarkMagenta());
        if (colorName == "darkolivegreen") return new Color(Color::DarkOliveGreen());
        if (colorName == "darkorange") return new Color(Color::DarkOrange());
        if (colorName == "darkorchid") return new Color(Color::DarkOrchid());
        if (colorName == "darkred") return new Color(Color::DarkRed());
        if (colorName == "darksalmon") return new Color(Color::DarkSalmon());
        if (colorName == "darkseagreen") return new Color(Color::DarkSeaGreen());
        if (colorName == "darkslateblue") return new Color(Color::DarkSlateBlue());
        if (colorName == "darkslategray") return new Color(Color::DarkSlateGray());
        if (colorName == "darkslategrey") return new Color(Color::DarkSlateGrey());
        if (colorName == "darkturquoise") return new Color(Color::DarkTurquoise());
        if (colorName == "darkviolet") return new Color(Color::DarkViolet());
        if (colorName == "deeppink") return new Color(Color::DeepPink());
        if (colorName == "deepskyblue") return new Color(Color::DeepSkyBlue());
        if (colorName == "dimgray") return new Color(Color::DimGray());
        if (colorName == "dimgrey") return new Color(Color::DimGrey());
        if (colorName == "dodgerblue") return new Color(Color::DodgerBlue());
        if (colorName == "firebrick") return new Color(Color::FireBrick());
        if (colorName == "floralwhite") return new Color(Color::FloralWhite());
        if (colorName == "forestgreen") return new Color(Color::ForestGreen());
        if (colorName == "fuchsia") return new Color(Color::Fuchsia());
        if (colorName == "gainsboro") return new Color(Color::Gainsboro());
        if (colorName == "ghostwhite") return new Color(Color::GhostWhite());
        if (colorName == "gold") return new Color(Color::Gold());
        if (colorName == "goldenrod") return new Color(Color::GoldenRod());
        if (colorName == "gray") return new Color(Color::Gray());
        if (colorName == "grey") return new Color(Color::Grey());
        if (colorName == "green") return new Color(Color::Green());
        if (colorName == "greenyellow") return new Color(Color::GreenYellow());
        if (colorName == "honeydew") return new Color(Color::Honeydew());
        if (colorName == "hotpink") return new Color(Color::HotPink());
        if (colorName == "indianred") return new Color(Color::IndianRed());
        if (colorName == "indigo") return new Color(Color::Indigo());
        if (colorName == "ivory") return new Color(Color::Ivory());
        if (colorName == "khaki") return new Color(Color::Khaki());
        if (colorName == "lavender") return new Color(Color::Lavender());
        if (colorName == "lavenderblush") return new Color(Color::LavenderBlush());
        if (colorName == "lawngreen") return new Color(Color::LawnGreen());
        if (colorName == "lemonchiffon") return new Color(Color::LemonChiffon());
        if (colorName == "lightblue") return new Color(Color::LightBlue());
        if (colorName == "lightcoral") return new Color(Color::LightCoral());
        if (colorName == "lightcyan") return new Color(Color::LightCyan());
        if (colorName == "lightgoldenrodyellow") return new Color(Color::LightGoldenrodYellow());
        if (colorName == "lightgray") return new Color(Color::LightGray());
        if (colorName == "lightgrey") return new Color(Color::LightGray());
        if (colorName == "lightgreen") return new Color(Color::LightGreen());
        if (colorName == "lightpink") return new Color(Color::LightPink());
        if (colorName == "lightsalmon") return new Color(Color::LightSalmon());
        if (colorName == "lightseagreen") return new Color(Color::LightSeaGreen());
        if (colorName == "lightskyblue") return new Color(Color::LightSkyBlue());
        if (colorName == "lightslategray") return new Color(Color::LightSlateGray());
        if (colorName == "lightslategrey") return new Color(Color::LightSlateGrey());
        if (colorName == "lightsteelblue") return new Color(Color::LightSteelBlue());
        if (colorName == "lightyellow") return new Color(Color::LightYellow());
        if (colorName == "lime") return new Color(Color::Lime());
        if (colorName == "limegreen") return new Color(Color::LimeGreen());
        if (colorName == "linen") return new Color(Color::Linen());
        if (colorName == "magenta") return new Color(Color::Magenta());
        if (colorName == "maroon") return new Color(Color::Maroon());
        if (colorName == "mediumaquamarine") return new Color(Color::MediumAquamarine());
        if (colorName == "mediumblue") return new Color(Color::MediumBlue());
        if (colorName == "mediumorchid") return new Color(Color::MediumOrchid());
        if (colorName == "mediumpurple") return new Color(Color::MediumPurple());
        if (colorName == "mediumseagreen") return new Color(Color::MediumSeaGreen());
        if (colorName == "mediumslategray") return new Color(Color::MediumSlateGray());
        if (colorName == "mediumslategrey") return new Color(Color::MediumSlateGrey());
        if (colorName == "mediumslateblue") return new Color(Color::MediumSlateBlue());
        if (colorName == "mediumspringgreen") return new Color(Color::MediumSpringGreen());
        if (colorName == "mediumturquoise") return new Color(Color::MediumTurquoise());
        if (colorName == "mediumvioletred") return new Color(Color::MediumVioletRed());
        if (colorName == "midnightblue") return new Color(Color::MidnightBlue());
        if (colorName == "mintcream") return new Color(Color::MintCream());
        if (colorName == "mistyrose") return new Color(Color::MistyRose());
        if (colorName == "moccasin") return new Color(Color::Moccasin());
        if (colorName == "navajowhite") return new Color(Color::NavajoWhite());
        if (colorName == "navy") return new Color(Color::Navy());
        if (colorName == "oldlace") return new Color(Color::OldLace());
        if (colorName == "olive") return new Color(Color::Olive());
        if (colorName == "olivedrab") return new Color(Color::OliveDrab());
        if (colorName == "orange") return new Color(Color::Orange());
        if (colorName == "orangered") return new Color(Color::OrangeRed());
        if (colorName == "orchid") return new Color(Color::Orchid());
        if (colorName == "palegoldenrod") return new Color(Color::PaleGoldenrod());
        if (colorName == "palegreen") return new Color(Color::PaleGreen());
        if (colorName == "paleturquoise") return new Color(Color::PaleTurquoise());
        if (colorName == "palevioletred") return new Color(Color::PaleVioletRed());
        if (colorName == "papayawhip") return new Color(Color::PapayaWhip());
        if (colorName == "peachpuff") return new Color(Color::PeachPuff());
        if (colorName == "peru") return new Color(Color::Peru());
        if (colorName == "pink") return new Color(Color::Pink());
        if (colorName == "plum") return new Color(Color::Plum());
        if (colorName == "powderblue") return new Color(Color::PowderBlue());
        if (colorName == "purple") return new Color(Color::Purple());
        if (colorName == "rebeccapurple") return new Color(Color::RebeccaPurple());
        if (colorName == "red") return new Color(Color::Red());
        if (colorName == "rosybrown") return new Color(Color::RosyBrown());
        if (colorName == "royalblue") return new Color(Color::RoyalBlue());
        if (colorName == "saddlebrown") return new Color(Color::SaddleBrown());
        if (colorName == "salmon") return new Color(Color::Salmon());
        if (colorName == "sandybrown") return new Color(Color::SandyBrown());
        if (colorName == "seagreen") return new Color(Color::SeaGreen());
        if (colorName == "sienna") return new Color(Color::Sienna());
        if (colorName == "silver") return new Color(Color::Silver());
        if (colorName == "skyblue") return new Color(Color::SkyBlue());
        if (colorName == "slateblue") return new Color(Color::SlateBlue());
        if (colorName == "slategray") return new Color(Color::SlateGray());
        if (colorName == "slategrey") return new Color(Color::SlateGrey());
        if (colorName == "snow") return new Color(Color::Snow());
        if (colorName == "springgreen") return new Color(Color::SpringGreen());
        if (colorName == "steelblue") return new Color(Color::SteelBlue());
        if (colorName == "tan") return new Color(Color::Tan());
        if (colorName == "teal") return new Color(Color::Teal());
        if (colorName == "thistle") return new Color(Color::Thistle());
        if (colorName == "tomato") return new Color(Color::Tomato());
        if (colorName == "transparent") return new Color(Color::Transparent());
        if (colorName == "turquoise") return new Color(Color::Turquoise());
        if (colorName == "violet") return new Color(Color::Violet());
        if (colorName == "wheat") return new Color(Color::Wheat());
        if (colorName == "white") return new Color(Color::White());
        if (colorName == "whitesmoke") return new Color(Color::WhiteSmoke());
        if (colorName == "whitetransparent") return new Color(Color::WhiteTransparent());
        if (colorName == "yellow") return new Color(Color::Yellow());
        if (colorName == "yellowgreen") return new Color(Color::YellowGreen());
        return nullptr;
    }

    COLOR_API void DeleteColor(Color* color) { delete color; }

    COLOR_API void ColorToString(Color* color, const char* type, const char* format, char* fullStr)
    {
        std::string result = color->ToString(type, format);
        strncpy(fullStr, result.c_str(), 255);
        fullStr[255] = '\0';
    }

    COLOR_API COLORREF ColorToCOLORREF(Color* color) { return color->ToCOLORREF(); }
    COLOR_API Color* ColorFromCOLORREF(COLORREF colorref) { return new Color(Color::FromCOLORREF(colorref)); }

    COLOR_API Gdiplus::Color ColorToGdipColor(Color* color) { return color->ToGdipColor(); }
    COLOR_API Color* ColorFromGdipColor(UINT64 gdipColor) { Gdiplus::Color color(gdipColor); return new Color(Color::FromGdipColor(color)); }

    COLOR_API void FreeColorArray(Color** colors, int count)
    {
        for (int i = 0; i < count; ++i)
            delete colors[i];

        free(colors);
    }

    typedef void (*PixelCallback)(int x, int y, unsigned int color, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    COLOR_API void ProcessImageBuffer(uint8_t* buffer, int width, int height, int stride, PixelCallback callback)
    {
        int totalPixels = width * height;
        for (int i = 0; i < totalPixels; ++i)
        {
            int x = i % width;
            int y = i / width;
            int index = y * stride + x * 4;

            uint8_t b = buffer[index];
            uint8_t g = buffer[index + 1];
            uint8_t r = buffer[index + 2];
            uint8_t a = buffer[index + 3];

            unsigned int color = (a << 24) | (r << 16) | (g << 8) | b;

            callback(x, y, color, r, g, b, a);
        }
    }
    #pragma endregion

    #pragma region ColorMatrix exports
    COLOR_API ColorMatrix* CreateColorMatrix() { return new ColorMatrix(); }
    COLOR_API void DeleteColorMatrix(ColorMatrix* matrix) { delete matrix; }
    COLOR_API ColorMatrix* MultiplyMatrixByScalar(ColorMatrix* matrix, double factor) { return new ColorMatrix(*matrix * factor); }
    COLOR_API ColorMatrix* MultiplyMatrices(ColorMatrix* matrix1, ColorMatrix* matrix2) { return new ColorMatrix(*matrix1 * *matrix2); }
    COLOR_API ColorMatrix* AddMatrices(ColorMatrix* matrix1, ColorMatrix* matrix2) { return new ColorMatrix(*matrix1 + *matrix2); }
    COLOR_API ColorMatrix* SubtractMatrices(ColorMatrix* matrix1, ColorMatrix* matrix2) { return new ColorMatrix(*matrix1 - *matrix2); }
    COLOR_API ColorMatrix* TransposeMatrix(ColorMatrix* matrix) { return new ColorMatrix(matrix->Transpose()); }
    COLOR_API void SetMatrixValue(ColorMatrix* matrix, int row, int col, double value) { matrix->data[row][col] = value; }
    COLOR_API double GetMatrixValue(ColorMatrix* matrix, int row, int col) { return matrix->data[row][col]; }
    #pragma endregion
}