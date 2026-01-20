#include "GameManager.h"

#include "Entity.h"
#include "Debug.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>

GameManager::GameManager()
{
	mpWindow = nullptr;
	mDeltaTime = 0.0f;
	mpScene = nullptr;
	mWindowWidth = -1;
	mWindowHeight = -1;
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	delete mpScene;

	for (CircleEntity* entity : mEntities)
	{
		delete entity;
	}
	for (RectangleEntity* entity : mREntities)
	{
		delete entity;
	}
}

void GameManager::CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit, sf::Color clearColor)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Fullscreen);
	mpWindow->setFramerateLimit(fpsLimit);

	mWindowWidth = width;
	mWindowHeight = height;

	mClearColor = clearColor;
}

void GameManager::Run()
{
	if (mpWindow == nullptr) 
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateWindow(1280, 720, "Default window");
	}

	//#TODO : Load somewhere else
	bool fontLoaded = mFont.loadFromFile("../../../res/Hack-Regular.ttf");
	_ASSERT(fontLoaded);

	_ASSERT(mpScene != nullptr);

	sf::Clock clock;
	while (mpWindow->isOpen())
	{
		SetDeltaTime(clock.restart().asSeconds());

		HandleInput();

		Update();
		
		Draw();
	}
}

void GameManager::HandleInput()
{
	sf::Event event;
	while (mpWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mpWindow->close();
		}

		mpScene->OnEvent(event);
	}
}

void GameManager::Update()
{
	mpScene->OnUpdate();

    //Update
    for (auto it = mEntities.begin(); it != mEntities.end(); )
    {
		CircleEntity* entity = *it;

        entity->Update();

        if (entity->ToDestroy() == false)
        {
            ++it;
            continue;
        }

        mEntitiesToDestroy.push_back(entity);
        it = mEntities.erase(it);
    }

 	for (auto it = mREntities.begin(); it != mREntities.end(); )
	{
		RectangleEntity* entity = *it;
		entity->Update();
		if (entity->ToDestroy() == false)
		{
			++it;
			continue;
		}
		mREntitiesToDestroy.push_back(entity);
		it = mREntities.erase(it);
	}

    //Collision
    for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
    {
        auto it2 = it1;
        ++it2;
        for (; it2 != mEntities.end(); ++it2)
        {
            CircleEntity* entity = *it1;
            CircleEntity* otherEntity = *it2;

            if (entity->IsColliding(otherEntity))
            {
				if (entity->IsRigidBody() && otherEntity->IsRigidBody())
					entity->Repulse(otherEntity);

                entity->OnCollision(otherEntity);
                otherEntity->OnCollision(entity);
            }
        }
    }

	for (auto it1 = mREntities.begin(); it1 != mREntities.end(); ++it1)
	{
		auto it2 = it1;
		++it2;
		for (; it2 != mREntities.end(); ++it2)
		{
			RectangleEntity* entity = *it1;
			RectangleEntity* otherEntity = *it2;
		}
	}

	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it) 
	{
		delete *it;
	}

	for (auto it = mREntitiesToDestroy.begin(); it != mREntitiesToDestroy.end(); ++it)
	{
		delete* it;
	}

    mEntitiesToDestroy.clear();
	mREntitiesToDestroy.clear();

	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}
	for (auto it = mREntitiesToAdd.begin(); it != mREntitiesToAdd.end(); ++it)
	{
		mREntities.push_back(*it);
	}

	mEntitiesToAdd.clear();
	mREntitiesToAdd.clear();
}

void GameManager::Draw()
{
	mpWindow->clear(mClearColor);
	
	for (CircleEntity* entity : mEntities)
	{
		mpWindow->draw(*entity->GetShape());
	}
	RectangleEntity* tmp;
	for (RectangleEntity* entity : mREntities)
	{
		tmp = entity;
		mpWindow->draw(*tmp->GetShape());
	}
	
	Debug::Get()->Draw(mpWindow);

	mpWindow->display();
}
