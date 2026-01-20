#pragma once

#include "Scene.h"
#include <GCSR.h>
#include "DummyEntity.h"
#include "GridConfiguration.h"

class Agent;

class Scene3D : public Scene
{
	DummyRectangleEntity* pREntitySelected;
	GridConfiguration* mGrid;
	Agent* pEntitySelected;

	std::vector<Agent*> mAgents; 
	std::vector<CircleEntity*> mCircles;
	struct CompareTiles {
		bool operator()( DummyRectangleEntity* a, DummyRectangleEntity* b) {

			return a->totalCost > b->totalCost;
		}
	};
	int agentSelected = 0;
	sr::Vector2 pos1;
private:
	sr::Vector2 Input();
	Agent* SelectEntity();
	DummyRectangleEntity* SelectRectEntity();


	bool isFScreen = true;
	int gridNumber = 0;

public:
	void OnInitialize() override;
	void OnEvent() override;
	void OnUpdate() override;
	void agentEvent();
	void setNewGrid(int gridNum);
	void CreateGrid();
	void RegisterGrid();
	std::vector<DummyRectangleEntity*> Pathfinding(DummyRectangleEntity* start, DummyRectangleEntity* end);

};
