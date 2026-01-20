#include "Agent.h"
#include "Scene.h"
#include <iostream>

void Agent::OnCollision(CircleEntity* other)
{
	std::cout << "ouaf" << std::endl;
}

//std::reverse(path.begin(), path.end());
void Agent::OnUpdate()
{
	if (mIsPatroling && a >= currentPath.size())
	{
		a = 0;
		mLastPosition = GetPosition();
		std::reverse(currentPath.begin(), currentPath.end());
		arrived = false;
	}


	
}

void Agent::Trail(std::vector<std::vector<DummyRectangleEntity*>> mGrid, int sizeX, int sizeY)
{
	if (!arrived)
	{
		for (int i = 0; i < currentPath.size(); i++)
		{
			
			sr::Vector3 pos;
			pos.x = currentPath[i]->GetPosition().x + (GetScene()->GetWindowWidth() / sizeX) / 2;
			pos.y = currentPath[i]->GetPosition().y + (GetScene()->GetWindowHeight() / sizeY) / 2;
			pos.z = mGrid[currentPath[i]->x][currentPath[i]->y]->GetTag();
			

			if (i == 0)
			{
				
				pos.x = GetPosition().x;
				pos.y = GetPosition().y;
				mLastPosition = pos;
			}
			else
			{
				pos.x = currentPath[i]->GetPosition().x + (GetScene()->GetWindowWidth() / sizeX) / 2;
				pos.y = currentPath[i]->GetPosition().y + (GetScene()->GetWindowHeight() / sizeY) / 2;
			}
			if (a == i)
			{


				if (GetPosition().x == pos.x && GetPosition().y == pos.y)
				{
					a++;
				}
				int spd = 0;
					if (sizeX > sizeY)
					{
						spd = GetScene()->GetWindowWidth() / sizeX;
					}
					else
					{
						spd = GetScene()->GetWindowHeight() / sizeY;
					}
						
				
				if (mGrid[currentPath[i]->x][currentPath[i]->y]->GetOcuped() == false)
				{
	/*				mGrid[currentPath[i]->x][currentPath[i]->y]->SetOcuped(true);
					if (i != 0)
					{
						mGrid[currentPath[i - 1]->x][currentPath[i - 1]->y]->SetOcuped(false);
					}*/
					GoToPosition(pos.x, pos.y, (50.0f * GetDeltaTime())*spd);
				}
				
			}
			//Debug::DrawCircle(pos.x, pos.y, 5, sf::Color::Blue);
			//std::cout << i << " | " << a << " | " << pos.x << " | " << pos.y << std::endl;
		}
	}

	//for (int i = 1; i < currentPath.size(); i++)
	//{
	//	Debug::DrawCircle(currentPath[i]->GetPosition().x + (GetScene()->GetWindowWidth() / sizeX)/2, (GetScene()->GetWindowHeight() / sizeY) / 2 + currentPath[i]->GetPosition().y, 5, sf::Color::Blue);
	//}
}