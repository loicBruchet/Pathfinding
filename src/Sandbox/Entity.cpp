#include "Entity.h"
#include "GCSR.h"
#include "GameManager.h"
#include "Utils.h"

void CircleEntity::Initialize(float radius, const gce::Vector3f32& color)
{
	mDirection = sr::Vector3(0.0f, 0.0f, 0.0f);
	mShape.SetScale({radius, radius, radius});
	mShape.SetColor(color);
	mShape.SetPosition({ 0.0f, 0.0f, 0.0f });
	mLastPosition = mShape.GetPosition();
	
	mTarget.isSet = false;

	OnInitialize();
}

void RectangleEntity::Initialize(gce::Vector3f32 perimeter, const gce::Vector3f32& color)
{
	mDirection = sr::Vector3(0.0f, 0.0f, 0.0f);
	mShape.SetScale({ perimeter.x, perimeter.y, perimeter.z });
	mShape.SetColor(color);
	mShape.SetPosition({ 0.0f, 0.0f, 0.0f });
	mTarget.isSet = false;
	OnInitialize();
}


void CircleEntity::Repulse(CircleEntity* other)
{

}

bool CircleEntity::IsColliding(CircleEntity* other)
{
	sr::Vector3 distance = mShape.GetPosition() - other->mShape.GetPosition();

	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

	float radius1 = mShape.radius;
	float radius2 = other->mShape.radius;

	float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

	return sqrLength < sqrRadius;
}

bool CircleEntity::IsInside(float x, float y)
{
	sr::Vector3 position = mShape.GetPosition();

	float dx = x - position.x;
	float dy = y - position.y;

	float radius = mShape.radius;

	return (dx * dx + dy * dy) < (radius * radius);
}

bool RectangleEntity::IsInside(float x, float y)
{
	sr::Vector3 position = mShape.GetPosition();
	sr::Vector3 size = mShape.GetScale();
	return (x >= position.x && x <= position.x + size.x &&
		y >= position.y && y <= position.y + size.y);
}

void CircleEntity::Destroy()
{
	mToDestroy = true;

	OnDestroy();
}

void RectangleEntity::Destroy()
{
	mToDestroy = true;
	OnDestroy();
}

void CircleEntity::SetPosition(float x, float y, float z)
{

	mShape.SetPosition({ x, y, z });

	//#TODO Optimise
	if (mTarget.isSet) 
	{
		sr::Vector3 position = mShape.GetPosition();
		mTarget.distance = Utils::GetDistance(position.x, position.y,position.z, mTarget.position.x, mTarget.position.y, mTarget.position.z);
		GoToDirection(mTarget.position.x, mTarget.position.y, mTarget.position.z);
		mTarget.isSet = true;
	}
}

void RectangleEntity::SetPosition(float x, float y, float z)
{
	mShape.SetPosition({ x, y, z });
	//#TODO Optimise
	if (mTarget.isSet)
	{
		sr::Vector3 position = mShape.GetPosition();
		mTarget.distance = Utils::GetDistance(position.x, position.y, position.z, mTarget.position.x, mTarget.position.y, mTarget.position.z);
		GoToDirection(mTarget.position.x, mTarget.position.y, mTarget.position.z);
		mTarget.isSet = true;
	}
}


bool CircleEntity::GoToDirection(int x, int y,int z, float speed)
{
	sr::Vector3 position = mShape.GetPosition();
	sr::Vector3 direction = sr::Vector3(x - position.x, y - position.y, z-position.z);
	
	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;

	SetDirection(direction.x, direction.y, speed);

	return true;
}

bool RectangleEntity::GoToDirection(int x, int y,int z, float speed)
{
	sr::Vector3 position = mShape.GetPosition();
	sr::Vector3 direction = sr::Vector3(x - position.x, y - position.y,z-position.z );
	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;
	SetDirection(direction.x, direction.y, speed);
	return true;
}

bool CircleEntity::GoToPosition(float x, float y,float z, float speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	sr::Vector3 position = mShape.GetPosition();

	mTarget.position = { x, y,z };
	mTarget.distance = Utils::GetDistance(position.x, position.y, position.z,x, y,z);
	mTarget.isSet = true;

	return true;
}

bool RectangleEntity::GoToPosition(float x, float y, float z, float speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;
	sr::Vector3 position = mShape.GetPosition();
	mTarget.position = { x, y, z };
	mTarget.distance = Utils::GetDistance(position.x, position.y, position.z, x, y,z);
	mTarget.isSet = true;
	return true;
}

void CircleEntity::SetDirection(float x, float y, float z, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	mDirection = sr::Vector3(x, y, z);
	mTarget.isSet = false;
}

void RectangleEntity::SetDirection(float x, float y, float z, float speed)
{
	if (speed > 0)
		mSpeed = speed;
	mDirection = sr::Vector3(x, y, z);
	mTarget.isSet = false;
}

void CircleEntity::Update()
{
	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	sr::Vector3 translation = { distance * mDirection.x,distance * mDirection.y,distance * mDirection.z };
	mShape.Translate(translation);

	if (mTarget.isSet) 
	{
		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y,mTarget.position.z);
			mDirection = sr::Vector3(0.f, 0.f, 0.0f);
			mTarget.isSet = false;
			mLastPosition = mShape.GetPosition();
		}
	}

	OnUpdate();
}

void RectangleEntity::Update()
{
	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	sr::Vector3 translation = { distance * mDirection.x,distance * mDirection.y,distance * mDirection.z };
	mShape.Translate(translation);
	if (mTarget.isSet)
	{
		mTarget.distance -= distance;
		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y,mTarget.position.z);
			mDirection = sr::Vector3(0.f, 0.f, 0.f);
			mTarget.isSet = false;
		}
	}
	OnUpdate();
}



Scene* CircleEntity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

Scene* RectangleEntity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float CircleEntity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}

float RectangleEntity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}


