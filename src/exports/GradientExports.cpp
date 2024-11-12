#include "../../include/exports/GradientExports.h"

using namespace KTLib;

extern "C"
{
    #pragma region Constructors
    GRADIENT_API Gradient* CreateGradientFromSteps(int totalSteps) { return new Gradient(totalSteps); }
    GRADIENT_API Gradient* CreateGradientFromGradient(Gradient* gradient) { return new Gradient(*gradient); }
    GRADIENT_API Gradient* CreateGradientFromColors(int totalSteps, unsigned int* colors, int colorCount)
    {
        std::vector<unsigned int> colorVec(colors, colors + colorCount);
        return new Gradient(totalSteps, colorVec);
    }
    #pragma endregion

    #pragma region Gradient Functions
    GRADIENT_API void GradientDeleteGradient(Gradient* gradient) { delete gradient; }
    GRADIENT_API void GradientSetAngle(Gradient* gradient, float degrees) { gradient->SetAngle(degrees); }
    GRADIENT_API float GradientGetAngle(Gradient* gradient) { return gradient->GetAngle(); }
    GRADIENT_API int GradientGetVertices(Gradient* gradient) { return gradient->GetVertices(); }
    GRADIENT_API void GradientSetVertices(Gradient* gradient, int vertices) { gradient->SetVertices(vertices); }
    GRADIENT_API void GradientSetType(Gradient* gradient, int type) { gradient->SetType(static_cast<GradientType>(type)); }
    GRADIENT_API int GradientGetType(Gradient* gradient) { return static_cast<int>(gradient->GetType()); }
    GRADIENT_API void GradientAddColorStop(Gradient* gradient, unsigned int color, float position) { gradient->AddColorStop(color, position); }
    GRADIENT_API void GradientRemoveColorStop(Gradient* gradient, float position) { gradient->RemoveColorStop(position); }
    GRADIENT_API Color* GradientGetColorAt(Gradient* gradient, float position) { return new Color(gradient->GetColorAt(position)); }
    GRADIENT_API void GradientRotate(Gradient* gradient, float angle) { gradient->Rotate(angle); }
    GRADIENT_API void GradientReverse(Gradient* gradient) { gradient->Reverse(); }
    GRADIENT_API void GradientShift(Gradient* gradient, float amount) { gradient->Shift(amount); }

    #pragma endregion

    #pragma region Color Modification Functions
    GRADIENT_API void GradientShiftHue(Gradient* gradient, double degrees) { gradient->ShiftHue(degrees); }
    GRADIENT_API void GradientShiftSaturation(Gradient* gradient, double amount) { gradient->ShiftSaturation(amount); }
    GRADIENT_API void GradientShiftLightness(Gradient* gradient, double amount) { gradient->ShiftLightness(amount); }
    GRADIENT_API void GradientShiftWhiteness(Gradient* gradient, double amount) { gradient->ShiftWhiteLevel(amount); }
    GRADIENT_API void GradientShiftBlackness(Gradient* gradient, double amount) { gradient->ShiftBlackLevel(amount); }
    GRADIENT_API void GradientShiftValue(Gradient* gradient, double amount) { gradient->ShiftValue(amount); }
    GRADIENT_API void GradientGrayscale(Gradient* gradient) { gradient->Grayscale(); }
    GRADIENT_API void GradientSepia(Gradient* gradient, double factor) { gradient->Sepia(factor); }
    GRADIENT_API void GradientInvert(Gradient* gradient) { gradient->Invert(); }
    GRADIENT_API void GradientComplement(Gradient* gradient) { gradient->Complement(); }
    GRADIENT_API void GradientSetTotalSteps(Gradient* gradient, int totalSteps) { gradient->SetTotalSteps(totalSteps); }
    GRADIENT_API int GradientGetTotalSteps(Gradient* gradient) { return gradient->GetTotalSteps(); }
    GRADIENT_API Color* GradientGetColorAtStep(Gradient* gradient, int step) { return new Color(gradient->GetColorAtStep(step)); }
    GRADIENT_API void GradientSetFocus(Gradient* gradient, float x, float y) { gradient->SetFocus(x, y); }
    GRADIENT_API void GradientGetFocus(Gradient* gradient, float* x, float* y) { gradient->GetFocus(x, y); }
    GRADIENT_API void GradientSetEdgeSharpness(Gradient* gradient, float sharpness) { gradient->SetEdgeSharpness(sharpness); }
    GRADIENT_API float GradientGetEdgeSharpness(Gradient* gradient) { return gradient->GetEdgeSharpness(); }
    GRADIENT_API void GradientSetWavelength(Gradient* gradient, float wavelength) { gradient->SetWavelength(wavelength); }
    GRADIENT_API float GradientGetWavelength(Gradient* gradient) { return gradient->GetWavelength(); }
    GRADIENT_API void GradientSetAmplitude(Gradient* gradient, float amplitude) { gradient->SetAmplitude(amplitude); }
    GRADIENT_API float GradientGetAmplitude(Gradient* gradient) { return gradient->GetAmplitude(); }
    GRADIENT_API void GradientSetRepetitions(Gradient* gradient, float repetitions) { gradient->SetRepetitions(repetitions); }
    GRADIENT_API float GradientGetRepetitions(Gradient* gradient) { return gradient->GetRepetitions(); }
    #pragma endregion

    #pragma region Utility
    GRADIENT_API const char* GradientSerialize(Gradient* gradient)
    {
        static std::string serializedData;
        serializedData = gradient->Serialize();
        return serializedData.c_str();
    }

    GRADIENT_API Gradient* GradientDeserialize(const char* data)
    {
        try
        {
            if (!data || !*data) return nullptr;

            auto gradient = new Gradient(Gradient::Deserialize(data));
            return gradient;
        }
        catch (const std::exception& e)
        {
            return nullptr;
        }
    }

    GRADIENT_API HBITMAP GradientCreateHBITMAP(Gradient* gradient, int width, int height) { return gradient->CreateHBITMAP(width, height); }
    #pragma endregion
}