#include "Core/Common.h"
#include "Displays/PFD/SpeedTape.h"

#include "Core/State/FlightState.h"
#include "Displays/PFD/Layout.h"
#include "Displays/PFD/Theme.h"
#include "Displays/Common/Draw.h"
#include "Displays/Common/Colors.h"

#include "XPLMGraphics.h"

#include <iostream>
#include <array>

namespace PFD
{
	static void DrawScale(const Rect& frame, float vs)
	{
		// for each visible knot value, position relative to f and the live speed:
		float cornerOffset = 0.1f * frame.height();
		float topOffset = frame.top - cornerOffset;
		float botOffset = frame.bottom + cornerOffset;

		Point p1 = { frame.left,  frame.top };
		Point p2 = { frame.right, topOffset };
		Point p3 = { frame.right, botOffset };
		Point p4 = { frame.left,  frame.bottom };

		std::array<Point, 4> pts = { p1, p2, p3, p4 };

		FillPoly(pts, Colors::Black, kOverlayFillAlpha);

		float rTickBorder = 0.2f * frame.width();
		float lTickBorder = 0.35f * frame.width();
		std::array<float, 10> ticks = { 0.96f, 0.88f, 0.8f, 0.66f, 0.58f, 0.42f, 0.34f, 0.2f, 0.12f, 0.04f };

		glEnable(GL_SCISSOR_TEST);
		glScissor(frame.left + lTickBorder, frame.bottom, frame.width() - lTickBorder - rTickBorder, frame.height());

		for (float tick : ticks)
		{
			Point p1 = { frame.left,  frame.bottom + tick * frame.height() };
			Point p2 = { frame.right, botOffset + tick * (topOffset - botOffset )};
			DrawLine(p1, p2, 2.0f, Colors::White);
		}

		glDisable(GL_SCISSOR_TEST);
		//glEnable(GL_SCISSOR_TEST);

		//glScissor(frame.left + smallGap, frame.bottom, frame.width(), frame.height());

		//   DrawText(f.left + m, tickY, label, kWhite);  // the number — TEXT
	}

	static void DrawTicks(const Rect& frame)
	{


	}

	void DrawVSI(const Rect& frame, const FlightState& flight)
	{
		DrawScale(frame, flight.verticalSpeedFpm);
	}
}