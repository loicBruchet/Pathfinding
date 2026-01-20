#include <iostream>

#include "GameManager.h"
#include "3DScene.h"
#include "GC-simple-render/Window.h"
#include <cstdlib>
#include <crtdbg.h>

int main() 
{
    GameManager* pInstance = GameManager::Get();

	pInstance->CreateNewWindow(1920, 1080, L"Eijiro", 60, sr::Color::Black, sr::Window::Flag::FLAG_ENABLE_FULLSCREEN);
	
	pInstance->LaunchScene<Scene3D>();

	return 0;
}