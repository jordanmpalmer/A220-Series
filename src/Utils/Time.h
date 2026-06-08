#pragma once
#include <algorithm>

namespace Time
{
	constexpr float MAX_DT = 0.1f;     // guard against large time steps (e.g. from pausing or debugging)
    constexpr float MIN_DT = 0.0001f;  // guard against zero/negative

    inline float Clamp(float dt)
    {
        return std::clamp(dt, MIN_DT, MAX_DT);
    }
}