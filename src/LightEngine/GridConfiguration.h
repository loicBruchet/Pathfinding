#pragma once
#include <vector>
#include "DummyEntity.h"

class Scene;

class GridConfiguration
{
	std::vector<std::vector<DummyRectangleEntity*>> mGrid;
	std::vector<std::vector<int>> mGridCells;
	Scene* mScene;
public:
	GridConfiguration(const std::vector<std::vector<int>>& gridCells, Scene* scene);
	void SetGrid();
	void SetGridCells(const std::vector<std::vector<int>>& gridCells) { mGridCells = gridCells; }
	void ModifyGridCells(int x, int y, int value) { mGridCells[x][y] = value; }
	std::vector<std::vector<int>> GetGridCells() const { return mGridCells; }
	DummyRectangleEntity* GetCellAtPosition(int x, int y) const;
	std::vector<std::vector<DummyRectangleEntity*>> GetGrid() const { return mGrid; }
	void newGrid(const char* filepath);
};

