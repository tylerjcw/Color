#pragma once

#include "../Gradient.hpp"

extern "C"
{
    using namespace KTLib;

    #pragma region Constructors
    GRADIENT_API Gradient* CreateGradientFromSteps(int totalSteps);
    GRADIENT_API Gradient* CreateGradientFromGradient(Gradient* gradient);
    GRADIENT_API Gradient* CreateGradientFromColors(int totalSteps, unsigned int* colors, int colorCount);
    #pragma endregion

    #pragma region Gradient Functions
    GRADIENT_API void GradientDeleteGradient(Gradient* gradient);
    GRADIENT_API void GradientSetAngle(Gradient* gradient, float degrees);
    GRADIENT_API float GradientGetAngle(Gradient* gradient);
    GRADIENT_API int GradientGetVertices(Gradient* gradient);
    GRADIENT_API void GradientSetVertices(Gradient* gradient, int vertices);
    GRADIENT_API void GradientSetType(Gradient* gradient, int type);
    GRADIENT_API int GradientGetType(Gradient* gradient);
    GRADIENT_API void GradientAddColorStop(Gradient* gradient, unsigned int color, float position);
    GRADIENT_API void GradientRemoveColorStop(Gradient* gradient, float position);
    GRADIENT_API Color* GradientGetColorAt(Gradient* gradient, float position);
    GRADIENT_API void GradientRotate(Gradient* gradient, float angle);
    GRADIENT_API void GradientReverse(Gradient* gradient);
    GRADIENT_API void GradientShift(Gradient* gradient, float amount);
    #pragma endregion

    #pragma region Color Modification Functions
    GRADIENT_API void GradientShiftHue(Gradient* gradient, double degrees);
    GRADIENT_API void GradientShiftSaturation(Gradient* gradient, double amount);
    GRADIENT_API void GradientShiftLightness(Gradient* gradient, double amount);
    GRADIENT_API void GradientShiftWhiteness(Gradient* gradient, double amount);
    GRADIENT_API void GradientShiftBlackness(Gradient* gradient, double amount);
    GRADIENT_API void GradientShiftValue(Gradient* gradient, double amount);
    GRADIENT_API void GradientGrayscale(Gradient* gradient);
    GRADIENT_API void GradientSepia(Gradient* gradient, double factor);
    GRADIENT_API void GradientInvert(Gradient* gradient);
    GRADIENT_API void GradientComplement(Gradient* gradient);
    GRADIENT_API void GradientSetTotalSteps(Gradient* gradient, int totalSteps);
    GRADIENT_API int GradientGetTotalSteps(Gradient* gradient);
    GRADIENT_API Color* GradientGetColorAtStep(Gradient* gradient, int step);
    GRADIENT_API void GradientSetFocus(Gradient* gradient, float x, float y);
    GRADIENT_API void GradientGetFocus(Gradient* gradient, float* x, float* y);
    GRADIENT_API void GradientSetEdgeSharpness(Gradient* gradient, float sharpness);
    GRADIENT_API float GradientGetEdgeSharpness(Gradient* gradient);
    GRADIENT_API void GradientSetWavelength(Gradient* gradient, float wavelength);
    GRADIENT_API float GradientGetWavelength(Gradient* gradient);
    GRADIENT_API void GradientSetAmplitude(Gradient* gradient, float amplitude);
    GRADIENT_API float GradientGetAmplitude(Gradient* gradient);
    GRADIENT_API void GradientSetRepetitions(Gradient* gradient, float repetitions);
    GRADIENT_API float GradientGetRepetitions(Gradient* gradient);
    #pragma endregion

    #pragma region Utility
    GRADIENT_API const char* GradientSerialize(Gradient* gradient);
    GRADIENT_API Gradient* GradientDeserialize(const char* data);
    GRADIENT_API HBITMAP GradientCreateHBITMAP(Gradient* gradient, int width, int height);
    GRADIENT_API void DrawGradient(Gradient* gradient, HWND hwnd, int x, int y, int width, int height);
    #pragma endregion
}