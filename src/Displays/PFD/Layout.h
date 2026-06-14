#pragma once 

namespace PFD
{
    // ---- Panel resolution ----
    inline constexpr int kScreenWidth = 2560;
    inline constexpr int kScreenHeight = 2048;

    inline constexpr float kPitchPixelsPerDegree = 17.5f;  // horizon travel per deg pitch
    inline constexpr float kHorizonSplit = 0.375f;  // fraction below attitude region (ND)

    // The attitude region: the rectangle the artificial horizon lives in, plus
    // the point the horizon and pitch ladder rotate about. Built once from the
    // screen size so horizon, reference symbol, and ladder share one set of numbers.
    struct AttitudeRegion
    {
        int   left;
        int   bottom;
        int   width;
        int   height;
        float centerX;
        float centerY;
    };

    inline constexpr AttitudeRegion MakeAttitudeRegion(int screenWidth, int screenHeight)
    {
        const int bottom = static_cast<int>(screenHeight * kHorizonSplit);
        const int height = screenHeight - bottom;

        return AttitudeRegion{
            0,                          // left
            bottom,                     // bottom
            screenWidth,                // width
            height,                     // height
            screenWidth * 0.5f,         // centerX
            bottom + height * 0.5f      // centerY
        };
    }
}
