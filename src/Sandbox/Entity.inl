#include "Entity.h"

#include "GameManager.h"

template<typename T>
T* CircleEntity::GetScene() const
{
	T* pScene = dynamic_cast<T*>(GetScene());

	_ASSERT(pScene != nullptr);

	return pScene;
}


template<typename T>
T* CircleEntity::CreateEntity(float radius, const gce::Color& color)
{
	return GetScene()->CreateEntity<T>(radius, color);
}

template<typename T>
T* RectangleEntity::CreateEntity(sr::Vector2 perimeter, const gce::Color& color)
{
	return GetScene()->CreateEntity<T>(perimeter, color);
}