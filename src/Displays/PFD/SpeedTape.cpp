#include "Core/Common.h"
#include "Displays/PFD/SpeedTape.h"

#include "Core/State/FlightState.h"
#include "Displays/PFD/Layout.h"
#include "Displays/Common/Draw.h"


#include "XPLMGraphics.h"

namespace PFD
{
	static void DrawScale(const Rect& f, float kt)
	{
		// for each visible knot value, position relative to f and the live speed:
		//   FillRect(tickRect);                     // the tick mark   — a SHAPE
		//   DrawText(f.left + m, tickY, label, kWhite);  // the number — TEXT
		return;
	}

	static void DrawReadoutBox(const Rect& f, float kt)
	{
		//const float midY = (f.top + f.bottom) * 0.5f;
		//FillRect(boxRect);                            // boxed background — SHAPE
		//char buf[8]; std::snprintf(buf, sizeof buf, "%.0f", kt);
		//DrawText(f.right - m, midY, buf, kWhite);     // current speed   — TEXT	}
	}

	void DrawSpeedTape(const Rect& frame, const FlightState& flight)
	{
		XPLMSetGraphicsState(0, 0, 0, 0, 1, 0, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

		glColor4f(1.0f, 0.0f, 0.0f, 0.3f);

		FillRect(frame);
		DrawScale(frame, flight.indicatedAirspeedKt);
		DrawReadoutBox(frame, flight.indicatedAirspeedKt);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}
