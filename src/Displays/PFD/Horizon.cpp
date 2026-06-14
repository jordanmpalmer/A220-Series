#include "Core/Common.h"

#include "Displays/PFD/Horizon.h"

#include "Core/AircraftState.h"
#include "Displays/Common/Draw.h"
#include "Displays/Common/Colors.h"
#include "Displays/PFD/Layout.h"

#include "XPLMGraphics.h"
#include "XPLMDisplay.h"

#include <cmath>

namespace PFD
{
	// ------------------------------------------------------
	// CONSTANTS
	// ------------------------------------------------------

	// Sky and ground colors (Airbus-style)
	constexpr float kSkyR = 0.243, kSkyG = 0.341f, kSkyB = 0.690f;
	constexpr float kGndR = 0.435f, kGndG = 0.259f, kGndB = 0.137f;

	constexpr float kPi = 3.14159265f;
	constexpr float kHalfPi = 1.5707963f;

	// Corner rounding radius in pixels.
	//constexpr float kCornerRadius = 80.0f;

	// ------------------------------------------------------
	// HORIZON
	// ------------------------------------------------------

	void DrawHorizon(const AircraftState& aircraft, const AttitudeRegion& region)
	{
		const float pitch = aircraft.flight.pitchDeg;
		const float roll = aircraft.flight.rollDeg;

		const float size = std::sqrt(static_cast<float>(kScreenWidth * kScreenWidth +
			kScreenHeight * kScreenHeight));

		//int margin = static_cast<int>(screenWidth * 0.25);

		// Clip all horizon drawing to the upper region only
		glEnable(GL_SCISSOR_TEST);
		//glScissor(0 + margin, horizonBottom, screenWidth - 2 * margin, horizonHeight);
		glScissor(0, region.bottom, region.width, region.height);

		XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(region.centerX, region.centerY, 0.0f);
		glRotatef(roll, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, -pitch * kPitchPixelsPerDegree, 0.0f);

		// Sky
		glColor4f(kSkyR, kSkyG, kSkyB, 1.0f);
		FillRectPoints(-size, size, size, 0.0f);

		// Ground
		glColor4f(kGndR, kGndG, kGndB, 1.0f);
		FillRectPoints(-size, size, 0.0f, -size);

		// Horizon line
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glLineWidth(3.0f);
		glBegin(GL_LINES);
			glVertex2f(-size, 0.0f);
			glVertex2f(size, 0.0f);
		glEnd();

		constexpr float kStepDeg = 2.5f;
		constexpr int   kMaxStep = 36;      // ±90°
		constexpr float kMinorBelow = 10.0f;   // 2.5° marks: 10° below the horizon …
		constexpr float kMinorAbove = 20.0f;   //             … to 20° above

		const float thick = 0.003f * region.height;

		float bottomClamp = 0.35f * region.height;
		float topClamp    = bottomClamp + 0.25f * region.height;
		glScissor(0, region.centerY - bottomClamp, region.width, topClamp);

		for (int i = -kMaxStep; i <= kMaxStep; ++i)
		{
			if (i == 0) continue;

			const float deg = i * kStepDeg;
			const bool  major = (i % 4 == 0);   // 10°
			const bool  medium = (i % 2 == 0);   // 5°
			const bool  minor = !medium;        // 2.5°

			// fine marks only near the horizon; major/medium run the full range
			if (minor && (deg < -kMinorBelow || deg > kMinorAbove)) continue;

			const float halfW = (major ? 0.046f : medium ? 0.024f : 0.01f) * region.width;
			const float y = deg * kPitchPixelsPerDegree;
			DrawLineColor(Point{ -halfW, y }, Point{ halfW, y }, thick, Colors::White);
		}

		glPopMatrix();

		glDisable(GL_SCISSOR_TEST);
	}

	// ------------------------------------------------------
	// Aircraft Symbol
	// ------------------------------------------------------

	// Proportions, as fractions of the attitude-region width.
	constexpr float kBoxHalf  = 0.0025f;
	constexpr float kOutline  = 0.001f;
	constexpr float kGap      = 0.04625f;
	constexpr float kWingLen  = 0.09f;
	constexpr float kStubDrop = 6.0f;

	void DrawAircraftRefSymbol(const AttitudeRegion& region)
	{
		XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);

		const float s    = region.width;
		const float box  = kBoxHalf * s;
		const float gap  = kGap     * s;
		const float wing = kWingLen * s;
		const float out  = kOutline * s;

		const Rect centerBox = { -box,      box,        box, -box };
		const Rect wingBar =   { gap - box, wing + box, box, -box };
		const Rect wingStub =  { gap - box, gap + box,  box, -kStubDrop * box };

		const Rect shapes[] = {
			centerBox,
			wingBar,  MirrorX(wingBar),
			wingStub, MirrorX(wingStub),
		};

		const float cx = region.centerX;
		const float cy = region.centerY;

		// Border
		for (const Rect& r : shapes) FillRectColor(Offset(Inflate(r, out), cx, cy), Colors::White);

		// Ref Symbol
		for (const Rect& r : shapes) FillRectColor(Offset(r, cx, cy), Colors::Black);

	}
}


