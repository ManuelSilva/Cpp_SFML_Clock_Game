#pragma once
#include "centeredCircle.h"
#define F_PI (3.141592653589793115997963468544185161590576171875f)
#define F_2PI (2*F_PI)
#define START_RAD (3*F_PI/2)

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
		bulletRadius *= curreentRadius;
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
			//g_GameManager.wonTime += 200;
			enabled = false;
		}
		auto bulletSize = getRadius();
		shape.setRadius(bulletSize);
		shape.setOrigin(bulletSize, bulletSize);
		shape.setPosition(getPosition());
	}

	void InitBullet(float theta)
	{
		curreentRadius = 1.0f;
		currentTheta = theta;
		enabled = true;
		shape.setPosition(getPosition());
	}

};

class playerCircle : public centeredCircle
{
	// 30 times smaller than biggest circle
	const float SIZE_FACTOR = 30;

	// Player Speed
	const float ACCELERATION = 500;
	const float MAX_VELOCITY = 5;
	const float DRAG = 1.0f;


	float m_velocity = 0;
	bool m_PressedShootButton = false;

	int prevSec = 0;


	float playerTween1 = 0;
	float playerTween1ElapsedTime = 0;

	float playerTween2 = 0;
	float playerTween2ElapsedTime = 0;
	bool m_finishedSettingSeconds = false;
public:
	static const int AMMO_COUNT = 100;
	projectile projectiles[AMMO_COUNT]{};
	float m_currentTheta;

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
	const float SIZE_FACTOR = 25;

	float startTheta = START_RAD + F_PI;

public:

	float m_currentRadius = 0;
	
	enemyBullet() : centeredCircle(sf::Color::Red, sf::Color::Transparent, 0, 50, nullptr)
	{

	}

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

class enemySpawner : public gameObject
{
public:
	static const int ENEMY_COUNT = 100;
	enemyBullet enemies[ENEMY_COUNT]{};

	enemySpawner(Layout* layout) : gameObject(layout)
	{
		for (int i = 0; i < ENEMY_COUNT; ++i)
		{
			enemies[i].layout = layout;
		}
	}

	void OnResize() override;


	void OnUpdate() override;


	void OnRender() override;

protected:
	void InitOnce() override;

};