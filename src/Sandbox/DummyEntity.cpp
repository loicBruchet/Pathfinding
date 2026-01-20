#include "DummyEntity.h"

#include <iostream>

void DummyEntity::OnCollision(CircleEntity* other)
{
	std::cout << "DummyEntity::OnCollision" << std::endl;
}
