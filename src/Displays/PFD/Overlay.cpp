#include "Core/Common.h"
#include "Displays/PFD/Overlay.h"

#include "Core/AircraftState.h"
#include "Core/State/GuidanceModeState.h"
#include "Displays/Common/Geometry.h" // Rect
#include "Displays/PFD/AltitudeTape.h"
#include "Displays/PFD/FMA.h"
#include "Displays/PFD/SpeedTape.h"
#include "Displays/PFD/VSI.h"

#include "XPLMGraphics.h"

namespace PFD
{
	struct OverlayFrames { Rect speedScale, speedSel, altScale, altSel, fma, vsi; };

	static OverlayFrames MakeOverlayFrames(const AttitudeRegion& region)
	{
		const float w = region.width;
		const float h = region.height;
		const float cx = region.centerX;
		const float cy = region.centerY;

		const float fmaTop    = cy + 0.5f * h;
		const float fmaBottom = cy + 0.4f * h;
		const float tapeTop    = cy + 0.358f * h;
		const float tapeBottom = cy - 0.360f * h;

		const float speedL     = cx - 0.275f * w;
		const float speedR     = cx - 0.210f * w;
		const float altL       = cx + 0.188f * w;
		const float altR       = cx + 0.272f * w;

		const float gap     = 0.009f * h;
		const float selH    = 0.025f * h;
		const float vsiL    = altR + gap;
		const float vsiR    = vsiL + 0.035f * w;
		const float vsiVGap = 0.02 * h;

		const Rect speedSel = { speedL, speedR, tapeTop + gap + selH, tapeTop + gap };
		const Rect altSel   = { altL,   altR,   tapeTop + gap + selH, tapeTop + gap };

		const Rect speedScale = { speedL, speedR, tapeTop,           tapeBottom };
		const Rect altScale   = { altL,   altR,   tapeTop,           tapeBottom };
		const Rect vsi        = { vsiL,   vsiR,   tapeTop - vsiVGap, tapeBottom + vsiVGap };

		const Rect fma = { speedL, altR, fmaTop, fmaBottom };


		return { speedScale, speedSel, altScale, altSel, fma, vsi };
	}

	void DrawOverlay(const AircraftState& aircraft, const AttitudeRegion& region)
	{

		const OverlayFrames frames = MakeOverlayFrames(region);

		XPLMSetGraphicsState(0, 0, 0, 0, 1, 0, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

		DrawSpeedTape(frames.speedScale, frames.speedSel, aircraft.flight);
		DrawAltitudeTape(frames.altScale, frames.altSel, aircraft.flight);
		DrawFMA(frames.fma, {}); // will pass in aircraft.guidance or equivalent guidance system struct
		DrawVSI(frames.vsi, aircraft.flight);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}
