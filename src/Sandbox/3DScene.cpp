#include "3DScene.h"

#include "DummyEntity.h"
#include "Agent.h"

#include <GCSR.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <queue>

sr::Vector2 Scene3D::Input()
{
	int x = 0;
	int y = 0;
	if (sr::GetKeyDown(sr::Keyboard::UP))
	{
		y -= 1;
	}
	if (sr::GetKeyDown(sr::Keyboard::DOWN))
	{
		y += 1;
	}
	if (sr::GetKeyDown(sr::Keyboard::LEFT))
	{
		x -= 1;
	}
	if (sr::GetKeyDown(sr::Keyboard::RIGHT))
	{
		x += 1;
	}
	return sr::Vector2(y, x);
}

Agent* Scene3D::SelectEntity()
{
	if (sr::GetKeyDown(sr::Keyboard::TAB))
	{
		agentSelected++;
		if (agentSelected >= mAgents.size())
		{
			agentSelected = 0;
		}
	}
	if (mAgents.size() > 0)
	{
		return mAgents[agentSelected];
	}
	else 
	{
		return nullptr;
	}
}

DummyRectangleEntity* Scene3D::SelectRectEntity()
{
	pos1 += Input();
	if (pos1.x < 0)
	{
		pos1.x = 0;
	}
	if (pos1.y < 0)
	{
		pos1.y = 0;
	}
	if (pos1.x >= mGrid->GetGrid().size())
	{
		pos1.x = mGrid->GetGrid().size() - 1;
	}
	if (pos1.y >= mGrid->GetGrid()[0].size())
	{
		pos1.y = mGrid->GetGrid()[0].size() - 1;
	}



	if (mGrid != nullptr)
	{
		return mGrid->GetGrid()[pos1.x][pos1.y];
	}
	return nullptr;
}

void Scene3D::OnInitialize()
{
	std::cout << "Select a grid : ";
	std::cin >> gridNumber;
	for (Agent* agent : mAgents)
	{
		agent = nullptr;
		delete agent;
	}
	mAgents.erase(mAgents.begin(), mAgents.end());
	mAgents.clear();
	setNewGrid(0);
}

void Scene3D::OnEvent()
{
	if (sr::GetKeyDown(sr::Keyboard::ESCAPE))
	{
		GetGameManager()->~GameManager();
	}

	if (sr::GetKeyDown(sr::Keyboard::F1))
	{
		RegisterGrid();
	}

	if (sr::GetKeyDown(sr::Keyboard::F2))
	{
		CreateGrid();
	}

	if (sr::GetKeyDown(sr::Keyboard::ALTGR))
	{
		std::cout << "Grid changer not enabled due to destroy error on GCE ";
	}
	
	agentEvent();

	pEntitySelected = SelectEntity();
	pREntitySelected = SelectRectEntity();
	
	if (pREntitySelected != nullptr)
	{
		for (std::vector<DummyRectangleEntity*> row : mGrid->GetGrid())
		{
			for (DummyRectangleEntity* cell : row)
			{
				if (cell->IsTag(0))
				{
					cell->GetShape()->SetColor(gce::Vector3f32{ 0,0,0 });
				}
				else if (cell->IsTag(1))
				{
					cell->GetShape()->SetColor(gce::Vector3f32{ 0,1,0 });
				}
				else if (cell->IsTag(2))
				{
					cell->GetShape()->SetColor(gce::Vector3f32{ 0,1,1 });
				}
			}
		}
		pREntitySelected->GetShape()->SetColor(gce::Vector3f32{ 1,0,0 });
	}
	if (sr::GetKeyDown(sr::Keyboard::A))
	{
		if (pREntitySelected != nullptr)
		{
			int currentTag = pREntitySelected->GetTag();
			int newTag = (currentTag + 1);
			if (newTag > 1)
			{
				newTag = -1;
			}
			pREntitySelected->SetTag(newTag);
			mGrid->ModifyGridCells(pREntitySelected->x, pREntitySelected->y, newTag);
			mGrid->GetGrid()[pREntitySelected->x][pREntitySelected->y]->walkable = (pREntitySelected == 0);
			mGrid->GetGrid()[pREntitySelected->x][pREntitySelected->y]->SetPosition(pREntitySelected->GetPosition().x, pREntitySelected->GetPosition().y, newTag);
		}
		if (pEntitySelected != nullptr && pEntitySelected->getCurrentPath().size() > 0)
		{
			for (int i = 0; i < mGrid->GetGrid().size(); i++)
			{
				for (int j = 0; j < mGrid->GetGrid()[i].size(); j++)
				{
					for (Agent* agent : mAgents)
					{
						if (mGrid->GetGrid()[i][j]->IsInside(pEntitySelected->GetPosition().x, pEntitySelected->GetPosition().y))
						{
							std::vector<DummyRectangleEntity*> path = Pathfinding(mGrid->GetGrid()[i][j], agent->getCurrentPath().back());
							pEntitySelected->SetCurrentPath(path);
							break;
						}
					}
				}
			}
		}
	}
}

