#pragma once

#include <list>

#include <GC-simple-render/Geometries/Geometries.h>

class CircleEntity;
class RectangleEntity;
class Scene;
class Debug;

namespace sr 
{
	class Window;
}

class GameManager
{
	std::list<CircleEntity*> mEntities;
	std::list<CircleEntity*> mEntitiesToDestroy;
	std::list<CircleEntity*> mEntitiesToAdd;
	std::list<RectangleEntity*> mREntities;
	std::list<RectangleEntity*> mREntitiesToDestroy;
	std::list<RectangleEntity*> mREntitiesToAdd;

	sr::Window* mpWindow;
	gce::Font mFont;

	Scene* mpScene;
	sr::Camera* m_camera;

	float mDeltaTime;

	int mWindowWidth;
	int mWindowHeight;
	float posMouseX, posMouseY = 0.0f;

	gce::Color mClearColor;

private:
	GameManager();

	void CameraUpdate();
	void Run();
	
	void HandleInput();
	void Update();
	void Draw();
	

	void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }
	public:
	sr::Window* GetWindow() const { return mpWindow; }


	~GameManager();
	void CreateNewWindow(unsigned int width, unsigned int height, const wchar_t* title, int fpsLimit = 60, gce::Color clearColor = gce::Color::Black, int32 flags = 0);
	static GameManager* Get();

	void SetCamera();

	template<typename T>
	void LaunchScene();

	float GetDeltaTime() const { return mDeltaTime; }
	Scene* GetScene() const { return mpScene; }
	sr::Font& GetFont() { return mFont; };
	void SetPos(float x, float y, float z) 
	{ 
		m_camera->SetPosition({ x, y, z }); 
	}
	friend Scene;
	sr::Camera* GetCamera() const { return m_camera; }
};

#include "GameManager.inl"