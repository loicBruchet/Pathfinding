#include "2DScene.h"

#include "DummyEntity.h"
#include "Agent.h"

#include "Debug.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <SFML/Graphics.hpp>

void Scene2D::OnInitialize()
{
	for (Agent* agent : mAgents)
	{
		agent->Destroy();
	}
	mAgents.erase(mAgents.begin(), mAgents.end());
	mAgents.clear();
	setNewGrid(1);
	for (std::vector<DummyRectangleEntity*> row : mGrid->GetGrid())
	{
		for (DummyRectangleEntity* cell : row)
		{
			if (cell->IsTag(0))
			{
				cell->GetShape()->setFillColor(sf::Color::Transparent);
			}
			else
			{
				cell->GetShape()->setFillColor(sf::Color::Yellow);
			}
		}
	}
	int size = mGrid->GetGrid().size();

	mPathGrid.resize(size, std::vector<DummyRectangleEntity*>(size));
	mParents.resize(size, std::vector<DummyRectangleEntity*>(size));

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mPathGrid[x][y] = new DummyRectangleEntity(x, y, true);
			mParents[x][y] = nullptr;
		}
	}
}

void Scene2D::OnEvent(const sf::Event& event)
{
	//return if no event
	if (event.type == sf::Event::Count)
		return;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
	{
		isFScreen = !isFScreen;
		GetGameManager()->GetWindow()->create(sf::VideoMode(GetWindowWidth(), GetWindowHeight()), "Shaw !", isFScreen ? sf::Style::Fullscreen : sf::Style::Default);
	}

	// ==================================< QUITTER LE JEU >==================================
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		GetGameManager()->GetWindow()->close();
	}

	// ==================================< ENREGISTRER LES MODIFICATIONS DE LA GRILLE >==================================
	if ( sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		RegisterGrid();
	}

	// ==================================< CREER UNE NOUVELLE GRILLE >==================================
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
		CreateGrid();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
	{
		DebugMode = !DebugMode;
	}

	if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		OnInitialize();
	}
	agentEvent(event);

	// ==================================< SELECTIONNER UNE CELLULE DE LA GRILLE >==================================
	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		for (std::vector<DummyRectangleEntity*> row : mGrid->GetGrid())
		{
			for (DummyRectangleEntity* cell : row)
			{
				TrySetSelectedEntity(cell, event.mouseButton.x, event.mouseButton.y, false);
			}
		}
	}

	// ==================================< SELECTIONNER PLUSIEURS CELLULES >==================================
	else if (event.mouseButton.button == sf::Mouse::Button::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		for (std::vector<DummyRectangleEntity*> row : mGrid->GetGrid())
		{
			for (DummyRectangleEntity* cell : row)
			{
				TrySetSelectedEntity(cell, event.mouseButton.x, event.mouseButton.y, true);
			}
		}
	}
	
	// ==================================< CHANGER LA COULEUR DE LA CELLULE >==================================
	if (pREntitySelected != nullptr  && event.type == event.MouseButtonPressed)
	{
		for (std::vector<DummyRectangleEntity*> row : mGrid->GetGrid())
		{
			for (DummyRectangleEntity* cell : row)
			{
				cell->GetShape()->setOutlineColor(sf::Color::Green);
			}
		}
		pREntitySelected->GetShape()->setOutlineColor(sf::Color::Red);
	}
	for (Agent* agent : mAgents)
	{
		if (pEntitySelected != nullptr && agent->GetID() == pEntitySelected->GetID())
		agent->GetShape()->setFillColor(sf::Color::Cyan);
		else
			agent->GetShape()->setFillColor(sf::Color::Red);
	}
	if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (pREntitySelected != nullptr)
		{
			//enlever le mur
			if (pREntitySelected->IsTag(1))
			{
				pREntitySelected->SetTag(0);
				pREntitySelected->GetShape()->setFillColor(sf::Color::Transparent);
				mGrid->ModifyGridCells(pREntitySelected->x, pREntitySelected->y, 0);
				mGrid->GetGrid()[pREntitySelected->x][pREntitySelected->y]->walkable = true;
			}
			//poser le mur
			else
			{
				pREntitySelected->SetTag(1);
				pREntitySelected->GetShape()->setFillColor(sf::Color::Yellow);
				mGrid->ModifyGridCells(pREntitySelected->x, pREntitySelected->y, 1);
				mGrid->GetGrid()[pREntitySelected->x][pREntitySelected->y]->walkable = false;
			}
		}
		//mettre à jour les paths des agents
		for (Agent* agent : mAgents)
		{
			for (int i = 0; i < agent->getCurrentPath().size(); i++)
			{
				for (int i = 0; i < mGrid->GetGrid().size(); i++)
				{
					for (int j = 0; j < mGrid->GetGrid()[i].size(); j++)
					{
						if (mGrid->GetGrid()[i][j]->IsInside(agent->GetPosition().x, agent->GetPosition().y))
						{
							pEntityModified = agent;
							std::vector<DummyRectangleEntity*> newpath = PathfindingThroughCheckpoints(mGrid->GetGrid()[i][j], agent->GetPathObjectif(), agent->getCurrentPath().back());
								agent->ReSetCurrentPath(newpath);
								goto exit_for1;
									
						}
					}
				}
						
			}
		exit_for1:;
			std::vector<DummyRectangleEntity*> pathsave = agent->GetSavePath();
			for (int i = 0; i < pathsave.size(); i++)
			{
				for (int i = 0; i < mGrid->GetGrid().size(); i++)
				{
					for (int j = 0; j < mGrid->GetGrid()[i].size(); j++)
					{
						if (mGrid->GetGrid()[i][j]->IsInside(agent->GetPosition().x, agent->GetPosition().y))
						{
							pEntityModified = agent;
							std::vector<DummyRectangleEntity*> newpath = PathfindingThroughCheckpoints(pathsave[0], agent->GetSavePathObjectif(), pathsave.back());
							agent->ReSetSavePath(newpath);
							goto exit_for2;

						}

					}
				}
			}
		exit_for2:;

		}
	}
}