void Scene3D::OnUpdate()
{
	if (pEntitySelected != nullptr)
	{
		sr::Vector3 position = pEntitySelected->GetPosition();
		pEntitySelected->GetShape()->SetColor(gce::Vector3f32{ 0, 0, 1 });
		std::vector<DummyRectangleEntity*> currentPath = pEntitySelected->getCurrentPath();
		for (int i = 0; i < currentPath.size(); i++)
		{
			sr::Vector3 pos;
			if (mCircles.size() < currentPath.size())
			{
				mCircles.push_back(CreateEntity<CircleEntity>(mAgents[0]->GetRadius() / 4, {0,0,1}));
			}
			else
			{
				mCircles[i]->GetShape()->SetPosition({ 0,0,-10 });
			}
			pos.x = currentPath[i]->GetPosition().x + (GetWindowWidth() / mGrid->GetGridCells().size()) / 2;
			pos.y = currentPath[i]->GetPosition().y + (GetWindowHeight() / mGrid->GetGridCells()[0].size()) / 2;
			pos.z = 1;
			mCircles[i]->SetPosition(pos.x, pos.y, pos.z);
		}
	}

	for (int i = 0; i < mAgents.size(); i++)
	{
		mAgents[i]->Trail(mGrid->GetGrid(), mGrid->GetGridCells().size(), mGrid->GetGridCells()[0].size());
		for (int j = 0; j < mGrid->GetGrid().size(); j++)
		{
			for (int k = 0; k < mGrid->GetGrid()[j].size(); k++)
			{
				mGrid->GetGrid()[j][k]->SetOcuped(false);
				sr::Vector3 agentPos = mAgents[i]->GetPosition();
				if (mGrid->GetGrid()[j][k]->IsInside(agentPos.x, agentPos.y) == true)
				{
					mGrid->GetGrid()[j][k]->SetOcuped(true);
				}
			}
		}
	}
}

void Scene3D::agentEvent()
{
	if (sr::GetKeyDown(sr::Keyboard::P))
	{
		if (pEntitySelected != nullptr)
		{
			pEntitySelected->SetPatroling(!pEntitySelected->IsPatroling());
		}

	}
	else if (sr::GetKeyDown(sr::Keyboard::J))
	{
		for (int i = 0; i < mAgents.size(); i++)
		{
			if (mAgents[i]->IsInside(sr::GetMousePosition().x, sr::GetMousePosition().y))
			{
				mAgents[i]->Destroy();
				mAgents.erase(mAgents.begin() + i);
				return;
			}
		}
		for (int i = 0; i < mGrid->GetGrid().size(); i++)
		{
			for (int j = 0; j < mGrid->GetGrid()[i].size(); j++)
			{
				if ((mGrid->GetGrid()[i][j]->IsTag(1) || mGrid->GetGrid()[i][j]->IsTag(2)) && mGrid->GetGrid()[i][j] == pREntitySelected)
				{
					sr::Vector3 pos = pREntitySelected->GetPosition();
					Agent* pEntity = CreateEntity<Agent>(1, {1,0,0});
					pEntity->SetPosition(pos.x, pos.y , pREntitySelected->GetTag()+1);
					pEntity->SetRigidBody(false);
					pEntity->SetPatroling(false);
					pREntitySelected->SetOcuped(true);
					mGrid->GetGrid()[pREntitySelected->x][pREntitySelected->y]->SetOcuped(true);
					mAgents.push_back(pEntity);
					break;
				}
			}
		}
	}
	else if (sr::GetButtonDown(sr::Mouse::LEFT) && sr::GetKeyDown(sr::Keyboard::Z))
	{
		if (pEntitySelected != nullptr && pREntitySelected !=nullptr)
		{
			if (pREntitySelected->IsTag(0) || pREntitySelected->IsTag(2))
				return;

			for (int i = 0; i < mGrid->GetGrid().size(); i++)
			{
				for (int j = 0; j < mGrid->GetGrid()[i].size(); j++)
				{
					if (mGrid->GetGrid()[i][j]->IsInside(pEntitySelected->GetPosition().x, pEntitySelected->GetPosition().y))
					{
						std::vector<DummyRectangleEntity*> path = Pathfinding(mGrid->GetGrid()[i][j], pREntitySelected);
						
						pEntitySelected->SetCurrentPath(path);
						break;
					}
				}
			}
		}
	}
}

