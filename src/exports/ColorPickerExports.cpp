#include "../../include/exports/ColorPickerExports.h"

extern "C"
{
    COLORPICKER_API ColorPicker* ColorPickerCreate(const char* colorSpace, const char* format) { return new ColorPicker(colorSpace, format); }
    COLORPICKER_API void ColorPickerDestroy(ColorPicker* picker) { delete picker; }

    COLORPICKER_API void ColorPickerStart(ColorPicker* picker) { picker->Start(); }
    COLORPICKER_API void ColorPickerStop(ColorPicker* picker) { picker->Stop(); }
    COLORPICKER_API void ColorPickerToggle(ColorPicker* picker) { picker->Toggle(); }
    COLORPICKER_API bool ColorPickerIsActive(ColorPicker* picker) { return picker->IsActive(); }
    COLORPICKER_API Color* ColorPickerGetCurrentColor(ColorPicker* picker) { return new Color(picker->GetCurrentColor()); }

    COLORPICKER_API const char* ColorPickerGetFormattedValue(ColorPicker* picker)
    {
        static std::string value;
        value = picker->GetFormattedValue();
        return value.c_str();
    }

    COLORPICKER_API void ColorPickerSetPosition(ColorPicker* picker, int x, int y) { picker->SetPosition(x, y); }
    COLORPICKER_API void ColorPickerGetPosition(ColorPicker* picker, int* x, int* y) { picker->GetPosition(*x, *y); }
    COLORPICKER_API void ColorPickerMove(ColorPicker* picker, int deltaX, int deltaY) { picker->Move(deltaX, deltaY); }
    COLORPICKER_API void ColorPickerSetPreviewSize(ColorPicker* picker, int width, int height) { picker->SetPreviewSize(width, height); }
    COLORPICKER_API void ColorPickerSetCaptureSize(ColorPicker* picker, int size) { picker->SetCaptureSize(size); }
    COLORPICKER_API void ColorPickerSetZoomLevel(ColorPicker* picker, float zoom) { picker->SetZoomLevel(zoom); }
    COLORPICKER_API void ColorPickerZoomIn(ColorPicker* picker, float amount) { picker->ZoomIn(amount); }
    COLORPICKER_API void ColorPickerZoomOut(ColorPicker* picker, float amount) { picker->ZoomOut(amount); }
    COLORPICKER_API void ColorPickerIncreaseCaptureSize(ColorPicker* picker, int amount) { picker->IncreaseCaptureSize(amount); }
    COLORPICKER_API void ColorPickerDecreaseCaptureSize(ColorPicker* picker, int amount) { picker->DecreaseCaptureSize(amount); }
    COLORPICKER_API void ColorPickerSetFontSize(ColorPicker* picker, int size) { picker->SetFontSize(size); }
    COLORPICKER_API void ColorPickerSetFontName(ColorPicker* picker, const wchar_t* name) { picker->SetFontName(name); }
    COLORPICKER_API void ColorPickerSetTextColor(ColorPicker* picker, Color* color) { picker->SetTextColor(*color); }
    COLORPICKER_API void ColorPickerSetTextBackgroundColor(ColorPicker* picker, Color* color) { picker->SetTextBackgroundColor(*color); }
    COLORPICKER_API void ColorPickerSetBorderColor(ColorPicker* picker, Color* color) { picker->SetBorderColor(*color); }
    COLORPICKER_API void ColorPickerSetViewModeColor(ColorPicker* picker, Color* color) { picker->SetViewModeColor(*color); }
    COLORPICKER_API void ColorPickerSetHighlightColor(ColorPicker* picker, Color* color) { picker->SetHighlightColor(*color); }
    COLORPICKER_API void ColorPickerSetViewMode(ColorPicker* picker, int mode) { picker->SetViewMode(static_cast<ColorPicker::ViewMode>(mode)); }
    COLORPICKER_API void ColorPickerToggleViewMode(ColorPicker* picker, int direction) { picker->ToggleViewMode(direction); }
    COLORPICKER_API void ColorPickerToggleHighlight(ColorPicker* picker) { picker->ToggleHighlight(); }

    COLORPICKER_API void ColorPickerSetOnStart(ColorPicker* picker, void(*callback)()) { picker->SetOnStart(callback); }
    COLORPICKER_API void ColorPickerSetOnUpdate(ColorPicker* picker, void(*callback)(Color*)) { picker->SetOnUpdate(callback); }
    COLORPICKER_API void ColorPickerSetOnExit(ColorPicker* picker, void(*callback)(Color*)) { picker->SetOnExit(callback); }
}