void Scene2D::TrySetSelectedEntity(Agent* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;
	pEntitySelected = pEntity;
	pEntityModified = pEntity;
}

void Scene2D::TrySetSelectedEntity(DummyRectangleEntity* pEntity, int x, int y, bool isControl)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pREntitySelected = pEntity;

	if (pEntitySelected != nullptr)
	{
		for (int i = 0; i < mGrid->GetGrid().size(); i++)
		{
			for (int j = 0; j < mGrid->GetGrid()[i].size(); j++)
			{
				if (mGrid->GetGrid()[i][j]->IsInside(pEntitySelected->GetPosition().x, pEntitySelected->GetPosition().y))
				{
					//pusher l'objectif dans la liste des objectifs
					if (isControl == true)
					{
						Objectifpath.push_back(pREntitySelected);
					}

					//laisser qu'un seul objectif
					else
					{
						Objectifpath.clear();
						Objectifpath.push_back(pREntitySelected);
					}
					path = PathfindingThroughCheckpoints(mGrid->GetGrid()[i][j], Objectifpath, pREntitySelected);
					return;
				}
			}
		}
	}
}

void Scene2D::OnUpdate()
{
	int sizeX = mGrid->GetGrid().size();
	int sizeY = mGrid->GetGrid()[0].size();
	if (pEntitySelected != nullptr)
	{
		float RadiusBall = mAgents[0]->GetRadius() / 4;
		// ==================================< AFFICHER LA PATH AVANT DE LANCER >==================================
		if (path.size() >0)
		{
			sf::Vector2f position = pEntitySelected->GetPosition();
			if (DebugMode)
			{
				Debug::DrawCircle(position.x, position.y, mAgents[0]->GetRadius() / 4, sf::Color::Blue);
			}
			for (int i = 0; i < path.size(); i++)
			{
				sf::Vector2f pos;

				pos.x = path[i]->GetPosition().x + (GetWindowWidth() / mGrid->GetGridCells().size()) / 2;
				pos.y = path[i]->GetPosition().y + (GetWindowHeight() / mGrid->GetGridCells()[0].size()) / 2;
				if (DebugMode)
				{
					Debug::DrawCircle(pos.x, pos.y, mAgents[0]->GetRadius() / 4, sf::Color::Blue);
				}
			}
		}
		// ==================================< AFFICHER LE PATH DE L'AGENT SELECTIONNE >==================================
		std::vector<DummyRectangleEntity*> savePath = pEntitySelected->GetSavePath();
		std::vector<DummyRectangleEntity*> saveObj = pEntitySelected->GetSavePathObjectif();
		for (int i = 0; i < savePath.size(); i++)
		{
			sf::Vector2f pos = savePath[i]->GetPosition();

			pos.x += (GetWindowWidth() / sizeX) / 2;
			pos.y += (GetWindowHeight() / sizeY) / 2;

			for (int j = 0; j < saveObj.size(); j++)
			{
				if (savePath[i]->x == saveObj[j]->x && savePath[i]->y == saveObj[j]->y)
				{
					Debug::DrawCircle(pos.x, pos.y, RadiusBall, sf::Color::Yellow);
				}
				else if (DebugMode)
					Debug::DrawCircle(pos.x, pos.y, mAgents[0]->GetRadius() / 4, sf::Color::Green);
			}
		}
	}

	// ==================================< METTRE A JOUR LES OCCUPATIONS DES CELLULES >==================================
		
		//Initaliser
		for (int j = 0; j < mGrid->GetGrid().size(); j++)
		{
			for (int k = 0; k < mGrid->GetGrid()[j].size(); k++)
			{
				mGrid->GetGrid()[j][k]->SetIDOcuped(-1);
			}
		}
		//Setter les occupations
		for (Agent* agent : mAgents)
		{
			sf::Vector2f pos = agent->GetPosition();
			DummyRectangleEntity* cell = mGrid->GetCellAtPosition(pos.x, pos.y);

			if (cell != nullptr)
			{
				if (cell->GetIDOcuped() == -1)
				{
					cell->SetIDOcuped(agent->GetID());
				}
				
			}
		}
	
	// ==================================< EMPECHER LES AGENTS DE SE BLOQUER >==================================
	for (int i = 0; i < mAgents.size(); i++)
	{
		if (mAgents[i]->IsTooMuchBlocked() == true)
		{
			pEntityModified = mAgents[i];
			sf::Vector2f pos = mAgents[i]->GetPosition();
			DummyRectangleEntity* cell = mGrid->GetCellAtPosition(pos.x, pos.y);
			std::vector<DummyRectangleEntity*> newpath = PathfindingThroughCheckpoints(cell, mAgents[i]->GetPathObjectif(), mAgents[i]->getCurrentPath().back());
			mAgents[i]->ReSetCurrentPath(newpath);
		}
		std::vector<std::vector<int>> gridCells = mGrid->GetGridCells();
		mAgents[i]->Trail(mGrid->GetGrid(), gridCells.size(), gridCells[0].size());
	}
}