void Scene3D::setNewGrid(int gridNum)
{
	gridNumber += gridNum;
	std::string fullPath = "..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
	const char* filepath = fullPath.c_str();
	std::ifstream fin = std::ifstream(filepath);
	int nblignes = 0;
	if (!fin.is_open())
	{
		gridNumber = 0;
		fullPath = "..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
		filepath = fullPath.c_str();
		fin = std::ifstream(filepath);
	}
	if (fin.is_open()) {
		std::string ligne;
		int nbcolonnes = 0;
		while (std::getline(fin, ligne)) {
			nbcolonnes = ligne.size();
			nblignes++;
		}
		
		if (mGrid != nullptr)
		for (int i = 0; i < mGrid->GetGrid().size(); i++) {
			for (int j = 0; j < mGrid->GetGrid()[0].size(); j++) {
				if (mGrid->GetGrid()[i].size() != 0)
				{
					mGrid->GetGrid()[i][j]->Destroy();
				}

			}
		}
		mGrid = new GridConfiguration(std::vector<std::vector<int>>(nblignes, std::vector<int>()), this);
	}
	fin.close();
	mGrid->newGrid(filepath);
	mGrid->SetGrid();
	pEntitySelected = nullptr;
	if (GameManager::Get()->GetCamera() == nullptr)
	{
		GameManager::Get()->SetCamera();
	}
	GameManager::Get()->SetPos(mGrid->GetGrid()[0].size() / 2.f, mGrid->GetGrid().size() / 2.f, 50.f);
}


void Scene3D::RegisterGrid()
{
	std::string fullPath = "..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
	const char* filepath = fullPath.c_str();
	std::ofstream fout = std::ofstream(filepath);
	int maxlines = 0;
	int maxcols = mGrid->GetGridCells()[0].size();

	for (int i = 0; i < mGrid->GetGridCells().size(); i++) {
		if (mGrid->GetGridCells()[i].size() != 0)
		{
			maxlines++;
		}
	}

	if (fout.is_open()) {
		for (int i = 0; i < maxlines; i++) {
			for (int j = 0; j < mGrid->GetGridCells()[0].size(); j++) {
				fout << mGrid->GetGridCells()[i][j];
			}
			if (i < maxlines)
			{
				fout << "\n";
			}
		}
	}
}
void Scene3D::CreateGrid()
{
	int largeur = 0.0f;
	int hauteur = 0.0f;
	std::cout << "Veuillez entrer la largeur (nombre décimal) : ";
	while (!(std::cin >> largeur) || largeur < 0.0f || largeur > 75) {
		std::cout << "Saisie invalide. Veuillez entrer une largeur positive entre 1 et 75 (ex: 25 ou 50) : ";
		std::cin.clear();
	}
	std::cout << "Veuillez entrer la hauteur (nombre décimal) : ";
	while (!(std::cin >> hauteur) || hauteur < 0.0f || hauteur > 75) {
		std::cout << "Saisie invalide. Veuillez entrer une hauteur positive entre 1 et 75 (ex: 25 ou 50) : ";
		std::cin.clear();
	}
	gridNumber += 1;
	std::string fullPath = "..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
	const char* filepath = fullPath.c_str();
	std::ifstream fin = std::ifstream(filepath);
	int nblignes = 0;
	while (fin.is_open())
	{
		gridNumber += 1;
		fullPath = "..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
		filepath = fullPath.c_str();
		fin = std::ifstream(filepath);
	}
	fin.close();

	std::ofstream fout = std::ofstream(filepath);
	int maxlines = hauteur;
	int maxcols = largeur;

	if (fout.is_open()) {
		for (int i = 0; i < maxlines; i++) {
			for (int j = 0; j < maxcols; j++) {
				fout << "1";
			}
			if (i < maxlines)
			{
				fout << "\n";
			}
		}
	}
}

