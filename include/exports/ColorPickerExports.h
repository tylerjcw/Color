#pragma once

#include "../ColorPicker.hpp"

extern "C"
{
    using namespace KTLib;

    COLORPICKER_API ColorPicker* ColorPickerCreate(const char* colorSpace, const char* format);
    COLORPICKER_API void ColorPickerDestroy(ColorPicker* picker);

    COLORPICKER_API void ColorPickerStart(ColorPicker* picker);
    COLORPICKER_API void ColorPickerStop(ColorPicker* picker);
    COLORPICKER_API void ColorPickerToggle(ColorPicker* picker);
    COLORPICKER_API bool ColorPickerIsActive(ColorPicker* picker);
    COLORPICKER_API Color* ColorPickerGetCurrentColor(ColorPicker* picker);
    COLORPICKER_API const char* ColorPickerGetFormattedValue(ColorPicker* picker);

    COLORPICKER_API void ColorPickerSetPosition(ColorPicker* picker, int x, int y);
    COLORPICKER_API void ColorPickerGetPosition(ColorPicker* picker, int* x, int* y);
    COLORPICKER_API void ColorPickerMove(ColorPicker* picker, int deltaX, int deltaY);
    COLORPICKER_API void ColorPickerSetPreviewSize(ColorPicker* picker, int width, int height);
    COLORPICKER_API void ColorPickerSetCaptureSize(ColorPicker* picker, int size);
    COLORPICKER_API void ColorPickerSetZoomLevel(ColorPicker* picker, float zoom);
    COLORPICKER_API void ColorPickerZoomIn(ColorPicker* picker, float amount);
    COLORPICKER_API void ColorPickerZoomOut(ColorPicker* picker, float amount);
    COLORPICKER_API void ColorPickerIncreaseCaptureSize(ColorPicker* picker, int amount);
    COLORPICKER_API void ColorPickerDecreaseCaptureSize(ColorPicker* picker, int amount);
    COLORPICKER_API void ColorPickerSetFontSize(ColorPicker* picker, int size);
    COLORPICKER_API void ColorPickerSetFontName(ColorPicker* picker, const wchar_t* name);
    COLORPICKER_API void ColorPickerSetTextColor(ColorPicker* picker, Color* color);
    COLORPICKER_API void ColorPickerSetTextBackgroundColor(ColorPicker* picker, Color* color);
    COLORPICKER_API void ColorPickerSetBorderColor(ColorPicker* picker, Color* color);
    COLORPICKER_API void ColorPickerSetViewModeColor(ColorPicker* picker, Color* color);
    COLORPICKER_API void ColorPickerSetHighlightColor(ColorPicker* picker, Color* color);
    COLORPICKER_API void ColorPickerSetViewMode(ColorPicker* picker, int mode);
    COLORPICKER_API void ColorPickerToggleViewMode(ColorPicker* picker, int direction);
    COLORPICKER_API void ColorPickerToggleHighlight(ColorPicker* picker);

    COLORPICKER_API void ColorPickerSetOnStart(ColorPicker* picker, void(*callback)());
    COLORPICKER_API void ColorPickerSetOnUpdate(ColorPicker* picker, void(*callback)(Color*));
    COLORPICKER_API void ColorPickerSetOnExit(ColorPicker* picker, void(*callback)(Color*));
}
