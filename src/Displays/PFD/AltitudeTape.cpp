#include "Core/Common.h"
#include "Displays/PFD/AltitudeTape.h"

#include "Core/State/FlightState.h"
#include "Displays/PFD/Layout.h"
#include "Displays/PFD/Theme.h"
#include "Displays/Common/Draw.h"
#include "Displays/Common/Colors.h"

#include "XPLMGraphics.h"

namespace PFD
{
	static void DrawScale(const Rect& frame, float alt)
	{
		// for each visible knot value, position relative to frame and the live speed:
		FillRectColor(frame, Colors::Black, kOverlayFillAlpha);
		//   DrawText(frame.left + m, tickY, label, kWhite);  // the number — TEXT
		return;
	}

	static void DrawReadoutBox(const Rect& frame, float alt)
	{
		//const float midY = (frame.top + frame.bottom) * 0.5f;
		FillRectColor(frame, Colors::Black, kOverlayFillAlpha);
		//char buf[8]; std::snprintf(buf, sizeof buf, "%.0f", kt);
		//DrawText(frame.right - m, midY, buf, kWhite);     // current altitude   — TEXT	}
		return;
	}

	void DrawAltitudeTape(const Rect& frameScale, const Rect& frameSel, const FlightState& flight)
	{
		DrawScale(frameScale, flight.altitudeFt);
		DrawReadoutBox(frameSel, flight.altitudeFt);
	}
}
