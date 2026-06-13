#pragma once

// Pure geometry for the PFD. No X-Plane, no logic — just the numbers every
// PFD element has to agree on. Lives in Displays/Common, beside the planned
// Colors.h / Fonts.h. Notice: zero includes. That's the proof it's pure data.


// ------------------------------------------------------
// DRAWING HELPERS
// ------------------------------------------------------

struct Rect { float left, right, top, bottom; };

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
