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
T* CircleEntity::CreateEntity(float radius, const sf::Color& color)
{
	return GetScene()->CreateEntity<T>(radius, color);
}

template<typename T>
T* RectangleEntity::CreateEntity(sf::Vector2f perimeter, const sf::Color& color)
{
	return GetScene()->CreateEntity<T>(perimeter, color);
}