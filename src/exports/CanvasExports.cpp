#include "../../include/exports/CanvasExports.h"

extern "C"
{
    #pragma region Constructors
    COLOR_API Canvas* CreateCanvasFromArray(Color** colors, int width, int height) { return new Canvas(colors, width, height); }
    COLOR_API Canvas* CreateCanvasFromWHC(int width, int height, Color* defaultColor) { return new Canvas(width, height, *defaultColor); }
    COLOR_API Canvas* CreateCanvasFromBuffer(unsigned int* buffer, int width, int height) { return new Canvas(buffer, width, height); }
    COLOR_API Canvas* CreateCanvasFromGradient(Gradient* gradient, int width, int height) { return new Canvas(*gradient, width, height); }

    // HBITMAP
    COLOR_API HBITMAP ExportCanvasAsHBITMAP(Canvas* buffer, int targetWidth, int targetHeight) { return buffer->ToHBITMAP(targetWidth, targetHeight); }
    COLOR_API Canvas* CreateCanvasFromHBITMAP(HBITMAP hBitmap, int width, int height) { return Canvas::FromHBITMAP(hBitmap, width, height); }

    // HDC
    COLOR_API HDC ExportCanvasAsHDC(Canvas* buffer, int targetWidth, int targetHeight) { return buffer->ToHDC(targetWidth, targetHeight); }
    COLOR_API Canvas* CreateCanvasFromHDC(HDC hdc, int x, int y, int width, int height) { return Canvas::FromHDC(hdc, x, y, width, height); }

    // HICON
    COLOR_API HICON ExportCanvasAsHICON(Canvas* buffer, int targetWidth, int targetHeight) { return buffer->ToHICON(targetWidth, targetHeight); }
    COLOR_API Canvas* CreateCanvasFromHICON(HICON hIcon) { return Canvas::FromHICON(hIcon); }

    // HCURSOR
    COLOR_API HCURSOR ExportCanvasAsHCURSOR(Canvas* buffer, int targetWidth, int targetHeight) { return buffer->ToHCURSOR(targetWidth, targetHeight); }
    COLOR_API Canvas* CreateCanvasFromHCURSOR(HCURSOR hCursor) { return Canvas::FromHCURSOR(hCursor); }

    COLOR_API Canvas* CreateCanvasFromHWND(HWND hWnd, int x, int y, int width, int height) { return Canvas::FromHWND(hWnd, x, y, width, height); }
    #pragma endregion

    #pragma region Canvas Functions
    COLOR_API void ShiftRedCanvas(Canvas* buffer, int amount) { buffer->ShiftRed(amount); }
    COLOR_API void ShiftGreenCanvas(Canvas* buffer, int amount) { buffer->ShiftGreen(amount); }
    COLOR_API void ShiftBlueCanvas(Canvas* buffer, int amount) { buffer->ShiftBlue(amount); }
    COLOR_API void ShiftAlphaCanvas(Canvas* buffer, int amount) { buffer->ShiftAlpha(amount); }
    COLOR_API void SetRedCanvas(Canvas* buffer, int value) { buffer->SetRed(value); }
    COLOR_API void SetGreenCanvas(Canvas* buffer, int value) { buffer->SetGreen(value); }
    COLOR_API void SetBlueCanvas(Canvas* buffer, int value) { buffer->SetBlue(value); }
    COLOR_API void SetAlphaCanvas(Canvas* buffer, int value) { buffer->SetAlpha(value); }

    COLOR_API void RotateCanvas(Canvas* buffer, double angle) { buffer->Rotate(angle); }
    COLOR_API void ResizeCanvas(Canvas* buffer, int newWidth, int newHeight, int resizeImage, Color* fillColor) { buffer->Resize(newWidth, newHeight, resizeImage, *fillColor); }
    COLOR_API void ScaleCanvas(Canvas* buffer, double scale) { buffer->Scale(scale); }
    COLOR_API void DeleteCanvas(Canvas* buffer) { delete buffer; }

    COLOR_API int GetCanvasWidth(Canvas* buffer) { return buffer->GetWidth(); }
    COLOR_API int GetCanvasHeight(Canvas* buffer) { return buffer->GetHeight(); }
    COLOR_API int GetCanvasSize(Canvas* buffer) { return buffer->GetSize(); }
    COLOR_API int GetCanvasStride(Canvas* buffer) { return buffer->GetStride(); }
    COLOR_API Color* CanvasGetAt(Canvas* buffer, int index) { return new Color(buffer->GetAt(index)); }
    COLOR_API void CanvasSetAt(Canvas* buffer, int index, Color* color) { buffer->SetAt(index, *color); }
    COLOR_API void CanvasGetXY(Canvas* buffer, int index, int* x, int* y) { buffer->GetXY(index, *x, *y); }
    COLOR_API void CanvasGetIndex(Canvas* buffer, int x, int y, int* index) { buffer->GetIndex(x, y, *index); }

    COLOR_API unsigned int GetColorIntFromBuffer(Canvas* buffer, int x, int y) { return buffer->Get(x, y).ToInt(0); }
    COLOR_API void SetColorIntInBuffer(Canvas* buffer, int x, int y, unsigned int colorInt) { buffer->Set(x, y, Color(colorInt)); }
    COLOR_API Color* GetColorFromBuffer(Canvas* buffer, int x, int y) { return new Color(buffer->Get(x, y)); }
    COLOR_API void SetColorInBuffer(Canvas* buffer, int x, int y, Color* color) { buffer->Set(x, y, *color); }
    #pragma endregion

    #pragma region Color Modification Functions
    COLOR_API void ShiftHueCanvas(Canvas* buffer, double degrees) { buffer->ShiftHue(degrees); }
    COLOR_API void ShiftSaturationCanvas(Canvas* buffer, double amount) { buffer->ShiftSaturation(amount); }
    COLOR_API void ShiftLightnessCanvas(Canvas* buffer, double amount) { buffer->ShiftLightness(amount); }
    COLOR_API void ShiftValueCanvas(Canvas* buffer, double amount) { buffer->ShiftValue(amount); }
    COLOR_API void ShiftIntensityCanvas(Canvas* buffer, double amount) { buffer->ShiftIntensity(amount); }
    COLOR_API void ShiftWhiteLevelCanvas(Canvas* buffer, double amount) { buffer->ShiftWhiteLevel(amount); }
    COLOR_API void ShiftBlackLevelCanvas(Canvas* buffer, double amount) { buffer->ShiftBlackLevel(amount); }
    COLOR_API void ShiftContrastCanvas(Canvas* buffer, double amount) { buffer->ShiftContrast(amount); }
    COLOR_API void AdjustContrastCanvas(Canvas* buffer, double factor) { buffer->AdjustContrast(factor); }
    COLOR_API void AdjustColorBalanceCanvas(Canvas* buffer, double redFactor, double greenFactor, double blueFactor) { buffer->AdjustColorBalance(redFactor, greenFactor, blueFactor); }

    COLOR_API void OverlayImageCanvas(Canvas* destBuffer, Canvas* overlayBuffer, int x, int y, double opacity) { destBuffer->OverlayImage(*overlayBuffer, x, y, opacity); }
    COLOR_API void InvertCanvas(Canvas* buffer) { buffer->Invert(); }
    COLOR_API void GrayscaleCanvas(Canvas* buffer) { buffer->Grayscale(); }
    COLOR_API void SepiaCanvas(Canvas* buffer, double factor) { buffer->Sepia(factor); }
    COLOR_API void PixelateCanvas(Canvas* buffer, int pixelSize) { buffer->Pixelate(pixelSize); }
    COLOR_API void BlurCanvas(Canvas* buffer, int radius) { buffer->Blur(radius); }
    COLOR_API void GaussianBlurCanvas(Canvas* buffer, int radius) { buffer->GaussianBlur(radius); }
    COLOR_API void SharpenCanvas(Canvas* buffer, float amount) { buffer->Sharpen(amount); }
    COLOR_API void CrossProcessCanvas(Canvas* buffer, double factor) { buffer->CrossProcess(factor); }
    COLOR_API void MoonlightCanvas(Canvas* buffer, double factor) { buffer->Moonlight(factor); }
    COLOR_API void VintageFilmCanvas(Canvas* buffer, double factor) { buffer->VintageFilm(factor); }
    COLOR_API void TechnicolorCanvas(Canvas* buffer, double factor) { buffer->Technicolor(factor); }
    COLOR_API void PolaroidCanvas(Canvas* buffer, double factor) { buffer->Polaroid(factor); }
    COLOR_API void ComplementCanvas(Canvas* buffer) { buffer->Complement(); }
    COLOR_API void ApplyEmbossCanvas(Canvas* buffer) { buffer->Emboss(); }
    COLOR_API void ApplyEdgeDetectCanvas(Canvas* buffer) { buffer->EdgeDetect(); }
    COLOR_API void ApplyVignetteCanvas(Canvas* buffer, double strength, double radius) { buffer->Vignette(strength, radius); }
    COLOR_API void ApplyTwoColorNoiseCanvas(Canvas* buffer, double density, Color* saltColor, Color* pepperColor) { buffer->TwoColorNoise(density, *saltColor, *pepperColor); }
    COLOR_API void ApplyGaussianNoiseCanvas(Canvas* buffer, double mean, double stdDev) { buffer->GaussianNoise(mean, stdDev); }
    COLOR_API void ApplyPerlinNoiseCanvas(Canvas* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { buffer->PerlinNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplySimplexNoiseCanvas(Canvas* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { buffer->SimplexNoise(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyFractalBrownianMotionCanvas(Canvas* buffer, double frequency, double amplitude, int octaves, double persistence, double lacunarity) { buffer->FractalBrownianMotion(frequency, amplitude, octaves, persistence, lacunarity); }
    COLOR_API void ApplyVoronoiDiagramCanvas(Canvas* buffer, int numberOfPoints, double falloff, double strength) { buffer->Voronoi(numberOfPoints, falloff, strength); }
    COLOR_API void PlasmaEffectCanvas(Canvas* buffer, double frequency, double phase) { buffer->Plasma(frequency, phase); }
    COLOR_API void DiamondSquareEffectCanvas(Canvas* buffer, double roughness, double waterLevel, double levelsPerStop) { buffer->DiamondSquare(roughness, waterLevel, levelsPerStop); }
    COLOR_API void PosterizeCanvas(Canvas* buffer, int levels) { buffer->Posterize(levels); }
    #pragma endregion

    #pragma region Utility
    COLOR_API void FlipCanvas(Canvas* buffer, bool horizontal) { buffer->Flip(horizontal); }
    COLOR_API void CropCanvas(Canvas* buffer, int x, int y, int width, int height) { buffer->Crop(x, y, width, height); }
    COLOR_API Color* AverageCanvas(Canvas* buffer) { return new Color(buffer->CalculateAverageColor(0, 0, buffer->GetWidth(), buffer->GetHeight())); }
    COLOR_API Canvas* CopyCanvasRegion(Canvas* buffer, int x, int y, int w, int h) { return buffer->CopyRegion(x, y, w, h); }
    COLOR_API Canvas* CopyCanvas(Canvas* buffer) { return buffer->Copy(); }

    COLOR_API int CanvasFind(Canvas* buffer, Color* color) { return buffer->Find(*color); };
    COLOR_API int CanvasFindLast(Canvas* buffer, Color* color) { return buffer->FindLast(*color); }
    COLOR_API void CanvasForEach(Canvas* buffer, void (*func)(Color*)) { buffer->ForEach([func](const Color& color) { func(const_cast<Color*>(&color)); }); }
    COLOR_API void CanvasSwap(Canvas* buffer, int index1, int index2) { buffer->Swap(index1, index2); }
    COLOR_API Canvas* CanvasFilter(Canvas* buffer, bool (*predicate)(Color*)) { return new Canvas(buffer->Filter([predicate](const Color& color) { return predicate(const_cast<Color*>(&color)); })); }
    COLOR_API int CanvasCount(Canvas* buffer, Color* color) { return buffer->Count(*color); }
    COLOR_API size_t CanvasCountUniqueColors(Canvas* buffer) { return buffer->CountUniqueColors(); }
    COLOR_API void CanvasShuffle(Canvas* buffer) { buffer->Shuffle(); }
    COLOR_API void CanvasClear(Canvas* buffer) { buffer->Clear(); }
    COLOR_API void CanvasSort(Canvas* buffer, int (*compare)(Color*, Color*))
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

    COLOR_API void CanvasAppendRight(Canvas* buffer, Canvas* other) { buffer->AppendRight(*other); }
    COLOR_API void CanvasAppendBottom(Canvas* buffer, Canvas* other) { buffer->AppendBottom(*other); }

    COLOR_API void MapColorsInBuffer(Canvas* buffer, int x, int y, int width, int height, void* mapFunction)
    {
        auto func = reinterpret_cast<unsigned int(*)(int, int, unsigned int)>(mapFunction);
        buffer->MapColors(x, y, width, height, func);
    }

    COLOR_API int* CanvasFindAll(Canvas* buffer, Color* color, int* count)
    {
        auto result = buffer->FindAll(*color);
        *count = result.size();
        int* arr = new int[*count];
        std::copy(result.begin(), result.end(), arr);
        return arr;
    }

    COLOR_API void CanvasApplyMatrix(Canvas* buffer, ColorMatrix* matrix) { buffer->ApplyMatrix(*matrix); }
    COLOR_API void DrawCanvas(Canvas* buffer, HWND hwnd, int x, int y) { buffer->Draw(hwnd, x, y); }
    #pragma endregion
}