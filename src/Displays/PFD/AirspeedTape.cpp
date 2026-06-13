#include "Core/Common.h"

#include "Displays/PFD/AirspeedTape.h"

#include "Displays/PFD/Layout.h"
#include "Displays/Common/Draw.h"

#include "XPLMGraphics.h"

namespace PFD
{
	// ------------------------------------------------------
	// SPEED TAPE
	// ------------------------------------------------------

	void DrawTapes(const PFD::AttitudeRegion& region)
	{
		XPLMSetGraphicsState(0, 0, 0, 0, 1, 0, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

		glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

		const float w = region.width;
		const float h = region.height;
		const float cx = region.centerX;
		const float cy = region.centerY;

		const float tapeTop    = cy + 0.36f * h;
		const float tapeBottom = cy - 0.363f * h;
		const float speedRight = cx - 0.21f * w;
		const float speedLeft  = cx - 0.28f * w;
		const float altLeft    = cx + 0.183f * w;
		const float altRight   = cx + 0.265f * w;
		const float gap        = 0.06f * h;

		const Rect speedTape = { speedLeft, speedRight, tapeTop, tapeBottom };
		const Rect altTape   = { altLeft,   altRight,   tapeTop, tapeBottom };

		FillRect(speedTape);
		FillRect(altTape);

		const Rect topTape = { speedLeft, altRight, cy + 0.5f * h , tapeTop + gap };
		FillRect(topTape);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}