void Scene2D::agentEvent(const sf::Event& event)
{
	// ==================================< ACTIVER LE MODE PATROUILLE >==================================
	if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		if (pEntitySelected != nullptr)
		{
			pEntitySelected->SetPatroling(!pEntitySelected->IsPatroling());
		}

	}

	// ==================================< CREER/DETUIRE UN AGENT >==================================
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && event.mouseButton.button == sf::Mouse::Button::Left && event.type == sf::Event::MouseButtonPressed)
	{
		if (pREntitySelected != nullptr)
		{
			sf::Vector2f pos = pREntitySelected->GetPosition();
			//détruire
			for (int i = 0; i < mAgents.size(); i++)
			{
				if (mAgents[i]->IsInside(pos.x, pos.y))
				{
					mAgents[i]->Destroy();
					mAgents.erase(mAgents.begin() + i);
					pEntitySelected = nullptr;
					path.clear();
					return;
				}
			}
			//créer
			{
				Agent* pEntity = CreateEntity<Agent>(GetWindowHeight() / mGrid->GetGrid().size() / 4, sf::Color::Red);
				pEntity->SetPosition(pos.x, pos.y);
				pEntity->SetID(getterID);
				getterID++;
				pEntity->Initpos(pos.x, pos.y);
				pEntity->SetRigidBody(false);
				pEntity->SetPatroling(false);
				mGrid->GetGrid()[pREntitySelected->x][pREntitySelected->y]->SetIDOcuped(pEntity->GetID());
				mAgents.push_back(pEntity);
			}
		}
	}
	
	// ==================================< SELECTIONNER UN AGENT >==================================
	else if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		for (Agent* pAgent : mAgents)
		{
			TrySetSelectedEntity(pAgent, event.mouseButton.x, event.mouseButton.y);
		}
	}

	// ==================================< ACTIVER LE PATH >==================================
	else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		if (pEntitySelected != nullptr && pREntitySelected !=nullptr)
		{
			if (pREntitySelected->IsTag(1))
				return;

			pEntitySelected->SetCurrentPath(path);
			pEntitySelected->SetCurrentPathObjectif(Objectifpath);
			Objectifpath.clear();
		}
	}
}

