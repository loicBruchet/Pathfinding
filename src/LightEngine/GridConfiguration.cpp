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
				mGrid[i].push_back(mScene->CreateRectEntity<DummyRectangleEntity>(sf::Vector2f(mScene->GetWindowWidth() / mGridCells[i].size(), mScene->GetWindowHeight() / mGridCells.size()), sf::Color::Yellow));
				mGrid[i][j]->SetTag(0);
			}
			else
			{
				mGrid[i].push_back(mScene->CreateRectEntity<DummyRectangleEntity>(sf::Vector2f(mScene->GetWindowWidth() / mGridCells[i].size(), mScene->GetWindowHeight() / mGridCells.size()), sf::Color::Transparent));
				mGrid[i][j]->SetTag(1);
			}
			mGrid[i][j]->GetShape()->setOutlineColor(sf::Color::Green);
			mGrid[i][j]->x = i;
			mGrid[i][j]->y = j;
			mGrid[i][j]->walkable = (mGridCells[i][j] == 0);
			mGrid[i][j]->GetShape()->setOutlineThickness(1.f);
			mGrid[i][j]->SetPosition(j * (mScene->GetWindowWidth() / mGridCells[i].size()), i * (mScene->GetWindowHeight() / mGridCells.size()), 0.f, 0.f);
		}
	}
}

DummyRectangleEntity* GridConfiguration::GetCellAtPosition(int x, int y) const
{

	int cellX = x / (mScene->GetWindowWidth() / mGridCells[0].size());
	int cellY = y / (mScene->GetWindowHeight() / mGridCells.size());
	if (cellY >= 0 && cellY < mGrid.size() && cellX >= 0 && cellX < mGrid[cellY].size()) {
		return mGrid[cellY][cellX];
	}
	return nullptr;

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
			//size = nblignes;
		
		mGridCells = std::vector<std::vector<int>>(size, std::vector<int>());
		mGrid = std::vector<std::vector<DummyRectangleEntity*>>(size, std::vector<DummyRectangleEntity*>());
		fin = std::ifstream(filepath);
		int compteurLigne = 0;
		while (std::getline(fin, ligne)) {
			//for (int i = 0; i < tmpSize; ++i) {
			for (int i = 0; i < size; ++i) {
  				int lineValue = ligne[i] - '0';
				if (lineValue != 0)
				{
					if (lineValue != 1)
					mGridCells[compteurLigne].push_back(0);
					else
						mGridCells[compteurLigne].push_back(1);
				}
				else 
					mGridCells[compteurLigne].push_back(0);
					
			}
			compteurLigne++;
		}
	}
}
