#include "GridConfiguration.h"

#include "Scene.h"
#include <fstream>

GridConfiguration::GridConfiguration(const std::vector<std::vector<int>>& gridCells, Scene* scene)
{
	mScene = scene;
	mGrid = std::vector<std::vector<DummyRectangleEntity*>>(gridCells.size(), std::vector<DummyRectangleEntity*>());
	for (int i = 0; i < gridCells.size(); i++) {
		for (int j = 0; j < gridCells[i].size(); j++) {
				if (gridCells[i][j] == 0) {
					mGrid[i].push_back(new DummyRectangleEntity(i, j, true));
				}
				else {
					mGrid[i].push_back(new DummyRectangleEntity(i,j, false));
				}
		}
	}
}

void GridConfiguration::SetGrid()
{
	
	for (int i = 0; i < mGridCells.size(); ++i)
	{
		for (int j = 0; j < mGridCells[i].size(); ++j)
		{
			if (mGridCells[i][j] == 0)
			{
				mGrid[i].push_back(mScene->CreateRectEntity<DummyRectangleEntity>(sr::Vector3(1,1,1), {0,1,1}));
				mGrid[i][j]->SetTag(0);
			}
			else if (mGridCells[i][j] == 1)
			{
				mGrid[i].push_back(mScene->CreateRectEntity<DummyRectangleEntity>(sr::Vector3(1,1,1), {0,0,0}));
				mGrid[i][j]->SetTag(1);
			}
			else if (mGridCells[i][j] == 2)
			{
				mGrid[i].push_back(mScene->CreateRectEntity<DummyRectangleEntity>(sr::Vector3(1,1,1), {0,0,1}));
				mGrid[i][j]->SetTag(2);
			}
			mGrid[i][j]->GetShape()->SetColor(gce::Vector3f32{ 0,1,0 });
			mGrid[i][j]->x = i;
			mGrid[i][j]->y = j;
			mGrid[i][j]->walkable = (mGridCells[i][j] == 0);
			int posZ = mGrid[i][j]->GetTag();
			mGrid[i][j]->SetPosition(j, i, posZ);
		}
	}
}

void GridConfiguration::newGrid(const char* filepath)
{
	std::ifstream fin = std::ifstream(filepath);
	int nblignes = 0;
	int size = 0;
	if (fin.is_open()) {
		std::string ligne;
		int tmpSize = 0;
		while (std::getline(fin, ligne)) {
			nblignes++;
			tmpSize = ligne.size();
		}
		
		if (nblignes<tmpSize)
			nblignes = tmpSize;
		size = nblignes;
		
		mGridCells = std::vector<std::vector<int>>(size, std::vector<int>());
		mGrid = std::vector<std::vector<DummyRectangleEntity*>>(size, std::vector<DummyRectangleEntity*>());
		fin = std::ifstream(filepath);
		int compteurLigne = 0;
		while (std::getline(fin, ligne)) {
			for (int i = 0; i < size; ++i) {
				int lineValue = ligne[i] - '0';
				if (lineValue >= 0 && lineValue <= 2)
				{
					mGridCells[compteurLigne].push_back(lineValue);
				}
				else
				{
					mGridCells[compteurLigne].push_back(0);
				}
			}
			compteurLigne++;
		}
	}
	fin.close();
}
