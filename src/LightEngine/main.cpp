
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameManager.h"
#include "2DScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main() 
{
    GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1920, 1080, "Eijiro", 60, sf::Color::Black);
	
	pInstance->LaunchScene<Scene2D>();

	return 0;
}