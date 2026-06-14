#pragma once

#include "Displays/Common/Geometry.h"   // Rect

void DrawLine(Point p1, Point p2, float width);
void DrawLineColor(Point p1, Point p2, float width, const float c[3], float a = 1.0f);

void FillRect(const Rect& r);
void FillRectPoints(float l, float r, float t, float b);
void FillRectColor(const Rect& r, const float c[3], float a = 1.0f);

void FillPolyColor(const Point* pts, int count, const float c[3], float a = 1.0f);
void StrokePolyColor(const Point* pts, int count, bool closed, float width, const float c[3], float a = 1.0f);

template <std::size_t N>
void FillPolyColor(const std::array<Point, N>& pts, const float c[3], float a = 1.0f) 
{ 
	FillPolyColor(pts.data(), static_cast<int>(N), c, a);
}

template <std::size_t N>
void StrokePolyColor(const std::array<Point, N>& pts, bool closed, float width, const float c[3], float a = 1.0f)
{
	StrokePolyColor(pts.data(), static_cast<int>(N), closed, width, c, a);
}

