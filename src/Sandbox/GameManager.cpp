#include "GameManager.h"

#include "Entity.h"

#include <GCSR.h>

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

void GameManager::SetCamera()
{
	m_camera = new sr::Camera(sr::CameraType::PERSPECTIVE);
	m_camera->SetPosition({ 0.0f, 0.0f, 50.0f });
	m_camera->SetRotation({ 180.0f, 0.0f, 0.0f });
	m_camera->SetFOV(gce::PI / 4.0f);
	m_camera->SetNearPlane(0.001f);
	m_camera->SetFarPlane(500.0f);
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

void GameManager::CreateNewWindow(unsigned int width, unsigned int height, const wchar_t* title, int fpsLimit, gce::Color clearColor, int32 flags)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new sr::Window(title, width, height, sr::Window::FLAG_MSAA_X8 | flags);

	mWindowWidth = width;
	mWindowHeight = height;

	mClearColor = clearColor;
}

void GameManager::CameraUpdate()
{
	if (sr::GetKey(sr::Keyboard::Z))
	{
		m_camera->Translate({ 0.0f, 0.0f, -5.0f * mDeltaTime });
	}
	if (sr::GetKey(sr::Keyboard::S))
	{
		m_camera->Translate({ 0.0f, 0.0f, 5.0f * mDeltaTime });
	}
	if (sr::GetKey(sr::Keyboard::Q))
	{
		m_camera->Translate({ -5.0f * mDeltaTime, 0.0f, 0.0f });
	}
	if (sr::GetKey(sr::Keyboard::D))
	{
		m_camera->Translate({ 5.0f * mDeltaTime, 0.0f, 0.0f });
	}
	if (sr::GetKey(sr::Keyboard::SPACE))
	{
		m_camera->Translate({ 0.0f, -5.0f * mDeltaTime, 0.0f });
	}
	if (sr::GetKey(sr::Keyboard::LSHIFT))
	{
		m_camera->Translate({ 0.0f, 5.0f * mDeltaTime, 0.0f });
	}
}

void GameManager::Run()
{
	if (mpWindow == nullptr) 
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateNewWindow(1280, 720, L"Egore");
	}
	
	//#TODO : Load somewhere else
	mFont.Load(L"../../../res/Hack-Regular.ttf");

	_ASSERT(mpScene != nullptr);

	
	posMouseX = sr::GetMousePosition().x;
	posMouseY = sr::GetMousePosition().y;
	while (mpWindow->IsOpen())
	{
		SetDeltaTime(mpWindow->GetDeltaTime());

		HandleInput();

		Update();
		
		Draw();
	}
}

void GameManager::HandleInput()
{
	mpScene->OnEvent();
	CameraUpdate();
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

	mpWindow->Begin(*m_camera);
	for (CircleEntity* entity : mEntities)
	{
		mpWindow->Draw(*entity->GetShape());
	}
	RectangleEntity* tmp;
	for (RectangleEntity* entity : mREntities)
	{
		tmp = entity;
		mpWindow->Draw(*tmp->GetShape());
	}

	mpWindow->End();

	mpWindow->Display();
}
