#pragma once
#include "centeredCircle.h"
#define F_PI (355.0f / 113.0f)

class projectile : public centeredCircle
{
	const float SIZE_FACTOR = 40.0f;
	const float SPEED_FACTOR = 2.0f;

	float currentTheta;
	float curreentRadius;

protected:
	float getRadius() override
	{
		auto bulletRadius = centeredCircle::getRadius() / SIZE_FACTOR;
		return bulletRadius;
	}

	sf::Vector2f getPosition() override
	{
		float x = curreentRadius * centeredCircle::getRadius() * .82f * std::cos(currentTheta);
		float y = curreentRadius * centeredCircle::getRadius() * .82f * std::sin(currentTheta);
		sf::Vector2f radialPosition{ x, y };
		return layout->getCenter() + radialPosition;
	}

public:

	projectile() : centeredCircle(sf::Color::White, sf::Color::Transparent, 0, 50, nullptr)
	{

	}

	void OnUpdate() override
	{
		if (!enabled)
		{
			return;
		}

		curreentRadius -= SPEED_FACTOR * g_GameManager.deltaTime;

		// Check for collisions
		if (curreentRadius < 0.001f)
		{
			enabled = false;
		}

		shape.setPosition(getPosition());
	}

	void InitBullet(float theta)
	{
		curreentRadius = 1.0f;
		currentTheta = theta;
		enabled = true;
	}

};

class playerCircle : public centeredCircle
{
	// 30 times smaller than biggest circle
	const float SIZE_FACTOR = 30;

	// Player Speed
	const float ACCELERATION = 20;
	const float MAX_VELOCITY = 5;
	const float DRAG = 0.99f;

	static const int AMMO_COUNT = 200;
	projectile projectiles[AMMO_COUNT]{};

	float m_velocity = 0;
	bool m_PressedShootButton = false;

public:
	float m_currentTheta = 90;

private:

	void Shoot();

	void AutoMove();

	float SimulateMovePlayer(float addedAcceleration, float delta);

	sf::Vector2f calculatePosition(float theta);

public:
	playerCircle(Layout* layout);

	void InitOnce() override;

	void OnUpdate() override;

	void OnRender() override;

	void OnResize() override;

protected:
	sf::Vector2f getPosition() override;

	float getRadius() override;
};

class enemyBullet : public centeredCircle
{
	const float SIZE_FACTOR = 20;

	float startTheta = 0;

	float m_currentRadius = 0;
public:
	enemyBullet(Layout* layout) : centeredCircle(sf::Color::Red, sf::Color::Transparent, 0, 50, layout)
	{

	}

protected:
	float getRadius() override;

	sf::Vector2f calculatePosition(float theta);

	sf::Vector2f getPosition() override;

public:

	void OnUpdate() override;
};