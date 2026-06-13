#pragma once

#include "Core/State/GuidanceModeState.h"
#include "Displays/Common/Geometry.h" // Rect

namespace PFD
{
	void DrawFMA(const Rect& frame, const GuidanceMode& modes); // will need to be swapped to GuidanceMode struct
}