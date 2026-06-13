#pragma once

#include "Displays/Common/Geometry.h" // Rect
#include "Core/State/FlightState.h"

namespace PFD
{
	void DrawAltitudeTape(const Rect& frame, const FlightState& flight);
}