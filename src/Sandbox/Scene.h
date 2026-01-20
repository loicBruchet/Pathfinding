#pragma once

class GameManager;

#include <GC-simple-render/Geometries/Geometries.h>

class Scene
{
private:
	GameManager* mpGameManager;

private:
	void SetGameManager(GameManager* pGameManager) { mpGameManager = pGameManager; }
	
protected:
	Scene() = default;

	virtual void OnInitialize() = 0;
	virtual void OnEvent() = 0;
	virtual void OnUpdate() = 0;

public:
	template<typename T>
	T* CreateEntity(float radius, const gce::Vector3f32 color);
	template<typename T>
	T* CreateRectEntity(sr::Vector3 perimeter, const gce::Vector3f32& color);

	float GetDeltaTime() const;

	GameManager* GetGameManager() const;

	int GetWindowWidth() const;
	int GetWindowHeight() const;

	friend GameManager;
};

#include "Scene.inl"