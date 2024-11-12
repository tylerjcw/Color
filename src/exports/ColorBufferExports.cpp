#include "../../include/exports/ColorBufferExports.h"

extern "C"
{
    #pragma region Constructors
    COLOR_API ColorBuffer* CreateColorBufferFromArray(Color** colors, int width, int height) { return new ColorBuffer(colors, width, height); }
    COLOR_API ColorBuffer* CreateColorBufferFromWHC(int width, int height, Color* defaultColor) { return new ColorBuffer(width, height, *defaultColor); }
    COLOR_API ColorBuffer* CreateColorBufferFromBuffer(unsigned int* buffer, int width, int height) { return new ColorBuffer(buffer, width, height); }
    COLOR_API ColorBuffer* CreateColorBufferFromHBITMAP(HBITMAP hBitmap, int targetWidth, int targetHeight) { return ColorBuffer::FromHBITMAP(hBitmap, targetWidth, targetHeight); }
    COLOR_API ColorBuffer* CreateColorBufferFromGradient(Gradient* gradient, int width, int height) { return new ColorBuffer(*gradient, width, height); }
    #pragma endregion

    #pragma region ColorBuffer Functions
    COLOR_API void ShiftRedColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftRed(amount); }
    COLOR_API void ShiftGreenColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftGreen(amount); }
    COLOR_API void ShiftBlueColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftBlue(amount); }
    COLOR_API void ShiftAlphaColorBuffer(ColorBuffer* colorBuffer, int amount) { colorBuffer->ShiftAlpha(amount); }
    COLOR_API void SetRedColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetRed(value); }
    COLOR_API void SetGreenColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetGreen(value); }
    COLOR_API void SetBlueColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetBlue(value); }
    COLOR_API void SetAlphaColorBuffer(ColorBuffer* colorBuffer, int value) { colorBuffer->SetAlpha(value); }

    COLOR_API void RotateColorBuffer(ColorBuffer* colorBuffer, double angle) { colorBuffer->Rotate(angle); }
    COLOR_API void ResizeColorBuffer(ColorBuffer* colorBuffer, int newWidth, int newHeight, int resizeImage, Color* fillColor) { colorBuffer->Resize(newWidth, newHeight, resizeImage, *fillColor); }
    COLOR_API void ScaleColorBuffer(ColorBuffer* colorBuffer, double scale) { colorBuffer->Scale(scale); }
    COLOR_API void DeleteColorBuffer(ColorBuffer* colorBuffer) { delete colorBuffer; }

    COLOR_API int GetColorBufferWidth(ColorBuffer* colorBuffer) { return colorBuffer->GetWidth(); }
    COLOR_API int GetColorBufferHeight(ColorBuffer* colorBuffer) { return colorBuffer->GetHeight(); }
    COLOR_API int GetColorBufferSize(ColorBuffer* colorBuffer) { return colorBuffer->GetSize(); }
    COLOR_API int GetColorBufferStride(ColorBuffer* colorBuffer) { return colorBuffer->GetStride(); }
    COLOR_API Color* ColorBufferGetAt(ColorBuffer* colorBuffer, int index) { return new Color(colorBuffer->GetAt(index)); }
    COLOR_API void ColorBufferSetAt(ColorBuffer* colorBuffer, int index, Color* color) { colorBuffer->SetAt(index, *color); }
    COLOR_API void ColorBufferGetXY(ColorBuffer* colorBuffer, int index, int* x, int* y) { colorBuffer->GetXY(index, *x, *y); }
    COLOR_API void ColorBufferGetIndex(ColorBuffer* colorBuffer, int x, int y, int* index) { colorBuffer->GetIndex(x, y, *index); }

    COLOR_API unsigned int GetColorIntFromBuffer(ColorBuffer* colorBuffer, int x, int y) { return colorBuffer->Get(x, y).ToInt(0); }
    COLOR_API void SetColorIntInBuffer(ColorBuffer* colorBuffer, int x, int y, unsigned int colorInt) { colorBuffer->Set(x, y, Color(colorInt)); }
    COLOR_API Color* GetColorFromBuffer(ColorBuffer* colorBuffer, int x, int y) { return new Color(colorBuffer->Get(x, y)); }
    COLOR_API void SetColorInBuffer(ColorBuffer* colorBuffer, int x, int y, Color* color) { colorBuffer->Set(x, y, *color); }
    #pragma endregion

    #pragma region Color Modification Functions
    COLOR_API void ShiftHueColorBuffer(ColorBuffer* colorBuffer, double degrees) { colorBuffer->ShiftHue(degrees); }
    COLOR_API void ShiftSaturationColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftSaturation(amount); }
    COLOR_API void ShiftLightnessColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftLightness(amount); }
    COLOR_API void ShiftValueColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftValue(amount); }
    COLOR_API void ShiftIntensityColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftIntensity(amount); }
    COLOR_API void ShiftWhiteLevelColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftWhiteLevel(amount); }
    COLOR_API void ShiftBlackLevelColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftBlackLevel(amount); }
    COLOR_API void ShiftContrastColorBuffer(ColorBuffer* colorBuffer, double amount) { colorBuffer->ShiftContrast(amount); }
    COLOR_API void AdjustContrastColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->AdjustContrast(factor); }
    COLOR_API void AdjustColorBalanceColorBuffer(ColorBuffer* colorBuffer, double redFactor, double greenFactor, double blueFactor) { colorBuffer->AdjustColorBalance(redFactor, greenFactor, blueFactor); }

    COLOR_API void OverlayImageColorBuffer(ColorBuffer* destBuffer, ColorBuffer* overlayBuffer, int x, int y, double opacity) { destBuffer->OverlayImage(*overlayBuffer, x, y, opacity); }
    COLOR_API void InvertColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Invert(); }
    COLOR_API void GrayscaleColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Grayscale(); }
    COLOR_API void SepiaColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Sepia(factor); }
    COLOR_API void PixelateColorBuffer(ColorBuffer* colorBuffer, int pixelSize) { colorBuffer->Pixelate(pixelSize); }
    COLOR_API void BlurColorBuffer(ColorBuffer* colorBuffer, int radius) { colorBuffer->Blur(radius); }
    COLOR_API void GaussianBlurColorBuffer(ColorBuffer* colorBuffer, int radius) { colorBuffer->GaussianBlur(radius); }
    COLOR_API void SharpenColorBuffer(ColorBuffer* colorBuffer, float amount) { colorBuffer->Sharpen(amount); }
    COLOR_API void CrossProcessColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->CrossProcess(factor); }
    COLOR_API void MoonlightColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Moonlight(factor); }
    COLOR_API void VintageFilmColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->VintageFilm(factor); }
    COLOR_API void TechnicolorColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Technicolor(factor); }
    COLOR_API void PolaroidColorBuffer(ColorBuffer* colorBuffer, double factor) { colorBuffer->Polaroid(factor); }
    COLOR_API void ComplementColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Complement(); }
    COLOR_API void ApplyEmbossColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->Emboss(); }
    COLOR_API void ApplyEdgeDetectColorBuffer(ColorBuffer* colorBuffer) { colorBuffer->EdgeDetect(); }
    COLOR_API void ApplyVignetteColorBuffer(ColorBuffer* colorBuffer, double strength, double radius) { colorBuffer->Vignette(strength, radius); }
    COLOR_API void ApplyTwoColorNoiseColorBuffer(ColorBuffer* colorBuffer, double density, Color* saltColor, Color* pepperColor) { colorBuffer->TwoColorNoise(density, *saltColor, *pepperColor); }
    COLOR_API void ApplyGaussianNoiseColorBuffer(ColorBuffer* colorBuffer, double mean, double stdDev) { colorBuffer->GaussianNoise(mean, stdDev); }
    COLOR_API void ApplyPerlinNoiseColorBuffer(ColorBuffer* colorBuffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { colorBuffer->PerlinNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplySimplexNoiseColorBuffer(ColorBuffer* colorBuffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { colorBuffer->SimplexNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyFractalBrownianMotionColorBuffer(ColorBuffer* colorBuffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { colorBuffer->FractalBrownianMotion(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyVoronoiDiagramColorBuffer(ColorBuffer* colorBuffer, int numberOfPoints, double falloff, double strength) { colorBuffer->Voronoi(numberOfPoints, falloff, strength); }
    COLOR_API void PlasmaEffectColorBuffer(ColorBuffer* colorBuffer, double frequency, double phase) { colorBuffer->Plasma(frequency, phase); }
    COLOR_API void DiamondSquareEffectColorBuffer(ColorBuffer* colorBuffer, double roughness, double waterLevel, double levelsPerStop) { colorBuffer->DiamondSquare(roughness, waterLevel, levelsPerStop); }
    COLOR_API void PosterizeColorBuffer(ColorBuffer* colorBuffer, int levels) { colorBuffer->Posterize(levels); }
    #pragma endregion

    #pragma region Utility
    COLOR_API void FlipColorBuffer(ColorBuffer* colorBuffer, bool horizontal) { colorBuffer->Flip(horizontal); }
    COLOR_API void CropColorBuffer(ColorBuffer* colorBuffer, int x, int y, int width, int height) { colorBuffer->Crop(x, y, width, height); }
    COLOR_API Color* AverageColorBuffer(ColorBuffer* colorBuffer) { return new Color(colorBuffer->CalculateAverageColor(0, 0, colorBuffer->GetWidth(), colorBuffer->GetHeight())); }
    COLOR_API ColorBuffer* CopyColorBufferRegion(ColorBuffer* colorBuffer, int x, int y, int w, int h) { return colorBuffer->CopyRegion(x, y, w, h); }
    COLOR_API ColorBuffer* CopyColorBuffer(ColorBuffer* colorBuffer) { return colorBuffer->Copy(); }

    COLOR_API int ColorBufferFind(ColorBuffer* buffer, Color* color) { return buffer->Find(*color); };
    COLOR_API int ColorBufferFindLast(ColorBuffer* buffer, Color* color) { return buffer->FindLast(*color); }
    COLOR_API void ColorBufferForEach(ColorBuffer* buffer, void (*func)(Color*)) { buffer->ForEach([func](const Color& color) { func(const_cast<Color*>(&color)); }); }
    COLOR_API void ColorBufferSwap(ColorBuffer* buffer, int index1, int index2) { buffer->Swap(index1, index2); }
    COLOR_API ColorBuffer* ColorBufferFilter(ColorBuffer* buffer, bool (*predicate)(Color*)) { return new ColorBuffer(buffer->Filter([predicate](const Color& color) { return predicate(const_cast<Color*>(&color)); })); }
    COLOR_API int ColorBufferCount(ColorBuffer* buffer, Color* color) { return buffer->Count(*color); }
    COLOR_API void ColorBufferShuffle(ColorBuffer* buffer) { buffer->Shuffle(); }
    COLOR_API void ColorBufferClear(ColorBuffer* buffer) { buffer->Clear(); }
    COLOR_API void ColorBufferSort(ColorBuffer* buffer, int (*compare)(Color*, Color*)) { buffer->Sort([compare](const Color& a, const Color& b) { return compare(const_cast<Color*>(&a), const_cast<Color*>(&b)) < 0; }); }
    COLOR_API void ColorBufferAppendRight(ColorBuffer* buffer, ColorBuffer* other) { buffer->AppendRight(*other); }
    COLOR_API void ColorBufferAppendBottom(ColorBuffer* buffer, ColorBuffer* other) { buffer->AppendBottom(*other); }

    COLOR_API void MapColorsInBuffer(ColorBuffer* colorBuffer, int x, int y, int width, int height, void* mapFunction)
    {
        auto func = reinterpret_cast<unsigned int(*)(int, int, unsigned int)>(mapFunction);
        colorBuffer->MapColors(x, y, width, height, func);
    }

    COLOR_API int* ColorBufferFindAll(ColorBuffer* buffer, Color* color, int* count)
    {
        auto result = buffer->FindAll(*color);
        *count = result.size();
        int* arr = new int[*count];
        std::copy(result.begin(), result.end(), arr);
        return arr;
    }

    COLOR_API void ColorBufferApplyMatrix(ColorBuffer* buffer,
        double m11, double m12, double m13, double m14, double m15,
        double m21, double m22, double m23, double m24, double m25,
        double m31, double m32, double m33, double m34, double m35,
        double m41, double m42, double m43, double m44, double m45,
        double m51, double m52, double m53, double m54, double m55)
    {
        ColorMatrix matrix;
        double m[25] = {
            m11, m12, m13, m14, m15,
            m21, m22, m23, m24, m25,
            m31, m32, m33, m34, m35,
            m41, m42, m43, m44, m45,
            m51, m52, m53, m54, m55
        };

        for(int i = 0; i < 25; i++) matrix.data[i/5][i%5] = m[i];

        #pragma omp parallel for
        for (int i = 0; i < buffer->GetWidth() * buffer->GetHeight(); i++)
        {
            Color color = buffer->GetAt(i);
            buffer->SetAt(i, color * matrix);
        }
    }

    COLOR_API HBITMAP ExportColorBufferAsHBitmap(ColorBuffer* colorBuffer, int targetWidth, int targetHeight) { return colorBuffer->ToHBITMAP(targetWidth, targetHeight); }
    #pragma endregion
}