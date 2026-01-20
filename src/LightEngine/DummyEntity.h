#pragma once
#include "Entity.h"

class Agent;

//class DummyEntity : public CircleEntity
//{
//public:
//	void OnCollision(CircleEntity* other) override;
//};

class DummyRectangleEntity : public RectangleEntity
{
public:
	int x;
	int y;
	bool walkable;
	int distToStart;
	int distToTarget;
	int totalCost;
	bool visited = false;
	bool IsOcuped;
	Agent* ocupedBy;
	int IDagentOnCell;
		DummyRectangleEntity (int x =-1, int y=-1, bool walkable = false)
		{
			this->x = x;
			this->y = y;
			this->walkable = walkable;
			IsOcuped = false;
			ocupedBy = nullptr;
		}
	public:

	/*void SetAOcuped(Agent* ocuped) { ocupedBy = ocuped; }
	void SetOcuped(bool ocuped) { IsOcuped = ocuped; }*/
	void SetIDOcuped(int IDOccuped) { IDagentOnCell = IDOccuped; }
	Agent* GetAgentOcuped() const { return ocupedBy; }
	bool GetOcuped() const { return ocupedBy; }
	int GetIDOcuped() const { return IDagentOnCell; }
};
