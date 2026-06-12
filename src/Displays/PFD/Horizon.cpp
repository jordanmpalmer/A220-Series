#include "Core/Common.h"

#include "Displays/PFD/Horizon.h"

#include "Core/AircraftState.h"
#include "Displays/Common/Geometry.h"

#include "XPLMGraphics.h"
#include "XPLMDisplay.h"

#include <cmath>

// ------------------------------------------------------
// CONSTANTS
// ------------------------------------------------------

// Sky and ground colors (Airbus-style)
constexpr float kSkyR = 0.20f, kSkyG = 0.55f, kSkyB = 0.85f;
constexpr float kGndR = 0.70f, kGndG = 0.40f, kGndB = 0.08f;

constexpr float kPi = 3.14159265f;
constexpr float kHalfPi = 1.5707963f;

// Corner rounding radius in pixels.
//constexpr float kCornerRadius = 80.0f;

// ------------------------------------------------------
// DRAWING HELPERS
// ------------------------------------------------------

struct Rect { float left, right, top, bottom; };

static void FillRectPoints(float l, float r, float t, float b)
{
    glBegin(GL_QUADS);
		glVertex2f(l, b);
		glVertex2f(l, t);
		glVertex2f(r, t);
		glVertex2f(r, b);
	glEnd();
}

static void FillRect(const Rect& r)
{
    glBegin(GL_QUADS);
		glVertex2f(r.left, r.bottom);
		glVertex2f(r.left, r.top);
		glVertex2f(r.right, r.top);
		glVertex2f(r.right, r.bottom);
	glEnd();
}

static Rect Inflate(Rect r, float b)
{
	return { r.left - b, r.right + b, r.top + b, r.bottom - b };
}

static Rect Offset(Rect r, float dx, float dy)
{
    return { r.left + dx, r.right + dx, r.top + dy, r.bottom + dy };
}

static Rect MirrorX(Rect r)
{
    return { -r.right, -r.left, r.top, r.bottom };
}

// ------------------------------------------------------
// HORIZON
// ------------------------------------------------------

void DrawHorizon(const AircraftState& aircraft, const PFD::AttitudeRegion& region)
{
    const float pitch = aircraft.flight.pitchDeg;
    const float roll = aircraft.flight.rollDeg;

    const float size = std::sqrt(static_cast<float>(PFD::kScreenWidth * PFD::kScreenWidth +
        PFD::kScreenHeight * PFD::kScreenHeight));

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
    glTranslatef(0.0f, -pitch * PFD::kPitchPixelsPerDegree, 0.0f);

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

    const Rect longLine = { -0.065f * region.width, 0.065f * region.width, 0.1f * region.height, 0.103f * region.height };
    const Rect mediumLine = { -0.03f * region.width, 0.03f * region.width, 0.1f * region.height, 0.103f * region.height };
    const Rect shortLine = { -0.01f * region.width, 0.01f * region.width, 0.1f * region.height, 0.103f * region.height };

  //  for (int i = -5; i < 5; i++)
  //  {
		//FillRect(Offset(shortLine, 0.0f, 0.05f * region.height + (0.2f * region.height * i)));
		//FillRect(Offset(longLine, 0.0f, 0.1f * region.height + (0.2f * region.height * i)));
		//FillRect(Offset(shortLine, 0.0f, 0.15f * region.height + (0.2f * region.height * i)));
		//FillRect(Offset(mediumLine, 0.0f, 0.2f * region.height + (0.2f * region.height * i)));
  //  }

    float const offset = 0.04f;
    for (int i = -5; i < 5; i++)
    {
		FillRect(Offset(shortLine, 0.0f, offset * region.height + (4.0f * offset * region.height * i)));
		FillRect(Offset(longLine, 0.0f, 2.0f * offset * region.height + (4.0f * offset * region.height * i)));
		FillRect(Offset(shortLine, 0.0f, 3.0f * offset * region.height + (4.0f * offset * region.height * i)));
		FillRect(Offset(mediumLine, 0.0f, 4.0f * offset * region.height + (4.0f * offset * region.height * i)));
    }


    glPopMatrix();

    //// Corner caps must be drawn in screen space (outside the rotated frame),
    //// and only over the attitude region.
    //DrawRoundedCorners(
    //    0.0f + margin,
    //    static_cast<float>(horizonBottom),
    //    static_cast<float>(screenWidth) - margin,
    //    static_cast<float>(screenHeight),
    //    kCornerRadius);

    glDisable(GL_SCISSOR_TEST);
}

