#pragma once

#include "Displays/Common/Geometry.h" // Rect
#include "Core/State/FlightState.h" // FlightState

namespace PFD
{
	void DrawSpeedTape(const Rect& frame, const FlightState& flight);
}
