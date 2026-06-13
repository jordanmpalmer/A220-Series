#include "Core/Common.h"
#include "Displays/Common/Draw.h"

#include "Displays/Common/Geometry.h"

void FillRect(const Rect& r)
{
    glBegin(GL_QUADS);
		glVertex2f(r.left, r.bottom);
		glVertex2f(r.left, r.top);
		glVertex2f(r.right, r.top);
		glVertex2f(r.right, r.bottom);
	glEnd();
}

void FillRectPoints(float l, float r, float t, float b)
{
    glBegin(GL_QUADS);
		glVertex2f(l, b);
		glVertex2f(l, t);
		glVertex2f(r, t);
		glVertex2f(r, b);
	glEnd();
}
