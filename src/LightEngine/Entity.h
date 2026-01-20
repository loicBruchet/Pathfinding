#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace sf 
{
	class Shape;
    class Color;
}

class Scene;

class CircleEntity
{
    struct Target 
    {
		sf::Vector2i position;
        float distance;
		bool isSet;
    };

protected:
    sf::CircleShape mShape;
	sf::Vector2f mLastPosition;
    sf::Vector2f mDirection;
	Target mTarget;
    float mSpeed = 0.f;
    bool mToDestroy = false;
    int mTag = -1;
	bool mRigidBody = false;
	bool mIsPatroling = false; // new

public:
	bool GoToDirection(int x, int y, float speed = -1.f);
    bool GoToPosition(int x, int y, float speed = -1.f);
    void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetDirection(float x, float y, float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }
	float GetRadius() const { return mShape.getRadius(); }
	void SetRigidBody(bool isRigitBody) { mRigidBody = isRigitBody; }
	void SetPatroling(bool isPatroling) { mIsPatroling = isPatroling; } //new
	bool IsRigidBody() const { return mRigidBody; }
	bool IsPatroling() const { return mIsPatroling; }
    sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
	sf::Shape* GetShape() { return &mShape; }

	bool IsTag(int tag) const { return mTag == tag; }
    bool IsColliding(CircleEntity* other) const;
	bool IsInside(float x, float y) const;

    void Destroy();
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>	
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(float radius, const sf::Color& color);

protected:
    CircleEntity() = default;
    ~CircleEntity() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(CircleEntity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void Update();
	void Initialize(float radius, const sf::Color& color);
	void Repulse(CircleEntity* other);

    friend class GameManager;
    friend Scene;
};

class RectangleEntity
{
	struct Target
	{
		sf::Vector2i position;
		float distance;
		bool isSet;
	};
protected:
	sf::RectangleShape mShape;
	sf::Vector2f mDirection;
	Target mTarget;
	float mSpeed = 0.f;
	bool mToDestroy = false;
	int mTag = -1;
	bool mRigidBody = false;

public:
	bool GoToDirection(int x, int y, float speed = -1.f);
	bool GoToPosition(int x, int y, float speed = -1.f);
	void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetDirection(float x, float y, float speed = -1.f);
	void SetTag(int tag) { mTag = tag; }
	sf::FloatRect GetBounds() const { return mShape.getGlobalBounds(); }

	sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
	sf::Shape* GetShape() { return &mShape; }

	bool IsTag(int tag) const { return mTag == tag; }
	bool IsInside(float x, float y) const;

	void Destroy();
	bool ToDestroy() const { return mToDestroy; }

	template<typename T>
	T* GetScene() const;

	Scene* GetScene() const;
	float GetDeltaTime() const;

	template<typename T>
	T* CreateEntity(sf::Vector2f perimeter, const sf::Color& color);

protected:
	RectangleEntity() = default;
	RectangleEntity(int x, int y, bool walkable);
	~RectangleEntity() = default;

	virtual void OnUpdate() {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};

private:
	void Update();
	void Initialize(sf::Vector2f perimeter, const sf::Color& color);

	friend class GameManager;
	friend Scene;
};

#include "Entity.inl"
