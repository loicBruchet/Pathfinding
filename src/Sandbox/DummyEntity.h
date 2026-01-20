#pragma once
#include "Entity.h"

class DummyEntity : public CircleEntity
{
public:
	void OnCollision(CircleEntity* other) override;
};

class DummyRectangleEntity : public RectangleEntity
{
public:
	int x;
	int y;
	int z;
	bool walkable;
	int distToStart;
	int distToTarget;
	int totalCost;
	bool visited = false;
		DummyRectangleEntity(int x =-1, int y=-1,int z=-1, bool walkable = false)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			mShape.SetPosition({ (float)x, (float)y, (float)z });
			this->walkable = walkable;
		}
	bool IsOcuped;
	public:

	void SetOcuped(bool ocuped) { IsOcuped = ocuped; }
	bool GetOcuped() const { return IsOcuped; }
};
