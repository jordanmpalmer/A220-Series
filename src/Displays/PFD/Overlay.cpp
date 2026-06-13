#include "Core/Common.h"
#include "Displays/PFD/Overlay.h"

#include "Core/AircraftState.h"
#include "Core/State/GuidanceModeState.h"
#include "Displays/Common/Geometry.h" // Rect
#include "Displays/PFD/AltitudeTape.h"
#include "Displays/PFD/FMA.h"
#include "Displays/PFD/SpeedTape.h"

#include "XPLMGraphics.h"

namespace PFD
{
	struct OverlayFrames { Rect speed, alt, fma; };

	static OverlayFrames MakeOverlayFrames(const AttitudeRegion& region)
	{
		const float w = region.width;
		const float h = region.height;
		const float cx = region.centerX;
		const float cy = region.centerY;
		//const float gap = 0.06f * h;
		const float gap = 0.01f * h;

		const float tapeTop    = cy + 0.358f * h;
		const float tapeBottom = cy - 0.36f * h;
		const Rect speed    = { cx - 0.275f * w,  cx - 0.21f * w,  tapeTop,       tapeBottom };
		const Rect alt      = { cx + 0.188f * w, cx + 0.272f * w, tapeTop,        tapeBottom };
		const Rect fma      = { speed.left,      alt.right,       cy + 0.5f * h, speed.top + gap };

		return { speed, alt, fma };
	}

	void DrawOverlay(const AircraftState& aircraft, const AttitudeRegion& region)
	{

		const OverlayFrames frames = MakeOverlayFrames(region);

		XPLMSetGraphicsState(0, 0, 0, 0, 1, 0, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

		DrawSpeedTape(frames.speed, aircraft.flight);
		DrawAltitudeTape(frames.alt, aircraft.flight);
		DrawFMA(frames.fma, {}); // will pass in aircraft.guidance or equivalent guidance system struct

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}
