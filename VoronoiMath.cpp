#include "VoronoiMath.h"
#include <iostream>

int VoronoiMath::GetTriangleOrientation(const Vec2 & a, const Vec2 & b, const Vec2& c)
{
	float det = detTriangle(a, b, c);
	return (det > 0) - (det < 0);
}

bool VoronoiMath::InCircle(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & p)
{
	Vec2 d[3] = { a - p,b - p,c - p };
	float dlen2[3] = {
		d[0].x*d[0].x + d[0].y*d[0].y,
		d[1].x*d[1].x + d[1].y*d[1].y ,
		d[2].x*d[2].x + d[2].y*d[2].y };

	float det
		=d[0].x*(d[1].y*dlen2[2] - dlen2[1] * d[2].y)
		+d[1].x*(d[2].y*dlen2[0] - dlen2[2] * d[0].y)
		+d[2].x*(d[0].y*dlen2[1] - dlen2[0] * d[1].y);
	return (det > 0);
}

float VoronoiMath::TriangleArea(const Vec2 & a, const Vec2 & b, const Vec2 & c)
{
	return abs(detTriangle(a, b, c))*0.5f;
}

float VoronoiMath::detTriangle(const Vec2 & a, const Vec2 & b, const Vec2 & c)
{
	/*return (a.x*b.y - b.x*a.y)
		+ (b.x*c.y - c.x*b.y)
		+ (c.x*a.y - a.x*c.y);*/
	Vec2 p(b.x - a.x, b.y - a.y);
	Vec2 q(c.x - a.x, c.y - a.y);
	return p.x*q.y - q.x*p.y;
}
