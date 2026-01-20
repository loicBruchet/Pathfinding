	#pragma once

#include "Scene.h"
#include "GameManager.h"
#include "Entity.h"

template<typename T>
T* Scene::CreateEntity(float radius, const gce::Vector3f32 color)
{
	static_assert(std::is_base_of<CircleEntity, T>::value, "T must be derived from Entity");

	T* newEntity = new T();

	CircleEntity* entity = newEntity;
	entity->Initialize(radius, color);
	
	mpGameManager->mEntitiesToAdd.push_back(newEntity);

	return newEntity;
}

template<typename T>
T* Scene::CreateRectEntity(sr::Vector3 perimeter, const gce::Vector3f32& color)
{
	static_assert(std::is_base_of<RectangleEntity, T>::value, "T must be derived from RectangleEntity");
	T* newEntity = new T();
	RectangleEntity* entity = newEntity;
	entity->Initialize(perimeter, color);
	mpGameManager->mREntitiesToAdd.push_back(newEntity);
	return newEntity;
}