void Scene2D::setNewGrid(int gridNum)
{
	if (mGrid != nullptr)
	{
		for (std::vector<DummyRectangleEntity*> row : mGrid->GetGrid())
		{
			for (DummyRectangleEntity* cell : row)
			{
				cell->Destroy();
			}
		}
		delete mGrid;
		mGrid = nullptr;
	}
	gridNumber += gridNum;
	std::string fullPath = "..\\..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
	const char* filepath = fullPath.c_str();
	std::ifstream fin = std::ifstream(filepath);
	int nblignes = 0;
	if (!fin.is_open())
	{
		gridNumber = 0;
		fullPath = "..\\..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
		filepath = fullPath.c_str();
		fin = std::ifstream(filepath);
	}
	if (fin.is_open()) {
		std::string ligne;
		while (std::getline(fin, ligne)) {
			nblignes++;
		}
		if (mGrid == nullptr)
		mGrid = new GridConfiguration(std::vector<std::vector<int>>(nblignes, std::vector<int>()), this);
	}
	mGrid->newGrid(filepath);
	mGrid->SetGrid();
	pEntitySelected = nullptr;
	pEntityModified = nullptr;

	int size = mGrid->GetGrid().size();
	int sizeY = mGrid->GetGrid()[0].size();

	mPathGrid.resize(size, std::vector<DummyRectangleEntity*>(size));
	mParents.resize(size, std::vector<DummyRectangleEntity*>(size));

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < sizeY; j++) {
			mPathGrid[i].resize(sizeY);
			mParents[i].resize(sizeY);
		}
	}

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mPathGrid[x][y] = new DummyRectangleEntity(x, y, true);
			mParents[x][y] = nullptr;
		}
	}

}

void Scene2D::RegisterGrid()
{
	std::string fullPath = "..\\..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
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

void Scene2D::CreateGrid()
{
	int largeur = 0.0f;
	int hauteur = 0.0f;
	std::cout << "Veuillez entrer la taille de la grid (int) : ";
	while (!(std::cin >> largeur) || largeur < 0.0f || largeur > 75) {
		std::cout << "Saisie invalide. Veuillez entrer une largeur positive entre 1 et 75 (ex: 25 ou 50) : ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	hauteur = largeur;
	gridNumber += 1;
	std::string fullPath = "..\\..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
	const char* filepath = fullPath.c_str();
	std::ifstream fin = std::ifstream(filepath);
	int nblignes = 0;
	while (fin.is_open())
	{
		gridNumber += 1;
		fullPath = "..\\..\\..\\res\\grid" + std::to_string(gridNumber) + ".txt";
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
				fout << "0";
			}
			if (i < maxlines)
			{
				fout << "\n";
			}
		}
	}
}


