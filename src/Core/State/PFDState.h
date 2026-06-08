#pragma once

struct PFDState
{
    bool  powered = false;
    float brightness = 1.0f;   // 0.0 - 1.0

    // Add PFD-specific display state here as you build it
    // e.g. which failure flags are shown, FMA mode strings etc.
};
