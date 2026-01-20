#include "Utils.h"

#include <cmath>

namespace Utils 
{
    bool Normalize(sr::Vector3& vector)
    {
		float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
			vector.z /= magnitude;
		
			return true;
		}

		return false;
    }

	float GetDistance(int x1, int y1,int z1, int x2, int y2,int z2)
	{
		int x = x2 - x1;
		int y = y2 - y1;
		int z = z2 - z1;
		return std::sqrt(x * x + y * y + z * z);
	}

	float GetAngleDegree(const sr::Vector2& v1, const sr::Vector2& v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot) * 180 / 3.14159265;
	}
}