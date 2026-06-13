#pragma once

// Pure geometry for the PFD. No X-Plane, no logic, just the numbers every
// PFD element has to agree on.
// Zero includes to ensure it's pure data.

#include <iostream>
#include <cmath>     // Required for std::abs

// ------------------------------------------------------
// DRAWING HELPERS
// ------------------------------------------------------

struct Rect { 
	float left, right, top, bottom; 
	float midX() const   { return (left + right) * 0.5f; }
	float midY() const   { return (top + bottom) * 0.5f; }
	float height() const { return std::abs(top - bottom); }
	float width() const  { return std::abs(right - left); }
};

inline constexpr Rect Inflate(Rect r, float b)
{
    return { r.left - b, r.right + b, r.top + b, r.bottom - b };
}

inline constexpr Rect Offset(Rect r, float dx, float dy)
{
    return { r.left + dx, r.right + dx, r.top + dy, r.bottom + dy };
}

inline constexpr Rect MirrorX(Rect r)   // mirror about local x = 0
{
    return { -r.right, -r.left, r.top, r.bottom };
}
