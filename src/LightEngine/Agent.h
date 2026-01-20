#pragma once
#include <iostream>
#include "Entity.h"
#include "DummyEntity.h"
#include "Node.h"

class Agent : public CircleEntity
{
	std::vector<DummyRectangleEntity*> SavecurrentPath;
	std::vector<DummyRectangleEntity*> currentPath;
	std::vector<Node*> Pathfinding;


	std::vector<DummyRectangleEntity*> SaveObjectif;
	std::vector<DummyRectangleEntity*> PathObjectif;
	int a = 0;
	bool arrived = true;

	int sizeX = 10;
	int sizeY = 10;

	int ID = 0;

	float mTimer = 0.0f;
	float mDelay = 4.0f;
	public:
		void Initpos(float x, float y) 
		{
			mLastPosition.x = x;
			mLastPosition.y = y;
		}

		void SetID(int id) { ID = id; }

		void SetCurrentPath(const std::vector<DummyRectangleEntity*>& path) 
		{
			currentPath.clear();

			a = 0;
			arrived = false;

			
			currentPath = path;
			SavecurrentPath = path;
		}

		void ReSetCurrentPath(const std::vector<DummyRectangleEntity*>& path) 
		{
			currentPath.clear();

			arrived = false;
			a = 0;
			
			currentPath = path;
		}

		void ReSetSavePath(const std::vector<DummyRectangleEntity*>& path) {SavecurrentPath = path; }

		void SetCurrentPath(const std::vector<Node*>& path) {
			Pathfinding.clear();

			arrived = false;

			a = 0;
			Pathfinding = path;
		}
		void SetCurrentPathObjectif(const std::vector<DummyRectangleEntity*>& path) {

			PathObjectif = path;
			SaveObjectif = path;
		}

		//GETTERS
		std::vector<DummyRectangleEntity*> getCurrentPath() { return currentPath; }
		std::vector<DummyRectangleEntity*> GetPathObjectif() { return PathObjectif;}
		std::vector<DummyRectangleEntity*> GetSavePathObjectif() { return SaveObjectif; }
		sf::Vector2f GetLastPosition() const { return mLastPosition; }
		int GetID() const { return ID; }
		bool GetIfpatroling() const { return mIsPatroling; }	
		bool IsArrived() const { return arrived; }
		std::vector<DummyRectangleEntity*> GetSavePath() { return SavecurrentPath; }
		bool IsTooMuchBlocked()
		{
			if (mTimer >= mDelay)
			{
				mTimer = 0.0f;
				return true;
			}
			else return false;
		}
		//Updates
		void OnCollision(CircleEntity* other) override;
		void OnUpdate() override;
		void Trail(std::vector<std::vector<DummyRectangleEntity*>> mGrid, int sizeX, int sizeY);
};