// ------------------------------------------------------
// Aircraft Symbol
// ------------------------------------------------------

// Proportions, as fractions of the attitude-region width.
constexpr float kBoxHalf  = 0.005f;
constexpr float kOutline  = 0.0015f;
constexpr float kGap      = 0.040f;
constexpr float kWingLen  = 0.110f;
constexpr float kStubDrop = 3.0f;

void DrawAircraftRefSymbol(const PFD::AttitudeRegion& region)
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

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (const Rect& r : shapes) FillRect(Offset(Inflate(r, out), cx, cy));

    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	for (const Rect& r : shapes) FillRect(Offset(r, cx, cy));

}

// ------------------------------------------------------
// SPEED TAPE
// ------------------------------------------------------

constexpr float kAltTapeWidth   = 0.045f;
constexpr float kAltTapeHeight  = 0.35f;
constexpr float kAltTapeXOffset = 0.25f;

void DrawAltitudeTape(const PFD::AttitudeRegion& region)
{
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

	const float w = region.width;
	const float h = region.height;
    const float wTape = w * kAltTapeWidth;
    const float hTape = h * kAltTapeHeight;
    const float xOffset = w * kAltTapeXOffset;

    const Rect speedTape = { -wTape, wTape, hTape, -hTape };

    const float cx = region.centerX;
    const float cy = region.centerY;

    FillRect(Offset(speedTape, cx + xOffset, cy));

}

constexpr float kSpeedTapeWidth   = 0.03f;
constexpr float kSpeedTapeHeight  = 0.35f;
constexpr float kSpeedTapeXOffset = 0.25f;

void DrawSpeedTape(const PFD::AttitudeRegion& region)
{
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

	const float w = region.width;
	const float h = region.height;
    const float wTape = w * kSpeedTapeWidth;
    const float hTape = h * kSpeedTapeHeight;
    const float xOffset = w * kSpeedTapeXOffset;

    const Rect speedTape = { -wTape, wTape, hTape, -hTape };

    const float cx = region.centerX;
    const float cy = region.centerY;

    FillRect(Offset(speedTape, cx - xOffset, cy));

}


constexpr float kTopBarWidth   = 0.288f;
constexpr float kTopBarHeight  = 0.055f;
constexpr float kTopBarYOffset = 0.24f;
constexpr float kTopBarXOffset = 0.013f;

void DrawTopBar(const PFD::AttitudeRegion& region)
{
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

	const float w = region.width;
	const float h = region.height;
    const float wBar = w * kTopBarWidth;
    const float hBar = h * kTopBarHeight;
    const float yOffset = w * kTopBarYOffset;
    const float xOffset = h * kTopBarXOffset;

    const Rect speedTape = { -wBar, wBar, hBar, -hBar };

    const float cx = region.centerX;
    const float cy = region.centerY;

    FillRect(Offset(speedTape, cx + xOffset, cy + yOffset));

}

// ------------------------------------------------------
// CORNER CAPS
// ------------------------------------------------------

//// Fills the gap between a square corner and a rounded arc, in black.
//// (px, py)   = the actual sharp rectangle corner being hidden
//// (ax, ay)   = the center the arc sweeps around (inset by radius from the corner)
//// startAngle = radians; which 90 degree quadrant this corner occupies
//static void DrawCornerCap(float px, float py, float ax, float ay,
//    float radius, float startAngle)
//{
//    const int   segments = 8;
//    const float step = kHalfPi / segments;
//
//    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
//
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(px, py);                          // fan anchored at the sharp corner
//    for (int i = 0; i <= segments; ++i)          // sweep the arc
//    {
//        float a = startAngle + step * i;
//        glVertex2f(ax + radius * std::cos(a),
//            ay + radius * std::sin(a));
//    }
//    glEnd();
//}

//// Caps all four corners of the given rectangle so it appears rounded.
//static void DrawRoundedCorners(float left, float bottom, float right, float top, float radius)
//{
//    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
//
//    // Top-left      90 -> 180
//    DrawCornerCap(left, top, left + radius, top - radius, radius, kHalfPi);
//    // Top-right      0 -> 90
//    DrawCornerCap(right, top, right - radius, top - radius, radius, 0.0f);
//    // Bottom-left  180 -> 270
//    DrawCornerCap(left, bottom, left + radius, bottom + radius, radius, kPi);
//    // Bottom-right 270 -> 360
//    DrawCornerCap(right, bottom, right - radius, bottom + radius, radius, 3.0f * kHalfPi);
//}

