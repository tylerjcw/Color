#include "../../include/exports/ColorBufferExports.h"

extern "C"
{
    #pragma region Constructors
    COLOR_API ColorBuffer* CreateColorBufferFromArray(Color** colors, int width, int height) { return new ColorBuffer(colors, width, height); }
    COLOR_API ColorBuffer* CreateColorBufferFromWHC(int width, int height, Color* defaultColor) { return new ColorBuffer(width, height, *defaultColor); }
    COLOR_API ColorBuffer* CreateColorBufferFromBuffer(unsigned int* buffer, int width, int height) { return new ColorBuffer(buffer, width, height); }
    COLOR_API ColorBuffer* CreateColorBufferFromGradient(Gradient* gradient, int width, int height) { return new ColorBuffer(*gradient, width, height); }

    // HBITMAP
    COLOR_API HBITMAP ExportColorBufferAsHBITMAP(ColorBuffer* buffer, int targetWidth, int targetHeight) { return buffer->ToHBITMAP(targetWidth, targetHeight); }
    COLOR_API ColorBuffer* CreateColorBufferFromHBITMAP(HBITMAP hBitmap, int width, int height) { return ColorBuffer::FromHBITMAP(hBitmap, width, height); }

    // HDC
    COLOR_API HDC ExportColorBufferAsHDC(ColorBuffer* buffer, int targetWidth, int targetHeight) { return buffer->ToHDC(targetWidth, targetHeight); }
    COLOR_API ColorBuffer* CreateColorBufferFromHDC(HDC hdc, int x, int y, int width, int height) { return ColorBuffer::FromHDC(hdc, x, y, width, height); }

    // HICON
    COLOR_API HICON ExportColorBufferAsHICON(ColorBuffer* buffer, int targetWidth, int targetHeight) { return buffer->ToHICON(targetWidth, targetHeight); }
    COLOR_API ColorBuffer* CreateColorBufferFromHICON(HICON hIcon) { return ColorBuffer::FromHICON(hIcon); }

    // HCURSOR
    COLOR_API HCURSOR ExportColorBufferAsHCURSOR(ColorBuffer* buffer, int targetWidth, int targetHeight) { return buffer->ToHCURSOR(targetWidth, targetHeight); }
    COLOR_API ColorBuffer* CreateColorBufferFromHCURSOR(HCURSOR hCursor) { return ColorBuffer::FromHCURSOR(hCursor); }

    COLOR_API ColorBuffer* CreateColorBufferFromHWND(HWND hWnd, int x, int y, int width, int height) { return ColorBuffer::FromHWND(hWnd, x, y, width, height); }
    #pragma endregion

    #pragma region ColorBuffer Functions
    COLOR_API void ShiftRedColorBuffer(ColorBuffer* buffer, int amount) { buffer->ShiftRed(amount); }
    COLOR_API void ShiftGreenColorBuffer(ColorBuffer* buffer, int amount) { buffer->ShiftGreen(amount); }
    COLOR_API void ShiftBlueColorBuffer(ColorBuffer* buffer, int amount) { buffer->ShiftBlue(amount); }
    COLOR_API void ShiftAlphaColorBuffer(ColorBuffer* buffer, int amount) { buffer->ShiftAlpha(amount); }
    COLOR_API void SetRedColorBuffer(ColorBuffer* buffer, int value) { buffer->SetRed(value); }
    COLOR_API void SetGreenColorBuffer(ColorBuffer* buffer, int value) { buffer->SetGreen(value); }
    COLOR_API void SetBlueColorBuffer(ColorBuffer* buffer, int value) { buffer->SetBlue(value); }
    COLOR_API void SetAlphaColorBuffer(ColorBuffer* buffer, int value) { buffer->SetAlpha(value); }

    COLOR_API void RotateColorBuffer(ColorBuffer* buffer, double angle) { buffer->Rotate(angle); }
    COLOR_API void ResizeColorBuffer(ColorBuffer* buffer, int newWidth, int newHeight, int resizeImage, Color* fillColor) { buffer->Resize(newWidth, newHeight, resizeImage, *fillColor); }
    COLOR_API void ScaleColorBuffer(ColorBuffer* buffer, double scale) { buffer->Scale(scale); }
    COLOR_API void DeleteColorBuffer(ColorBuffer* buffer) { delete buffer; }

    COLOR_API int GetColorBufferWidth(ColorBuffer* buffer) { return buffer->GetWidth(); }
    COLOR_API int GetColorBufferHeight(ColorBuffer* buffer) { return buffer->GetHeight(); }
    COLOR_API int GetColorBufferSize(ColorBuffer* buffer) { return buffer->GetSize(); }
    COLOR_API int GetColorBufferStride(ColorBuffer* buffer) { return buffer->GetStride(); }
    COLOR_API Color* ColorBufferGetAt(ColorBuffer* buffer, int index) { return new Color(buffer->GetAt(index)); }
    COLOR_API void ColorBufferSetAt(ColorBuffer* buffer, int index, Color* color) { buffer->SetAt(index, *color); }
    COLOR_API void ColorBufferGetXY(ColorBuffer* buffer, int index, int* x, int* y) { buffer->GetXY(index, *x, *y); }
    COLOR_API void ColorBufferGetIndex(ColorBuffer* buffer, int x, int y, int* index) { buffer->GetIndex(x, y, *index); }

    COLOR_API unsigned int GetColorIntFromBuffer(ColorBuffer* buffer, int x, int y) { return buffer->Get(x, y).ToInt(0); }
    COLOR_API void SetColorIntInBuffer(ColorBuffer* buffer, int x, int y, unsigned int colorInt) { buffer->Set(x, y, Color(colorInt)); }
    COLOR_API Color* GetColorFromBuffer(ColorBuffer* buffer, int x, int y) { return new Color(buffer->Get(x, y)); }
    COLOR_API void SetColorInBuffer(ColorBuffer* buffer, int x, int y, Color* color) { buffer->Set(x, y, *color); }
    #pragma endregion

    #pragma region Color Modification Functions
    COLOR_API void ShiftHueColorBuffer(ColorBuffer* buffer, double degrees) { buffer->ShiftHue(degrees); }
    COLOR_API void ShiftSaturationColorBuffer(ColorBuffer* buffer, double amount) { buffer->ShiftSaturation(amount); }
    COLOR_API void ShiftLightnessColorBuffer(ColorBuffer* buffer, double amount) { buffer->ShiftLightness(amount); }
    COLOR_API void ShiftValueColorBuffer(ColorBuffer* buffer, double amount) { buffer->ShiftValue(amount); }
    COLOR_API void ShiftIntensityColorBuffer(ColorBuffer* buffer, double amount) { buffer->ShiftIntensity(amount); }
    COLOR_API void ShiftWhiteLevelColorBuffer(ColorBuffer* buffer, double amount) { buffer->ShiftWhiteLevel(amount); }
    COLOR_API void ShiftBlackLevelColorBuffer(ColorBuffer* buffer, double amount) { buffer->ShiftBlackLevel(amount); }
    COLOR_API void ShiftContrastColorBuffer(ColorBuffer* buffer, double amount) { buffer->ShiftContrast(amount); }
    COLOR_API void AdjustContrastColorBuffer(ColorBuffer* buffer, double factor) { buffer->AdjustContrast(factor); }
    COLOR_API void AdjustColorBalanceColorBuffer(ColorBuffer* buffer, double redFactor, double greenFactor, double blueFactor) { buffer->AdjustColorBalance(redFactor, greenFactor, blueFactor); }

    COLOR_API void OverlayImageColorBuffer(ColorBuffer* destBuffer, ColorBuffer* overlayBuffer, int x, int y, double opacity) { destBuffer->OverlayImage(*overlayBuffer, x, y, opacity); }
    COLOR_API void InvertColorBuffer(ColorBuffer* buffer) { buffer->Invert(); }
    COLOR_API void GrayscaleColorBuffer(ColorBuffer* buffer) { buffer->Grayscale(); }
    COLOR_API void SepiaColorBuffer(ColorBuffer* buffer, double factor) { buffer->Sepia(factor); }
    COLOR_API void PixelateColorBuffer(ColorBuffer* buffer, int pixelSize) { buffer->Pixelate(pixelSize); }
    COLOR_API void BlurColorBuffer(ColorBuffer* buffer, int radius) { buffer->Blur(radius); }
    COLOR_API void GaussianBlurColorBuffer(ColorBuffer* buffer, int radius) { buffer->GaussianBlur(radius); }
    COLOR_API void SharpenColorBuffer(ColorBuffer* buffer, float amount) { buffer->Sharpen(amount); }
    COLOR_API void CrossProcessColorBuffer(ColorBuffer* buffer, double factor) { buffer->CrossProcess(factor); }
    COLOR_API void MoonlightColorBuffer(ColorBuffer* buffer, double factor) { buffer->Moonlight(factor); }
    COLOR_API void VintageFilmColorBuffer(ColorBuffer* buffer, double factor) { buffer->VintageFilm(factor); }
    COLOR_API void TechnicolorColorBuffer(ColorBuffer* buffer, double factor) { buffer->Technicolor(factor); }
    COLOR_API void PolaroidColorBuffer(ColorBuffer* buffer, double factor) { buffer->Polaroid(factor); }
    COLOR_API void ComplementColorBuffer(ColorBuffer* buffer) { buffer->Complement(); }
    COLOR_API void ApplyEmbossColorBuffer(ColorBuffer* buffer) { buffer->Emboss(); }
    COLOR_API void ApplyEdgeDetectColorBuffer(ColorBuffer* buffer) { buffer->EdgeDetect(); }
    COLOR_API void ApplyVignetteColorBuffer(ColorBuffer* buffer, double strength, double radius) { buffer->Vignette(strength, radius); }
    COLOR_API void ApplyTwoColorNoiseColorBuffer(ColorBuffer* buffer, double density, Color* saltColor, Color* pepperColor) { buffer->TwoColorNoise(density, *saltColor, *pepperColor); }
    COLOR_API void ApplyGaussianNoiseColorBuffer(ColorBuffer* buffer, double mean, double stdDev) { buffer->GaussianNoise(mean, stdDev); }
    COLOR_API void ApplyPerlinNoiseColorBuffer(ColorBuffer* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { buffer->PerlinNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplySimplexNoiseColorBuffer(ColorBuffer* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { buffer->SimplexNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyFractalBrownianMotionColorBuffer(ColorBuffer* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { buffer->FractalBrownianMotion(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyVoronoiDiagramColorBuffer(ColorBuffer* buffer, int numberOfPoints, double falloff, double strength) { buffer->Voronoi(numberOfPoints, falloff, strength); }
    COLOR_API void PlasmaEffectColorBuffer(ColorBuffer* buffer, double frequency, double phase) { buffer->Plasma(frequency, phase); }
    COLOR_API void DiamondSquareEffectColorBuffer(ColorBuffer* buffer, double roughness, double waterLevel, double levelsPerStop) { buffer->DiamondSquare(roughness, waterLevel, levelsPerStop); }
    COLOR_API void PosterizeColorBuffer(ColorBuffer* buffer, int levels) { buffer->Posterize(levels); }
    #pragma endregion

    #pragma region Utility
    COLOR_API void FlipColorBuffer(ColorBuffer* buffer, bool horizontal) { buffer->Flip(horizontal); }
    COLOR_API void CropColorBuffer(ColorBuffer* buffer, int x, int y, int width, int height) { buffer->Crop(x, y, width, height); }
    COLOR_API Color* AverageColorBuffer(ColorBuffer* buffer) { return new Color(buffer->CalculateAverageColor(0, 0, buffer->GetWidth(), buffer->GetHeight())); }
    COLOR_API ColorBuffer* CopyColorBufferRegion(ColorBuffer* buffer, int x, int y, int w, int h) { return buffer->CopyRegion(x, y, w, h); }
    COLOR_API ColorBuffer* CopyColorBuffer(ColorBuffer* buffer) { return buffer->Copy(); }

    COLOR_API int ColorBufferFind(ColorBuffer* buffer, Color* color) { return buffer->Find(*color); };
    COLOR_API int ColorBufferFindLast(ColorBuffer* buffer, Color* color) { return buffer->FindLast(*color); }
    COLOR_API void ColorBufferForEach(ColorBuffer* buffer, void (*func)(Color*)) { buffer->ForEach([func](const Color& color) { func(const_cast<Color*>(&color)); }); }
    COLOR_API void ColorBufferSwap(ColorBuffer* buffer, int index1, int index2) { buffer->Swap(index1, index2); }
    COLOR_API ColorBuffer* ColorBufferFilter(ColorBuffer* buffer, bool (*predicate)(Color*)) { return new ColorBuffer(buffer->Filter([predicate](const Color& color) { return predicate(const_cast<Color*>(&color)); })); }
    COLOR_API int ColorBufferCount(ColorBuffer* buffer, Color* color) { return buffer->Count(*color); }
    COLOR_API size_t ColorBufferCountUniqueColors(ColorBuffer* buffer) { return buffer->CountUniqueColors(); }
    COLOR_API void ColorBufferShuffle(ColorBuffer* buffer) { buffer->Shuffle(); }
    COLOR_API void ColorBufferClear(ColorBuffer* buffer) { buffer->Clear(); }
    COLOR_API void ColorBufferSort(ColorBuffer* buffer, int (*compare)(Color*, Color*))
    {
        buffer->Sort([compare](const Color& a, const Color& b) {
            Color* aPtr = new Color(a);
            Color* bPtr = new Color(b);
            int result = compare(aPtr, bPtr);
            delete aPtr;
            delete bPtr;
            return result < 0;
        });
    }

    COLOR_API void ColorBufferAppendRight(ColorBuffer* buffer, ColorBuffer* other) { buffer->AppendRight(*other); }
    COLOR_API void ColorBufferAppendBottom(ColorBuffer* buffer, ColorBuffer* other) { buffer->AppendBottom(*other); }

    COLOR_API void MapColorsInBuffer(ColorBuffer* buffer, int x, int y, int width, int height, void* mapFunction)
    {
        auto func = reinterpret_cast<unsigned int(*)(int, int, unsigned int)>(mapFunction);
        buffer->MapColors(x, y, width, height, func);
    }

    COLOR_API int* ColorBufferFindAll(ColorBuffer* buffer, Color* color, int* count)
    {
        auto result = buffer->FindAll(*color);
        *count = result.size();
        int* arr = new int[*count];
        std::copy(result.begin(), result.end(), arr);
        return arr;
    }

    COLOR_API void ColorBufferApplyMatrix(ColorBuffer* buffer, ColorMatrix* matrix) { buffer->ApplyMatrix(*matrix); }
    COLOR_API void DrawColorBuffer(ColorBuffer* buffer, HWND hwnd, int x, int y) { buffer->Draw(hwnd, x, y); }
    #pragma endregion
}