std::vector<DummyRectangleEntity*> Scene3D::Pathfinding(DummyRectangleEntity* start, DummyRectangleEntity* end)
{
	//int dirX[4] = { -1, 1, 0, 0 };
	//int dirY[4] = { 0, 0, -1, 1 };
	//std::vector<std::vector<DummyRectangleEntity*>> grid(mGrid->GetGrid().size(), std::vector<DummyRectangleEntity*>(mGrid->GetGrid().size()));
	//std::vector<std::vector<DummyRectangleEntity*>> parents(mGrid->GetGrid().size(), std::vector<DummyRectangleEntity*>(mGrid->GetGrid().size(), nullptr));
	//for (int i = 0; i < mGrid->GetGrid().size(); i++) {
	//	for (int j = 0; j < mGrid->GetGrid().size(); j++) {
	//		grid[i][j] = new DummyRectangleEntity(i, j, mGrid->GetGridCells()[i][j] != 1);
	//		grid[i][j]->distToStart = 99999;
	//		grid[i][j]->totalCost = 99999;
	//		grid[i][j]->SetOcuped(mGrid->GetGrid()[i][j]->GetOcuped());
	//		grid[i][j]->visited = false;
	//		parents[i][j] = new DummyRectangleEntity(-1, -1);
	//	}
	//}
	//start->x = grid[start->x][start->y]->x;
	//start->y = grid[start->x][start->y]->y;
	//end->x = grid[end->x][end->y]->x;
	//end->y = grid[end->x][end->y]->y;
	//
	//if (!start->walkable || !end->walkable) {
	//	return {};
	//}
	//if (start->x == end->x && start->y == end->y) {
	//	return {start};
	//}
	//std::priority_queue<DummyRectangleEntity*, std::vector<DummyRectangleEntity*>, CompareTiles> openList;
	//grid[start->x][start->y]->distToStart = 0;
	//grid[start->x][start->y]->distToTarget = std::abs(start->x - end->x) + std::abs(start->y - end->y);
	//grid[start->x][start->y]->totalCost = grid[start->x][start->y]->distToStart + grid[start->x][start->y]->distToTarget;
	//openList.push(grid[start->x][start->y]);
	//std::vector<DummyRectangleEntity*> visitedTilesForDisplay;
	//while (!openList.empty()) {
	//	DummyRectangleEntity* current = openList.top();
	//
	//	current->SetPosition(current->y * (GetWindowWidth() / mGrid->GetGridCells().size()), current->x * (GetWindowHeight() / mGrid->GetGridCells()[0].size()), 0.f, 0.f);
	//	openList.pop();
	//	if (grid[current->x][current->y]->visited) {
	//	continue;
	//	}
	//	grid[current->x][current->y]->visited = true;
	//	visitedTilesForDisplay.push_back(current);
	//	if (current->x == end->x && current->y == end->y) {
	//		std::vector<DummyRectangleEntity*> path;
	//		int distanceToStart = 0;
	//		DummyRectangleEntity* step = current;
	//		while (step->x != start->x || step->y != start->y) {
	//			distanceToStart++;
	//			path.push_back(step);
	//			if (parents[step->x][step->y]->x == -1)
	//				return {};
	//			step = parents[step->x][step->y];
	//		}
	//		path.push_back(step);
	//		std::reverse(path.begin(), path.end());
	//		return path;
	//	}
	//	for (int i = 0; i < 4; ++i) {
	//		int nextX = current->x + dirX[i];
	//		int nextY = current->y + dirY[i];
	//		if (nextX >= 0 && nextX < mGrid->GetGrid().size() && nextY >= 0 && nextY < mGrid->GetGrid().size()) {
	//			DummyRectangleEntity* next = grid[nextX][nextY];
	//			if (!next->walkable || next->visited || next->GetOcuped()) {
	//				if (next == start)
	//				{
	//				}
	//				else continue;
	//			}
	//			int newDistToStart = current->distToStart + 1;
	//			if (newDistToStart < next->distToStart) {
	//				next->distToStart = newDistToStart;
	//				next->distToTarget = std::abs(nextX - end->x) + std::abs(nextY - end->y);
	//				next->totalCost = next->distToStart + next->distToTarget;
	//				parents[next->x][next->y] = current;
	//				openList.push(next);
	//			}
	//		}
	//	}
	//}
	//return {};

	int dirX[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int dirY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };


	std::vector<std::vector<DummyRectangleEntity*>> grid(mGrid->GetGrid().size(), std::vector<DummyRectangleEntity*>(mGrid->GetGrid().size()));
	std::vector<std::vector<DummyRectangleEntity*>> parents(mGrid->GetGrid().size(), std::vector<DummyRectangleEntity*>(mGrid->GetGrid().size(), nullptr));
	for (int i = 0; i < mGrid->GetGrid().size(); i++) {
		for (int j = 0; j < mGrid->GetGrid().size(); j++) {
			grid[i][j] = new DummyRectangleEntity(i, j,mGrid->GetGrid()[i][j]->GetTag(), mGrid->GetGridCells()[i][j] != 1);
			grid[i][j]->distToStart = 99999;
			grid[i][j]->totalCost = 99999;
			grid[i][j]->SetOcuped(mGrid->GetGrid()[i][j]->GetOcuped());
			grid[i][j]->visited = false;
			parents[i][j] = new DummyRectangleEntity(-1, -1, -1);
		}
	}
	start->x = grid[start->x][start->y]->x;
	start->y = grid[start->x][start->y]->y;

	end->x = grid[end->x][end->y]->x;
	end->y = grid[end->x][end->y]->y;

	if (!start->walkable || !end->walkable) {
		return {};
	}
	if (start->x == end->x && start->y == end->y) {
		return { start };
	}

	std::priority_queue<DummyRectangleEntity*, std::vector<DummyRectangleEntity*>, CompareTiles> openList;
	grid[start->x][start->y]->distToStart = 0;
	grid[start->x][start->y]->distToTarget = std::abs(start->x - end->x) + std::abs(start->y - end->y);
	grid[start->x][start->y]->totalCost = grid[start->x][start->y]->distToStart + grid[start->x][start->y]->distToTarget;
	openList.push(grid[start->x][start->y]);
	std::vector<DummyRectangleEntity*> visitedTilesForDisplay;
	while (!openList.empty()) {
		DummyRectangleEntity* current = openList.top();

		current->SetPosition(current->y * (GetWindowWidth() / mGrid->GetGridCells().size()), current->x * (GetWindowHeight() / mGrid->GetGridCells()[0].size()), current->z);
		openList.pop();
		if (grid[current->x][current->y]->visited) {
			continue;
		}
		grid[current->x][current->y]->visited = true;
		visitedTilesForDisplay.push_back(current);
		if (current->x == end->x && current->y == end->y) {
			std::vector<DummyRectangleEntity*> path;
			int distanceToStart = 0;
			DummyRectangleEntity* step = current;
			while (step->x != start->x || step->y != start->y) {
				distanceToStart++;
				path.push_back(step);
				if (parents[step->x][step->y]->x == -1)
					return {};
				step = parents[step->x][step->y];
			}
			path.push_back(step);
			std::reverse(path.begin(), path.end());
			return path;
		}
		for (int i = 0; i < 8; ++i) {
			int nextX = current->x + dirX[i];
			int nextY = current->y + dirY[i];
			if (nextX >= 0 && nextX < mGrid->GetGrid().size() && nextY >= 0 && nextY < mGrid->GetGrid().size()) {
				DummyRectangleEntity* next = grid[nextX][nextY];
				bool diagonal = (dirX[i] != 0 && dirY[i] != 0);


				DummyRectangleEntity* side1 = grid[current->x][current->y + dirY[i]]; // vertical
				DummyRectangleEntity* side2 = grid[current->x + dirX[i]][current->y]; // horizontal

				// Interdiction si une des deux cases orthogonales n'est pas walkable
				if (!side1->walkable || !side2->walkable) continue;

				// Interdiction si une des deux cases orthogonales est occupée
				if (side1->GetOcuped() || side2->GetOcuped()) continue;

				// Interdiction si la case diagonale elle-même est occupée
				if (next->GetOcuped()) continue;
				if (!next->walkable || next->visited || next->GetOcuped()) {
					if (next == start)
					{

					}
					else continue;
				}
				
				int newDistToStart = current->distToStart + (diagonal ? 14 : 10);
				if (newDistToStart < next->distToStart) {
					next->distToStart = newDistToStart;
					int dx = std::abs(nextX - end->x);
					int dy = std::abs(nextY - end->y);
					if (dx < dy)
					{
						dy = dx;
					}
					next->distToTarget = 10 * (dx + dy) + (14 - 2 * 10) * dy;
					next->totalCost = next->distToStart + next->distToTarget;
					parents[next->x][next->y] = current;
					openList.push(next);
				}
			}
		}
	}
	return {};
}