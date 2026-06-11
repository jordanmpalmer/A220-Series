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
static const float kSkyR = 0.20f, kSkyG = 0.55f, kSkyB = 0.85f;
static const float kGndR = 0.70f, kGndG = 0.40f, kGndB = 0.08f;

static const float kPi = 3.14159265f;
static const float kHalfPi = 1.5707963f;

// Corner rounding radius in pixels.
//static const float kCornerRadius = 80.0f;


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
    glBegin(GL_QUADS);
		glVertex2f(-size, 0.0f);
		glVertex2f(size, 0.0f);
		glVertex2f(size, size);
		glVertex2f(-size, size);
    glEnd();

    // Ground
    glColor4f(kGndR, kGndG, kGndB, 1.0f);
    glBegin(GL_QUADS);
		glVertex2f(-size, -size);
		glVertex2f(size, -size);
		glVertex2f(size, 0.0f);
		glVertex2f(-size, 0.0f);
    glEnd();

    // Horizon line
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
		glVertex2f(-size, 0.0f);
		glVertex2f(size, 0.0f);
    glEnd();

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

void DrawAircraftRefSymbol(const PFD::AttitudeRegion& region)
{
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);

    float halfBoxSide = 0.005f * region.width;
    float border = 0.0015 * region.width;
    int offset = 0.04f * region.width;
    int symbolWidth = 0.1f * region.width;
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
		glVertex2f(region.centerX - halfBoxSide - border, region.centerY - halfBoxSide - border);
		glVertex2f(region.centerX - halfBoxSide - border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX + halfBoxSide + border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX + halfBoxSide + border, region.centerY - halfBoxSide - border);
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX - halfBoxSide + offset - border, region.centerY - halfBoxSide - border);
		glVertex2f(region.centerX - halfBoxSide + offset - border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX + halfBoxSide + symbolWidth + border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX + halfBoxSide + symbolWidth + border, region.centerY - halfBoxSide - border);
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX - halfBoxSide + offset - border, region.centerY - 3 * halfBoxSide - border);
		glVertex2f(region.centerX - halfBoxSide + offset - border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX + halfBoxSide + offset + border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX + halfBoxSide + offset + border, region.centerY - 3 * halfBoxSide - border);
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX + halfBoxSide - offset + border, region.centerY - halfBoxSide - border);
		glVertex2f(region.centerX + halfBoxSide - offset + border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX - halfBoxSide - symbolWidth - border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX - halfBoxSide - symbolWidth - border, region.centerY - halfBoxSide - border);
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX + halfBoxSide - offset + border, region.centerY - 3 * halfBoxSide - border);
		glVertex2f(region.centerX + halfBoxSide - offset + border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX - halfBoxSide - offset - border, region.centerY + halfBoxSide + border);
		glVertex2f(region.centerX - halfBoxSide - offset - border, region.centerY - 3 * halfBoxSide - border);
	glEnd();



    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
		glVertex2f(region.centerX - halfBoxSide, region.centerY - halfBoxSide); // bottom left
		glVertex2f(region.centerX - halfBoxSide, region.centerY + halfBoxSide); // top left
		glVertex2f(region.centerX + halfBoxSide, region.centerY + halfBoxSide); // top right
		glVertex2f(region.centerX + halfBoxSide, region.centerY - halfBoxSide); // bottom right
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX - halfBoxSide + offset, region.centerY - halfBoxSide);
		glVertex2f(region.centerX - halfBoxSide + offset, region.centerY + halfBoxSide);
		glVertex2f(region.centerX + halfBoxSide + symbolWidth, region.centerY + halfBoxSide);
		glVertex2f(region.centerX + halfBoxSide + symbolWidth, region.centerY - halfBoxSide);
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX - halfBoxSide + offset, region.centerY - 3 * halfBoxSide);
		glVertex2f(region.centerX - halfBoxSide + offset, region.centerY + halfBoxSide);
		glVertex2f(region.centerX + halfBoxSide + offset, region.centerY + halfBoxSide);
		glVertex2f(region.centerX + halfBoxSide + offset, region.centerY - 3 * halfBoxSide);
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX + halfBoxSide - offset, region.centerY - halfBoxSide);
		glVertex2f(region.centerX + halfBoxSide - offset, region.centerY + halfBoxSide);
		glVertex2f(region.centerX - halfBoxSide - symbolWidth, region.centerY + halfBoxSide);
		glVertex2f(region.centerX - halfBoxSide - symbolWidth, region.centerY - halfBoxSide);
	glEnd();

    glBegin(GL_QUADS);
		glVertex2f(region.centerX + halfBoxSide - offset, region.centerY - 3 * halfBoxSide);
		glVertex2f(region.centerX + halfBoxSide - offset, region.centerY + halfBoxSide);
		glVertex2f(region.centerX - halfBoxSide - offset, region.centerY + halfBoxSide);
		glVertex2f(region.centerX - halfBoxSide - offset, region.centerY - 3 * halfBoxSide);
	glEnd();

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

