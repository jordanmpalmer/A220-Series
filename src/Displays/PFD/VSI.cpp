#include "Core/Common.h"
#include "Displays/PFD/VSI.h"

#include "Core/State/FlightState.h"
#include "Displays/PFD/Theme.h"
#include "Displays/Common/Geometry.h"
#include "Displays/Common/Draw.h"
#include "Displays/Common/Colors.h"

#include <array>
#include <cmath>
#include <algorithm>
#include <cstdio>

namespace PFD
{
	constexpr std::array<float, 6> kTickValues = { 0.0f, 500.0f, 1000.0f, 2000.0f, 4000.0, 6000.0f };
	constexpr float kMaxVs = 6000.0f;
	constexpr float kCornerInset = 0.15f;   // right-corner taper, as a fraction of height

	// horizontal offsets from the left edge, as fractions of width
	constexpr float kTickLeftFrac = 0.42f;   // tick start
	constexpr float kTickRightFrac = 0.675f;   // tick end / needle drop line
	constexpr float kNeedleLeftFrac = 0.075f;  // needle pivot

	constexpr float kTickWidth = 4.0f;
	constexpr float kNeedleWidth = 5.0f;

	//// vertical speed (fpm) to y position, compressed past 2000
	//static float VsToY(float fpm, float cy, float halfH)
	//{
	//	const float a = std::fabs(fpm);
	//	const float frac = (a <= 2000.0f)
	//		? (a / 2000.0f) * 0.60f
	//		: 0.60f + (std::min(a, kMaxVs) - 2000.0f) / 4000.0f * 0.30f;

	//	return cy + (fpm < 0.0f ? -1.0f : 1.0f) * frac * halfH;
	//}

	static float VsToY(float fpm, const float cy, float halfH)
	{
		const float afpm = std::fabs(fpm);

		float frac;
		if (afpm <= 1000.0f)
		{
			frac = (afpm / 1000.0f) * 0.32f;
		}
		else if (afpm <= 2000.0f)
		{
			frac = 0.32f + (afpm - 1000.0f) / 1000.0f * 0.22f;
		}
		else if (afpm <= 3000.0f)
		{
			frac = 0.54f + (afpm - 2000.0f) / 1000.0f * 0.14f;
		}
		else
		{
			frac = 0.68f + (std::min(afpm, 6000.0f) - 3000.0f) / 3000.0f * 0.23f;
		}

		return cy + (fpm < 0.0f ? -1.0f : 1.0f) * frac * halfH;
	}

	// half-height of the tapered outline at horizontal offset dx from the left edge
	static float HalfHeightAt(float dx, float fullHalf, float taperHalf, float w)
	{
		return fullHalf + (taperHalf - fullHalf) * (dx / w);
	}

	// the point for vertical-speed value v at horizontal offset dx, rides the taper
	static Point ScalePointHeight(const Rect& frame, float v, float dx)
	{
		const float fullHalf = 0.5f * frame.height();
		const float taperHalf = fullHalf - kCornerInset * frame.height();
		const float halfHAtDx = HalfHeightAt(dx, fullHalf, taperHalf, frame.width());

		return { frame.left + dx, VsToY(v, frame.midY(), halfHAtDx) };
	}

	static void DrawBackground(const Rect& frame)
	{
		const float inset = kCornerInset * frame.height();
		const std::array<Point, 4> pts = { {
			{ frame.left,  frame.top            },
			{ frame.right, frame.top    - inset },
			{ frame.right, frame.bottom + inset },
			{ frame.left,  frame.bottom         },
		} };
		FillPolyColor(pts, Colors::Black, kOverlayFillAlpha);
	}

	static void DrawScale(const Rect& frame)
	{
		const float w = frame.width();
		for (float tick : kTickValues)
		{
			DrawLineColor(ScalePointHeight(frame,  tick, kTickLeftFrac  * w),
						  ScalePointHeight(frame,  tick, kTickRightFrac * w), kTickWidth, Colors::White);
			DrawLineColor(ScalePointHeight(frame, -tick, kTickLeftFrac  * w),
				          ScalePointHeight(frame, -tick, kTickRightFrac * w), kTickWidth, Colors::White);
		}
	}

	static void DrawNeedle(const Rect& frame, float fpm)
	{
		const float w = frame.width();

		// pointer: from the inner pivot out to the right edge, riding the taper
		DrawLineColor(ScalePointHeight(frame, fpm, kNeedleLeftFrac * w),
					  ScalePointHeight(frame, fpm, w), kNeedleWidth, Colors::White);

		// drop line: from the pointer down to the zero line
		const Point top = ScalePointHeight(frame, fpm, kTickRightFrac * w);

		DrawLineColor(top, { top.x, frame.midY() }, kNeedleWidth, Colors::White);
	}

	void DrawVSI(const Rect& frame, const FlightState& flight)
	{
		DrawBackground(frame);
		DrawScale(frame);
		DrawNeedle(frame, flight.verticalSpeedFpm);
	}
}
