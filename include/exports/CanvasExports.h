#pragma once

#include "../Canvas.hpp"

extern "C"
{
    using namespace KTLib;

    #pragma region Constructors
    COLOR_API Canvas* CreateCanvasFromArray(Color** colors, int width, int height);
    COLOR_API Canvas* CreateCanvasFromWHC(int width, int height, Color* defaultColor);
    COLOR_API Canvas* CreateCanvasFromBuffer(unsigned int* buffer, int width, int height);
    COLOR_API Canvas* CreateCanvasFromGradient(Gradient* gradient, int width, int height);

    COLOR_API HBITMAP ExportCanvasAsHBITMAP(Canvas* buffer, int targetWidth, int targetHeight);
    COLOR_API Canvas* CreateCanvasFromHBITMAP(HBITMAP hBitmap, int width, int height);

    COLOR_API HDC ExportCanvasAsHDC(Canvas* buffer, int targetWidth, int targetHeight);
    COLOR_API Canvas* CreateCanvasFromHDC(HDC hdc, int x, int y, int width, int height);

    COLOR_API HICON ExportCanvasAsHICON(Canvas* buffer, int targetWidth, int targetHeight);
    COLOR_API Canvas* CreateCanvasFromHICON(HICON hIcon);

    COLOR_API HCURSOR ExportCanvasAsHCURSOR(Canvas* buffer, int targetWidth, int targetHeight);
    COLOR_API Canvas* CreateCanvasFromHCURSOR(HCURSOR hCursor);

    COLOR_API Canvas* CreateCanvasFromHWND(HWND hWnd, int x, int y, int width, int height);
    #pragma endregion

    #pragma region Canvas Functions
    COLOR_API void ShiftRedCanvas(Canvas* buffer, int amount);
    COLOR_API void ShiftGreenCanvas(Canvas* buffer, int amount);
    COLOR_API void ShiftBlueCanvas(Canvas* buffer, int amount);
    COLOR_API void ShiftAlphaCanvas(Canvas* buffer, int amount);
    COLOR_API void SetRedCanvas(Canvas* buffer, int value);
    COLOR_API void SetGreenCanvas(Canvas* buffer, int value);
    COLOR_API void SetBlueCanvas(Canvas* buffer, int value);
    COLOR_API void SetAlphaCanvas(Canvas* buffer, int value);

    COLOR_API void RotateCanvas(Canvas* buffer, double angle);
    COLOR_API void ResizeCanvas(Canvas* buffer, int newWidth, int newHeight, int resizeImage, Color* fillColor);
    COLOR_API void ScaleCanvas(Canvas* buffer, double scale);
    COLOR_API void DeleteCanvas(Canvas* buffer);

    COLOR_API int GetCanvasWidth(Canvas* buffer);
    COLOR_API int GetCanvasHeight(Canvas* buffer);
    COLOR_API int GetCanvasSize(Canvas* buffer);
    COLOR_API int GetCanvasStride(Canvas* buffer);
    COLOR_API Color* CanvasGetAt(Canvas* buffer, int index);
    COLOR_API void CanvasSetAt(Canvas* buffer, int index, Color* color);
    COLOR_API void CanvasGetXY(Canvas* buffer, int index, int* x, int* y);
    COLOR_API void CanvasGetIndex(Canvas* buffer, int x, int y, int* index);

    COLOR_API unsigned int GetColorIntFromBuffer(Canvas* buffer, int x, int y);
    COLOR_API void SetColorIntInBuffer(Canvas* buffer, int x, int y, unsigned int colorInt);
    COLOR_API Color* GetColorFromBuffer(Canvas* buffer, int x, int y);
    COLOR_API void SetColorInBuffer(Canvas* buffer, int x, int y, Color* color);
    #pragma endregion

    #pragma region Color Modification Functions
    COLOR_API void ShiftHueCanvas(Canvas* buffer, double degrees);
    COLOR_API void ShiftSaturationCanvas(Canvas* buffer, double amount);
    COLOR_API void ShiftLightnessCanvas(Canvas* buffer, double amount);
    COLOR_API void ShiftValueCanvas(Canvas* buffer, double amount);
    COLOR_API void ShiftIntensityCanvas(Canvas* buffer, double amount);
    COLOR_API void ShiftWhiteLevelCanvas(Canvas* buffer, double amount);
    COLOR_API void ShiftBlackLevelCanvas(Canvas* buffer, double amount);
    COLOR_API void ShiftContrastCanvas(Canvas* buffer, double amount);
    COLOR_API void AdjustContrastCanvas(Canvas* buffer, double factor);
    COLOR_API void AdjustColorBalanceCanvas(Canvas* buffer, double redFactor, double greenFactor, double blueFactor);

    COLOR_API void OverlayImageCanvas(Canvas* destBuffer, Canvas* overlayBuffer, int x, int y, double opacity);
    COLOR_API void InvertCanvas(Canvas* buffer);
    COLOR_API void GrayscaleCanvas(Canvas* buffer);
    COLOR_API void SepiaCanvas(Canvas* buffer, double factor);
    COLOR_API void PixelateCanvas(Canvas* buffer, int pixelSize);
    COLOR_API void BlurCanvas(Canvas* buffer, int radius);
    COLOR_API void GaussianBlurCanvas(Canvas* buffer, int radius);
    COLOR_API void SharpenCanvas(Canvas* buffer, float amount);
    COLOR_API void CrossProcessCanvas(Canvas* buffer, double factor);
    COLOR_API void MoonlightCanvas(Canvas* buffer, double factor);
    COLOR_API void VintageFilmCanvas(Canvas* buffer, double factor);
    COLOR_API void TechnicolorCanvas(Canvas* buffer, double factor);
    COLOR_API void PolaroidCanvas(Canvas* buffer, double factor);
    COLOR_API void ComplementCanvas(Canvas* buffer);
    COLOR_API void ApplyEmbossCanvas(Canvas* buffer);
    COLOR_API void ApplyEdgeDetectCanvas(Canvas* buffer);
    COLOR_API void ApplyVignetteCanvas(Canvas* buffer, double strength, double radius);
    COLOR_API void ApplyTwoColorNoiseCanvas(Canvas* buffer, double density, Color* saltColor, Color* pepperColor);
    COLOR_API void ApplyGaussianNoiseCanvas(Canvas* buffer, double mean, double stdDev);
    COLOR_API void ApplyPerlinNoiseCanvas(Canvas* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity);
    COLOR_API void ApplySimplexNoiseCanvas(Canvas* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity);
    COLOR_API void ApplyFractalBrownianMotionCanvas(Canvas* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity);
    COLOR_API void ApplyVoronoiDiagramCanvas(Canvas* buffer, int numberOfPoints, double falloff, double strength);
    COLOR_API void PlasmaEffectCanvas(Canvas* buffer, double frequency, double phase);
    COLOR_API void DiamondSquareEffectCanvas(Canvas* buffer, double roughness, double waterLevel, double levelsPerStop);
    COLOR_API void PosterizeCanvas(Canvas* buffer, int levels);
    #pragma endregion

    #pragma region Utility
    COLOR_API void FlipCanvas(Canvas* buffer, bool horizontal);
    COLOR_API void CropCanvas(Canvas* buffer, int x, int y, int width, int height);
    COLOR_API Color* AverageCanvas(Canvas* buffer);
    COLOR_API Canvas* CopyCanvasRegion(Canvas* buffer, int x, int y, int w, int h);
    COLOR_API Canvas* CopyCanvas(Canvas* buffer);

    COLOR_API int CanvasFind(Canvas* buffer, Color* color);
    COLOR_API int CanvasFindLast(Canvas* buffer, Color* color);
    COLOR_API size_t CanvasCountUniqueColors(Canvas* buffer);
    COLOR_API void CanvasForEach(Canvas* buffer, void (*func)(Color*));
    COLOR_API void CanvasSwap(Canvas* buffer, int index1, int index2);
    COLOR_API Canvas* CanvasFilter(Canvas* buffer, bool (*predicate)(Color*));
    COLOR_API int CanvasCount(Canvas* buffer, Color* color);
    COLOR_API void CanvasShuffle(Canvas* buffer);
    COLOR_API void CanvasClear(Canvas* buffer);
    COLOR_API void CanvasSort(Canvas* buffer, int (*compare)(Color*, Color*));
    COLOR_API void CanvasAppendRight(Canvas* buffer, Canvas* other);
    COLOR_API void CanvasAppendBottom(Canvas* buffer, Canvas* other);
    COLOR_API void MapColorsInBuffer(Canvas* buffer, int x, int y, int width, int height, void* mapFunction);
    COLOR_API int* CanvasFindAll(Canvas* buffer, Color* color, int* count);
    COLOR_API void CanvasApplyMatrix(Canvas* buffer, ColorMatrix* matrix);
    COLOR_API void DrawCanvas(Canvas* buffer, HWND hwnd, int x, int y);
    #pragma endregion
}