#pragma once

#include "../Showcase.hpp"

extern "C"
{
    using namespace KTLib;

    SHOWCASE_API int ShowColorShowcase(Color* color, const char* title);
    SHOWCASE_API int ShowGradientShowcase(Gradient* gradient, const char* title);
    SHOWCASE_API void ShowColorBuffer(ColorBuffer* buffer, const char* title);
}