#pragma once

struct Node
{
	int x, y;
	float xPos, yPos;
	bool walkable;
	bool visited;
	int distToStart;
	int distToTarget;
	int totalCost;
	bool IsOcuped;
	Node(int mx =-1, int my = -1, bool mwalkable = false)
	{
		x = mx;
		y = my;
		walkable = mwalkable;
	}
};