#pragma once

#include "../ColorBuffer.hpp"

extern "C"
{
    using namespace KTLib;

    #pragma region Constructors
    COLOR_API ColorBuffer* CreateColorBufferFromArray(Color** colors, int width, int height);
    COLOR_API ColorBuffer* CreateColorBufferFromWHC(int width, int height, Color* defaultColor);
    COLOR_API ColorBuffer* CreateColorBufferFromBuffer(unsigned int* buffer, int width, int height);
    COLOR_API ColorBuffer* CreateColorBufferFromHBITMAP(HBITMAP hBitmap, int targetWidth, int targetHeight);
    COLOR_API ColorBuffer* CreateColorBufferFromGradient(Gradient* gradient, int width, int height);
    #pragma endregion

    #pragma region ColorBuffer Functions
    COLOR_API void ShiftRedColorBuffer(ColorBuffer* buffer, int amount);
    COLOR_API void ShiftGreenColorBuffer(ColorBuffer* buffer, int amount);
    COLOR_API void ShiftBlueColorBuffer(ColorBuffer* buffer, int amount);
    COLOR_API void ShiftAlphaColorBuffer(ColorBuffer* buffer, int amount);
    COLOR_API void SetRedColorBuffer(ColorBuffer* buffer, int value);
    COLOR_API void SetGreenColorBuffer(ColorBuffer* buffer, int value);
    COLOR_API void SetBlueColorBuffer(ColorBuffer* buffer, int value);
    COLOR_API void SetAlphaColorBuffer(ColorBuffer* buffer, int value);

    COLOR_API void RotateColorBuffer(ColorBuffer* buffer, double angle);
    COLOR_API void ResizeColorBuffer(ColorBuffer* buffer, int newWidth, int newHeight, int resizeImage, Color* fillColor);
    COLOR_API void ScaleColorBuffer(ColorBuffer* buffer, double scale);
    COLOR_API void DeleteColorBuffer(ColorBuffer* buffer);

    COLOR_API int GetColorBufferWidth(ColorBuffer* buffer);
    COLOR_API int GetColorBufferHeight(ColorBuffer* buffer);
    COLOR_API int GetColorBufferSize(ColorBuffer* buffer);
    COLOR_API int GetColorBufferStride(ColorBuffer* buffer);
    COLOR_API Color* ColorBufferGetAt(ColorBuffer* buffer, int index);
    COLOR_API void ColorBufferSetAt(ColorBuffer* buffer, int index, Color* color);
    COLOR_API void ColorBufferGetXY(ColorBuffer* buffer, int index, int* x, int* y);
    COLOR_API void ColorBufferGetIndex(ColorBuffer* buffer, int x, int y, int* index);

    COLOR_API unsigned int GetColorIntFromBuffer(ColorBuffer* buffer, int x, int y);
    COLOR_API void SetColorIntInBuffer(ColorBuffer* buffer, int x, int y, unsigned int colorInt);
    COLOR_API Color* GetColorFromBuffer(ColorBuffer* buffer, int x, int y);
    COLOR_API void SetColorInBuffer(ColorBuffer* buffer, int x, int y, Color* color);
    #pragma endregion

    #pragma region Color Modification Functions
    COLOR_API void ShiftHueColorBuffer(ColorBuffer* buffer, double degrees);
    COLOR_API void ShiftSaturationColorBuffer(ColorBuffer* buffer, double amount);
    COLOR_API void ShiftLightnessColorBuffer(ColorBuffer* buffer, double amount);
    COLOR_API void ShiftValueColorBuffer(ColorBuffer* buffer, double amount);
    COLOR_API void ShiftIntensityColorBuffer(ColorBuffer* buffer, double amount);
    COLOR_API void ShiftWhiteLevelColorBuffer(ColorBuffer* buffer, double amount);
    COLOR_API void ShiftBlackLevelColorBuffer(ColorBuffer* buffer, double amount);
    COLOR_API void ShiftContrastColorBuffer(ColorBuffer* buffer, double amount);
    COLOR_API void AdjustContrastColorBuffer(ColorBuffer* buffer, double factor);
    COLOR_API void AdjustColorBalanceColorBuffer(ColorBuffer* buffer, double redFactor, double greenFactor, double blueFactor);

    COLOR_API void OverlayImageColorBuffer(ColorBuffer* destBuffer, ColorBuffer* overlayBuffer, int x, int y, double opacity);
    COLOR_API void InvertColorBuffer(ColorBuffer* buffer);
    COLOR_API void GrayscaleColorBuffer(ColorBuffer* buffer);
    COLOR_API void SepiaColorBuffer(ColorBuffer* buffer, double factor);
    COLOR_API void PixelateColorBuffer(ColorBuffer* buffer, int pixelSize);
    COLOR_API void BlurColorBuffer(ColorBuffer* buffer, int radius);
    COLOR_API void GaussianBlurColorBuffer(ColorBuffer* buffer, int radius);
    COLOR_API void SharpenColorBuffer(ColorBuffer* buffer, float amount);
    COLOR_API void CrossProcessColorBuffer(ColorBuffer* buffer, double factor);
    COLOR_API void MoonlightColorBuffer(ColorBuffer* buffer, double factor);
    COLOR_API void VintageFilmColorBuffer(ColorBuffer* buffer, double factor);
    COLOR_API void TechnicolorColorBuffer(ColorBuffer* buffer, double factor);
    COLOR_API void PolaroidColorBuffer(ColorBuffer* buffer, double factor);
    COLOR_API void ComplementColorBuffer(ColorBuffer* buffer);
    COLOR_API void ApplyEmbossColorBuffer(ColorBuffer* buffer);
    COLOR_API void ApplyEdgeDetectColorBuffer(ColorBuffer* buffer);
    COLOR_API void ApplyVignetteColorBuffer(ColorBuffer* buffer, double strength, double radius);
    COLOR_API void ApplyTwoColorNoiseColorBuffer(ColorBuffer* buffer, double density, Color* saltColor, Color* pepperColor);
    COLOR_API void ApplyGaussianNoiseColorBuffer(ColorBuffer* buffer, double mean, double stdDev);
    COLOR_API void ApplyPerlinNoiseColorBuffer(ColorBuffer* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity);
    COLOR_API void ApplySimplexNoiseColorBuffer(ColorBuffer* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity);
    COLOR_API void ApplyFractalBrownianMotionColorBuffer(ColorBuffer* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity);
    COLOR_API void ApplyVoronoiDiagramColorBuffer(ColorBuffer* buffer, int numberOfPoints, double falloff, double strength);
    COLOR_API void PlasmaEffectColorBuffer(ColorBuffer* buffer, double frequency, double phase);
    COLOR_API void DiamondSquareEffectColorBuffer(ColorBuffer* buffer, double roughness, double waterLevel, double levelsPerStop);
    COLOR_API void PosterizeColorBuffer(ColorBuffer* buffer, int levels);
    #pragma endregion

    #pragma region Utility
    COLOR_API void FlipColorBuffer(ColorBuffer* buffer, bool horizontal);
    COLOR_API void CropColorBuffer(ColorBuffer* buffer, int x, int y, int width, int height);
    COLOR_API Color* AverageColorBuffer(ColorBuffer* buffer);
    COLOR_API ColorBuffer* CopyColorBufferRegion(ColorBuffer* buffer, int x, int y, int w, int h);
    COLOR_API ColorBuffer* CopyColorBuffer(ColorBuffer* buffer);

    COLOR_API int ColorBufferFind(ColorBuffer* buffer, Color* color);
    COLOR_API int ColorBufferFindLast(ColorBuffer* buffer, Color* color);
    COLOR_API void ColorBufferForEach(ColorBuffer* buffer, void (*func)(Color*));
    COLOR_API void ColorBufferSwap(ColorBuffer* buffer, int index1, int index2);
    COLOR_API ColorBuffer* ColorBufferFilter(ColorBuffer* buffer, bool (*predicate)(Color*));
    COLOR_API int ColorBufferCount(ColorBuffer* buffer, Color* color);
    COLOR_API void ColorBufferShuffle(ColorBuffer* buffer);
    COLOR_API void ColorBufferClear(ColorBuffer* buffer);
    COLOR_API void ColorBufferSort(ColorBuffer* buffer, int (*compare)(Color*, Color*));
    COLOR_API void ColorBufferAppendRight(ColorBuffer* buffer, ColorBuffer* other);
    COLOR_API void ColorBufferAppendBottom(ColorBuffer* buffer, ColorBuffer* other);
    COLOR_API void MapColorsInBuffer(ColorBuffer* buffer, int x, int y, int width, int height, void* mapFunction);
    COLOR_API int* ColorBufferFindAll(ColorBuffer* buffer, Color* color, int* count);
    COLOR_API void ColorBufferApplyMatrix(ColorBuffer* buffer,
        double m11, double m12, double m13, double m14, double m15,
        double m21, double m22, double m23, double m24, double m25,
        double m31, double m32, double m33, double m34, double m35,
        double m41, double m42, double m43, double m44, double m45,
        double m51, double m52, double m53, double m54, double m55);
    COLOR_API HBITMAP ExportColorBufferAsHBitmap(ColorBuffer* buffer, int targetWidth, int targetHeight);
    COLOR_API void DrawColorBuffer(ColorBuffer* buffer, HWND hwnd, int x, int y);
    #pragma endregion
}