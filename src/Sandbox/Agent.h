#pragma once
#include <iostream>
#include "Entity.h"
#include "DummyEntity.h"

class Agent : public CircleEntity
{
	std::vector<DummyRectangleEntity*> currentPath;
	int a = 0;
	bool arrived = false;
	public:
		void Initpos(float x, float y) {
			mLastPosition.x = x;
			mLastPosition.y = y;
		}
		void SetCurrentPath(const std::vector<DummyRectangleEntity*>& path) {
			currentPath.clear();

			arrived = false;
			a = 0;
			currentPath = path;
			std::cout << "initialPos " << " : " << GetPosition().x << " | " << GetPosition().y << std::endl;
			for (int i = 0; i < currentPath.size(); i++) {
				std::cout << "Path " << i << " : " << currentPath[i]->GetPosition().x << " | " << currentPath[i]->GetPosition().y << std::endl;
			}
		}
		std::vector<DummyRectangleEntity*> getCurrentPath() {
			
			return currentPath;
		}
		void OnCollision(CircleEntity* other) override;
		void OnUpdate() override;
		void Trail(std::vector<std::vector<DummyRectangleEntity*>> mGrid, int sizeX, int sizeY);
};

