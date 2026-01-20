#pragma once

#include "Scene.h"
#include "DummyEntity.h"
#include "GridConfiguration.h"
#include "Node.h"

class Agent;

class Scene2D : public Scene
{
	GridConfiguration* mGrid;
	std::vector<Agent*> mAgents;

	//Entitée sélectionnée
	DummyRectangleEntity* pREntitySelected;
	Agent* pEntitySelected;
	Agent* pEntityModified;

	//Pathfing
	struct CompareTiles {
		bool operator()( DummyRectangleEntity* a, DummyRectangleEntity* b) {

			return a->totalCost > b->totalCost;
		}
	};
	std::vector<DummyRectangleEntity*> path;
	std::vector<DummyRectangleEntity*> Objectifpath;
	std::vector<Node*> previewPath;

	std::vector<std::vector<DummyRectangleEntity*>> mPathGrid;
	std::vector<std::vector<DummyRectangleEntity*>> mParents;

	int getterID = 0;

private:
	void TrySetSelectedEntity(Agent* pEntity, int x, int y);
	void TrySetSelectedEntity(DummyRectangleEntity* pEntity, int x, int y, bool isControl);
	bool isFScreen = true;
	int gridNumber = 0;
	bool DebugMode = false;
public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
	void agentEvent(const sf::Event& event);
	void setNewGrid(int gridNum);
	void CreateGrid();
	void RegisterGrid();
	std::vector<DummyRectangleEntity*> Pathfinding(DummyRectangleEntity* start, DummyRectangleEntity* end);
	std::vector<DummyRectangleEntity*> PathfindingThroughCheckpoints(DummyRectangleEntity* start, const std::vector<DummyRectangleEntity*>& checkpoints, DummyRectangleEntity* finalTarget);
};
