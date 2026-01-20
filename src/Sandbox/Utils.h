#pragma once

#include <GC-simple-render/Geometries/Geometries.h>

namespace Utils
{
	bool Normalize(sr::Vector3& vector);
	float GetDistance(int x1, int y1,int z1, int x2, int y2, int z2);
	float GetAngleDegree(const sr::Vector2& v1, const sr::Vector2& v2);
}