std::vector<DummyRectangleEntity*> Scene2D::Pathfinding(DummyRectangleEntity* start, DummyRectangleEntity* end)
{
	int size = mGrid->GetGrid().size();
	int dirX[8] = { -1, 1, 0, 0, -1, -1,  1,  1 };
	int dirY[8] = { 0, 0, -1, 1, -1,  1, -1,  1 };

	// ==================================< INITIALISER LA GRILLE POUR LE PATH >==================================
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			mPathGrid[i][j]->distToStart = 99999;
			mPathGrid[i][j]->totalCost = 99999;
			mPathGrid[i][j]->walkable = mGrid->GetGrid()[i][j]->walkable;
			mPathGrid[i][j]->SetIDOcuped(mGrid->GetGrid()[i][j]->GetIDOcuped());
			mPathGrid[i][j]->visited = false;

			mParents[i][j] = nullptr;
		}
	}

	start->x = mPathGrid[start->x][start->y]->x;
	start->y = mPathGrid[start->x][start->y]->y;
	end->x = mPathGrid[end->x][end->y]->x;
	end->y = mPathGrid[end->x][end->y]->y;

	if (!start->walkable || !end->walkable) {
		return {};
	}

	if (start->x == end->x && start->y == end->y) {
		return { start };
	}

	std::priority_queue<DummyRectangleEntity*, std::vector<DummyRectangleEntity*>, CompareTiles> openList;

	mPathGrid[start->x][start->y]->distToStart = 0;
	mPathGrid[start->x][start->y]->distToTarget = std::abs(start->x - end->x) + std::abs(start->y - end->y);
	mPathGrid[start->x][start->y]->totalCost = mPathGrid[start->x][start->y]->distToStart + mPathGrid[start->x][start->y]->distToTarget;

	openList.push(mPathGrid[start->x][start->y]);

	std::vector<DummyRectangleEntity*> visitedTilesForDisplay;

	while (!openList.empty()) {
		DummyRectangleEntity* current = openList.top();
		current->SetPosition(current->y * (GetWindowWidth() / mGrid->GetGridCells().size()), current->x * (GetWindowHeight() / mGrid->GetGridCells()[0].size()), 0.f, 0.f);

		openList.pop();

		if (mPathGrid[current->x][current->y]->visited)
			continue;

		mPathGrid[current->x][current->y]->visited = true;
		visitedTilesForDisplay.push_back(current);


		//Path terminée
		if (current->x == end->x && current->y == end->y) {
			std::vector<DummyRectangleEntity*> path;
			DummyRectangleEntity* step = current;

			while (step->x != start->x || step->y != start->y) {
				path.push_back(step);

				if (mParents[step->x][step->y]->x == -1)
					return {};

				step = mParents[step->x][step->y];
			}

			path.push_back(step);
			std::reverse(path.begin(), path.end());
			return path;
		}

		for (int i = 0; i < 8; ++i) {
			int nextX = current->x + dirX[i];
			int nextY = current->y + dirY[i];

			if (nextX < 0 || nextX >= size || nextY < 0 || nextY >= size)
				continue;

			DummyRectangleEntity* next = mPathGrid[nextX][nextY];

			if ((!next->walkable || next->visited || (next->GetIDOcuped() != -1 && next->GetIDOcuped() != pEntityModified->GetID())) && next != start)
				continue;

			// REALISER LES DIAGONALES SI POSSIBLE
			bool diagonal = (i >= 4);

			if (diagonal) {
				int adj1X = current->x + dirX[i];
				int adj1Y = current->y;
				int adj2X = current->x;
				int adj2Y = current->y + dirY[i];

				if (!mPathGrid[adj1X][adj1Y]->walkable || (mPathGrid[adj1X][adj1Y]->GetIDOcuped() != pEntitySelected->GetID() && mPathGrid[adj1X][adj1Y]->GetIDOcuped() != -1))
					continue;
				if (!mPathGrid[adj2X][adj2Y]->walkable || (mPathGrid[adj2X][adj2Y]->GetIDOcuped() != pEntitySelected->GetID() && mPathGrid[adj2X][adj2Y]->GetIDOcuped() != -1))
					continue;
			}

			int cost = diagonal ? 2 : 1;
			int newDistToStart = current->distToStart + cost;

			if (newDistToStart < next->distToStart) {
				next->distToStart = newDistToStart;
				next->distToTarget = std::abs(nextX - end->x) + std::abs(nextY - end->y);
				next->totalCost = next->distToStart + next->distToTarget;

				mParents[nextX][nextY] = current;
				openList.push(next);
			}
		}
	}

	return {};
}

std::vector<DummyRectangleEntity*> Scene2D::PathfindingThroughCheckpoints(DummyRectangleEntity* start, const std::vector<DummyRectangleEntity*>& checkpoints, DummyRectangleEntity* finalTarget)
{
	std::vector<DummyRectangleEntity*> fullPath;

	DummyRectangleEntity* currentStart = start;

	//parcourir tous les checkpoints
	for (auto* cp : checkpoints)
	{
		std::vector<DummyRectangleEntity*> segment = Pathfinding(currentStart, cp);

		if (segment.empty())
			return {};

		// Éviter les doublons quand on enchaîne les chemins
		if (!fullPath.empty())
			segment.erase(segment.begin());

		fullPath.insert(fullPath.end(), segment.begin(), segment.end());
		currentStart = cp;
	}

	//Parcourir jusqu'à la fin
	std::vector<DummyRectangleEntity*> lastSegment = Pathfinding(currentStart, finalTarget);

	if (lastSegment.empty())
		return {};

	if (!fullPath.empty())
		lastSegment.erase(lastSegment.begin());

	fullPath.insert(fullPath.end(), lastSegment.begin(), lastSegment.end());

	return fullPath;
}
