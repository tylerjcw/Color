#include "../../include/exports/ShowcaseExports.h"

#include <iostream>

extern "C"
{
    SHOWCASE_API int ShowColorShowcase(Color* color, const char* title)
    {
        Showcase showcase(color, title);
        return showcase.Show();
    }

    SHOWCASE_API int ShowGradientShowcase(Gradient* gradient, const char* title)
    {
        Showcase showcase(gradient, title);
        return showcase.Show();
    }
}