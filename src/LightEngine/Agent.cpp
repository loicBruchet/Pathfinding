#include "Agent.h"
#include "Scene.h"
#include "Debug.h"
#include <iostream>

void Agent::OnCollision(CircleEntity* other)
{
}

void Agent::OnUpdate()
{
	if (a >= currentPath.size())
	{
		arrived = true;
	}
	if (mIsPatroling && a >= currentPath.size())
	{
		a = 0;
		mLastPosition = GetPosition();
		std::reverse(SavecurrentPath.begin(), SavecurrentPath.end());
		std::reverse(SaveObjectif.begin(), SaveObjectif.end());
		currentPath = SavecurrentPath;
		PathObjectif = SaveObjectif;
		arrived = false;
	}


	
}

void Agent::Trail(std::vector<std::vector<DummyRectangleEntity*>> mGrid, int sizeX, int sizeY)
{
	int spd = 0;
	if (sizeX > sizeY)
	{
		spd = GetScene()->GetWindowWidth() / sizeX;
	}
	else
	{
		spd = GetScene()->GetWindowHeight() / sizeY;
	}
	if (!arrived)
	{
		for (int i = 0; i < currentPath.size(); i++)
		{
			
			sf::Vector2f pos;
			pos.x = currentPath[i]->GetPosition().x + (GetScene()->GetWindowWidth() / sizeX) / 2;
			pos.y = currentPath[i]->GetPosition().y + (GetScene()->GetWindowHeight() / sizeY) / 2;
			

			if (i == 0)
			{
				
				pos.x = GetPosition().x;
				pos.y = GetPosition().y;
			}
			
			if (a == i)
			{
				if (GetPosition().x == pos.x && GetPosition().y == pos.y)
				{
					a = i + 1;
					mGrid[currentPath[i]->x][currentPath[i]->y]->SetIDOcuped(-1);
				}

						
					for (int j = 0; j < PathObjectif.size(); j++)
					{
						if (currentPath[i]->x == PathObjectif[j]->x && currentPath[i]->y == PathObjectif[j]->y)
						{
							PathObjectif.erase(PathObjectif.begin() + j);
						}
					}

				if (mGrid[currentPath[i]->x][currentPath[i]->y]->GetIDOcuped() == -1 || mGrid[currentPath[i]->x][currentPath[i]->y]->GetIDOcuped() == ID)
				{
					mTimer = 0.0f;
					GoToPosition(pos.x, pos.y, (50.0f * GetDeltaTime())*spd);
				}
				else
				{
					mTimer += GetDeltaTime();
					GoToPosition(GetPosition().x, GetPosition().y, (50.0f * GetDeltaTime()) * spd);
				}
				
			}
			
		}
	}
}