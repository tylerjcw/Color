#pragma once

#include "../Color.hpp"

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
    COLOR_API Color* CreateDarkGreyColor() { return new Color(Color::DarkGrey()); }
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
    COLOR_API Color* CreateDarkSlateGreyColor() { return new Color(Color::DarkSlateGrey()); }
    COLOR_API Color* CreateDarkTurquoiseColor() { return new Color(Color::DarkTurquoise()); }
    COLOR_API Color* CreateDarkVioletColor() { return new Color(Color::DarkViolet()); }
    COLOR_API Color* CreateDeepPinkColor() { return new Color(Color::DeepPink()); }
    COLOR_API Color* CreateDeepSkyBlueColor() { return new Color(Color::DeepSkyBlue()); }
    COLOR_API Color* CreateDimGrayColor() { return new Color(Color::DimGray()); }
    COLOR_API Color* CreateDimGreyColor() { return new Color(Color::DimGrey()); }
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
    COLOR_API Color* CreateGreyColor() { return new Color(Color::Grey()); }
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
    COLOR_API Color* CreateLightGreyColor() { return new Color(Color::LightGrey()); }
    COLOR_API Color* CreateLightGreenColor() { return new Color(Color::LightGreen()); }
    COLOR_API Color* CreateLightPinkColor() { return new Color(Color::LightPink()); }
    COLOR_API Color* CreateLightSalmonColor() { return new Color(Color::LightSalmon()); }
    COLOR_API Color* CreateLightSeaGreenColor() { return new Color(Color::LightSeaGreen()); }
    COLOR_API Color* CreateLightSkyBlueColor() { return new Color(Color::LightSkyBlue()); }
    COLOR_API Color* CreateLightSlateGrayColor() { return new Color(Color::LightSlateGray()); }
    COLOR_API Color* CreateLightSlateGreyColor() { return new Color(Color::LightSlateGrey()); }
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
    COLOR_API Color* CreateMediumSlateGrayColor() { return new Color(Color::MediumSlateGray()); }
    COLOR_API Color* CreateMediumSlateGreyColor() { return new Color(Color::MediumSlateGrey()); }
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
    COLOR_API Color* CreateSlateGreyColor() { return new Color(Color::SlateGrey()); }
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

    #pragma region Core Methods
    COLOR_API Color* CreateColorObject() { return new Color(); }
    COLOR_API void DeleteColorObject(Color* color) { delete color; }
    COLOR_API Color* CreateColorFromARGB(uint8_t argb) { return new Color(argb); }
    COLOR_API Color* CreateColorFromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    COLOR_API Color* CreateColorFromInt(uint32_t argb);
    COLOR_API Color* CreateColorFromHexString(const char* hex);
    COLOR_API Color* CreateColorFromName(const char* name);
    COLOR_API uint32_t ColorToInt(Color* color, int format);
    COLOR_API void ColorNormalize(Color* color, double* r, double* g, double* b);
    #pragma endregion

    #pragma region Color space conversions
    COLOR_API void ColorToRGB(Color* color, int* r, int* g, int* b, int* a);
    COLOR_API void ColorToHSL(Color* color, double* h, double* s, double* l, int* a);
    COLOR_API Color* ColorFromHSL(double h, double s, double l, int a);
    COLOR_API void ColorToHSV(Color* color, double* h, double* s, double* v, int* a);
    COLOR_API Color* ColorFromHSV(double h, double s, double v, int a);
    COLOR_API void ColorToHSI(Color* color, double* h, double* s, double* i, int* a);
    COLOR_API Color* ColorFromHSI(double h, double s, double i, double a);
    COLOR_API void ColorToHWB(Color* color, double* h, double* w, double* b, int* a);
    COLOR_API Color* ColorFromHWB(double h, double w, double b, double a);
    COLOR_API void ColorToCMYK(Color* color, double* c, double* m, double* y, double* k, int* a);
    COLOR_API Color* ColorFromCMYK(double c, double m, double y, double k, double a);
    COLOR_API void ColorToXYZ_D50(Color* color, double* x, double* y, double* z, int* a);
    COLOR_API Color* ColorFromXYZ_D50(double x, double y, double z, double a);
    COLOR_API void ColorToXYZ_D65(Color* color, double* x, double* y, double* z, int* a);
    COLOR_API Color* ColorFromXYZ_D65(double x, double y, double z, double a);
    COLOR_API void ColorToLab(Color* color, double* l, double* a, double* b, int* t);
    COLOR_API Color* ColorFromLab(double l, double a, double b, double alpha);
    COLOR_API void ColorToLuv(Color* color, double* L, double* u, double* v, int* a);
    COLOR_API Color* ColorFromLuv(double L, double u, double v, int a);
    COLOR_API void ColorToYIQ(Color* color, double* y, double* i, double* q, int* a);
    COLOR_API Color* ColorFromYIQ(double y, double i, double q, double a);
    COLOR_API void ColorToYPbPr(Color* color, double* y, double* pb, double* pr, int* a);
    COLOR_API Color* ColorFromYPbPr(double y, double pb, double pr, int a);
    COLOR_API void ColorToLCHab(Color* color, double* l, double* c, double* h, int* a);
    COLOR_API Color* ColorFromLCHab(double l, double c, double h, int a);
    COLOR_API void ColorToLCHuv(Color* color, double* l, double* c, double* h, int* a);
    COLOR_API Color* ColorFromLCHuv(double l, double c, double h, int a);
    #pragma endregion

    #pragma region Color manipulation
    COLOR_API void InvertColor(Color* color);
    COLOR_API void GrayscaleColor(Color* color);
    COLOR_API void SepiaColor(Color* color, double factor);
    COLOR_API void CrossProcessColor(Color* color, double factor);
    COLOR_API void MoonlightColor(Color* color, double factor);
    COLOR_API void VintageFilmColor(Color* color, double factor);
    COLOR_API void TechnicolorColor(Color* color, double factor);
    COLOR_API void PolaroidColor(Color* color, double factor);
    COLOR_API void ComplementColor(Color* color);
    #pragma endregion

    #pragma region Color adjustments
    COLOR_API void ShiftHueColor(Color* color, double degrees);
    COLOR_API void ShiftSaturationColor(Color* color, double amount);
    COLOR_API void ShiftLightnessColor(Color* color, double amount);
    COLOR_API void ShiftValueColor(Color* color, double amount);
    COLOR_API void ShiftIntensityColor(Color* color, double amount);
    COLOR_API void ShiftWhiteLevelColor(Color* color, double amount);
    COLOR_API void ShiftBlackLevelColor(Color* color, double amount);
    COLOR_API void ShiftContrastColor(Color* color, double amount);
    #pragma endregion

    #pragma region Color blending
    COLOR_API Color* ColorMix(Color* color1, Color* color2, double amount);
    COLOR_API Color* ColorScreen(Color* color1, Color* color2);
    COLOR_API Color* ColorMultiply(Color* color1, Color* color2);
    COLOR_API Color* ColorOverlay(Color* color1, Color* color2);
    COLOR_API Color* ColorAverage(Color** colors, int count);
    #pragma endregion

    #pragma region Matrix operations
    COLOR_API Color* ColorMultiplyMatrix(Color* color,
        double m11, double m12, double m13, double m14, double m15,
        double m21, double m22, double m23, double m24, double m25,
        double m31, double m32, double m33, double m34, double m35,
        double m41, double m42, double m43, double m44, double m45,
        double m51, double m52, double m53, double m54, double m55);

    COLOR_API Color* ColorAddMatrix(Color* color,
        double m11, double m12, double m13,
        double m21, double m22, double m23,
        double m31, double m32, double m33);

    COLOR_API Color* ColorSubtractMatrix(Color* color,
        double m11, double m12, double m13,
        double m21, double m22, double m23,
        double m31, double m32, double m33);
    #pragma endregion

    #pragma region Utility functions
    COLOR_API double GetColorLuminance(Color* color);
    COLOR_API bool IsColorLight(Color* color);
    COLOR_API bool IsColorDark(Color* color);
    COLOR_API double ColorGetContrast(Color* color1, Color* color2);
    COLOR_API bool IsColorAccessible(Color* color, Color* background, int level);
    COLOR_API Color* CreateRandomColor(int alphaRand);
    COLOR_API void ColorToString(Color* color, const char* type, const char* format, char* outStr);
    COLOR_API void FreeColorArray(Color** colors, int count);
    #pragma endregion
}