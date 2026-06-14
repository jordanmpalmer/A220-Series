#include "Core/Common.h"
#include "Displays/PFD/SpeedTape.h"

#include "Core/State/FlightState.h"
#include "Displays/PFD/Layout.h"
#include "Displays/PFD/Theme.h"
#include "Displays/Common/Draw.h"
#include "Displays/Common/Colors.h"

#include "XPLMGraphics.h"

namespace PFD
{
	constexpr float kSelBoxFrac = 0.10f;   // selector = top 10% of the frame

	//static void DrawFrameWithAlpha(const Rect& frame, )

	static void DrawSelectorBox(const Rect& frame, float selectedKt)
	{
		FillRectColor(frame, Colors::Black, kOverlayFillAlpha);
		//char buf[8];
		//std::snprintf(buf, sizeof buf, "%.0f", selectedKt);
		//DrawText(frame.midX(), frame.midY(), buf, Colors::kMagenta);
	}

	static void DrawScale(const Rect& frame, float kt)
	{
		// for each visible knot value, position relative to f and the live speed:
		FillRectColor(frame, Colors::Black, kOverlayFillAlpha);
		//   DrawText(f.left + m, tickY, label, kWhite);  // the number — TEXT
	}

	static void DrawReadoutBox(const Rect& frame, float kt)
	{
		//const float midY = (f.top + f.bottom) * 0.5f;
		//FillRect(frame);                            // boxed background — SHAPE
		//char buf[8]; std::snprintf(buf, sizeof buf, "%.0f", kt);
		//DrawText(f.right - m, midY, buf, kWhite);     // current speed   — TEXT	}
	}

	void DrawSpeedTape(const Rect& frameScale, const Rect& frameSel, const FlightState& flight)
	{
		DrawScale(frameScale, flight.indicatedAirspeedKt);
		DrawSelectorBox(frameSel, flight.indicatedAirspeedKt);
		//DrawReadoutBox(frame, flight.indicatedAirspeedKt);
	}
}
