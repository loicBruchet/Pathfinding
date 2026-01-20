#pragma once

#include <GC-simple-render/Geometries/Geometries.h>
#include "GCSR.h"

class Scene;

class CircleEntity
{
    struct Target 
    {
		sr::Vector3 position;
        float distance;
		bool isSet;
    };

protected:
    sr::Sphere mShape;
	gce::Vector3f32 mLastPosition;
    sr::Vector3 mDirection;
	Target mTarget;
    float mSpeed = 0.f;
    bool mToDestroy = false;
    int mTag = -1;
	bool mRigidBody = false;
	bool mIsPatroling = false; // new

public:
	bool GoToDirection(int x, int y, int z, float speed =-1.f);
	bool GoToPosition(float x, float y, float z, float speed = -1.f);
	void SetDirection(float x, float y, float z,float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }
	float GetRadius() const { return mShape.radius; }
	void SetRigidBody(bool isRigitBody) { mRigidBody = isRigitBody; }
	void SetPatroling(bool isPatroling) { mIsPatroling = isPatroling; } //new
	bool IsRigidBody() const { return mRigidBody; }
	bool IsPatroling() const { return mIsPatroling; }
	sr::Sphere* GetShape() { return &mShape; }
	sr::Vector3 GetPosition() { return mShape.GetPosition(); }

	bool IsTag(int tag) const { return mTag == tag; }
    bool IsColliding(CircleEntity* other);
	bool IsInside(float x, float y);

    void Destroy();
	void SetPosition(float x, float y, float z);
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>	
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(float radius, const gce::Color& color);

protected:
    CircleEntity() = default;
    ~CircleEntity() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(CircleEntity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void Update();
	void Initialize(float radius, const gce::Vector3f32& color);
	void Repulse(CircleEntity* other);

    friend class GameManager;
    friend Scene;
};

class RectangleEntity
{
	struct Target
	{
		sr::Vector3 position;
		float distance;
		bool isSet;
	};
protected:
	sr::Cube mShape;
	sr::Vector3 mDirection;
	Target mTarget;
	float mSpeed = 0.f;
	bool mToDestroy = false;
	int mTag = -1;
	bool mRigidBody = false;

public:
	bool GoToDirection(int x, int y,int z, float speed = -1.f);
	bool GoToPosition(float x, float y, float z, float speed = -1.f);
	void SetPosition(float x, float y, float z);
	void SetDirection(float x, float y, float z, float speed = -1.f);
	void SetTag(int tag) { mTag = tag; }
	int GetTag() const { return mTag; }
	sr::Vector3 GetPosition() { return mShape.GetPosition(); }
	//sr::Cube GetBounds() const { return  }

	sr::Cube* GetShape() { return &mShape; }

	bool IsTag(int tag) const { return mTag == tag; }
	void Initialize(gce::Vector3f32 perimeter, const gce::Vector3f32& color);
	bool IsInside(float x, float y);

	void Destroy();
	bool ToDestroy() const { return mToDestroy; }

	template<typename T>
	T* GetScene() const;

	Scene* GetScene() const;
	float GetDeltaTime() const;

	template<typename T>
	T* CreateEntity(sr::Vector2 perimeter, const gce::Color& color);

protected:
	RectangleEntity() = default;
	RectangleEntity(int x, int y, bool walkable);
	~RectangleEntity() = default;

	virtual void OnUpdate() {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};

private:
	void Update();

	friend class GameManager;
	friend Scene;
};

#include "Entity.inl"
