#pragma once

#include "XPLMDisplay.h"

struct AvionicsState
{
    XPLMAvionicsID pfdPilot = nullptr;
    XPLMAvionicsID pfdCopilot = nullptr;
    XPLMAvionicsID ndPilot = nullptr;
    XPLMAvionicsID ndCopilot = nullptr;
    XPLMAvionicsID ecam1 = nullptr;
    XPLMAvionicsID ecam2 = nullptr;
};