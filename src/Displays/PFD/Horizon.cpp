#include "Core/Common.h"

#include "Displays/PFD/Horizon.h"

#include "Core/AircraftState.h"

#include "XPLMGraphics.h"
#include "XPLMDisplay.h"

#include <cmath>

// ------------------------------------------------------
// CONSTANTS
// ------------------------------------------------------

static const float kPitchPixelsPerDegree = 30.0f;

// Sky and ground colors (Airbus-style)
static const float kSkyR = 0.20f, kSkyG = 0.55f, kSkyB = 0.85f;
//static const float kGndR = 0.45f, kGndG = 0.30f, kGndB = 0.15f;
static const float kGndR = 0.70f, kGndG = 0.40f, kGndB = 0.08f;
//static const float kGndR = 0.78f, kGndG = 0.42f, kGndB = 0.08f;
//static const float kGndR = 0.82f, kGndG = 0.40f, kGndB = 0.06f;

// ------------------------------------------------------
// HORIZON
// ------------------------------------------------------

void DrawHorizon(const AircraftState& aircraft, int screenWidth, int screenHeight)
{
    const float pitch = aircraft.flight.pitchDeg;
    const float roll = aircraft.flight.rollDeg;

    // The horizon occupies the top portion of the screen.
    // Everything below this Y is the ND region (stays black).
    const int horizonBottom = static_cast<int>(screenHeight * 0.35f);  // tune
    const int horizonHeight = screenHeight - horizonBottom;

    // Center of the PFD attitude region (not the whole screen)
    const float cx = screenWidth * 0.5f;
    const float cy = horizonBottom + horizonHeight * 0.5f;

    const float size = std::sqrt(static_cast<float>(screenWidth * screenWidth +
        screenHeight * screenHeight));

    // Clip all horizon drawing to the upper region only
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, horizonBottom, screenWidth, horizonHeight);

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(cx, cy, 0.0f);
    glRotatef(roll, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -pitch * kPitchPixelsPerDegree, 0.0f);

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

    glDisable(GL_SCISSOR_TEST);
}
