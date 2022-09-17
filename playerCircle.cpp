#include "playerCircle.h"

playerCircle::playerCircle(Layout* layout) : centeredCircle(sf::Color::Green, sf::Color::Green, 0, 15, layout)
{
	for (int i = 0; i < AMMO_COUNT; ++i)
	{
		projectiles[i].layout = layout;
		projectiles[i].enabled = false;
	}
}

void playerCircle::InitOnce()
{
	centeredCircle::InitOnce();

	for (int i = 0; i < AMMO_COUNT; ++i)
	{
		projectiles[i].Init();
	}

	m_currentTheta = (100 * F_2PI ) + START_RAD;
	playerTween2 = m_currentTheta;
}


void playerCircle::AutoMove()
{
	int sec = g_GameManager.systemClock.tm_sec;

	auto secDelta = (sec * F_2PI) / 60.0f;
	auto msDelta = (g_GameManager.currentMs * F_2PI) / 60.0f;

	float value = START_RAD + secDelta + msDelta;
	int numberOfSpins = playerTween2 / F_2PI;
	float target = value + ((numberOfSpins) * (F_2PI));

	if (m_finishedSettingSeconds)
	{
		m_currentTheta = target;
	}
	else
	{
		m_currentTheta = gameManager::TweenValue(playerTween2, target, playerTween2ElapsedTime, 0.3f, EEasingFunc::CubicEaseOut, m_finishedSettingSeconds);
	}
}


float playerCircle::SimulateMovePlayer(float addedAcceleration, float delta)
{
	//m_velocity *= DRAG;
	m_velocity = -addedAcceleration * delta;

	if (std::abs(m_velocity) > MAX_VELOCITY)
	{
		m_velocity = m_velocity > 0 ? MAX_VELOCITY : -MAX_VELOCITY;
	}

	return m_velocity * delta;
}

sf::Vector2f playerCircle::calculatePosition(float theta)
{
	float radius = centeredCircle::getRadius() * .89f;
	float x = radius * std::cos(theta);
	float y = radius * std::sin(theta);
	sf::Vector2f radialPosition{ x, y };
	return layout->getCenter() + radialPosition;
}

void playerCircle::OnUpdate()
{
	const float EPSILON = 0.001f;
	float delta = g_GameManager.deltaTime;

	if (g_GameManager.gameState == EMainGameState::TransitionPart1)
	{
		int numberOfSpins = playerTween1 / F_2PI;
		float target = START_RAD + ((numberOfSpins - 2) * (F_2PI));

		bool ignore;
		m_currentTheta = gameManager::TweenValue(playerTween1, target, playerTween1ElapsedTime, .2f, EEasingFunc::CubicEaseOut, ignore);
		shape.setPosition(getPosition());

		if (std::abs(std::abs(m_currentTheta) - std::abs(target)) < 0.001f)
		{
			g_GameManager.gameState = EMainGameState::MainGameMode;
		}

		return;
	}

	float addedAcceleration = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		addedAcceleration += ACCELERATION;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		addedAcceleration -= ACCELERATION;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (g_GameManager.gameState == EMainGameState::ClockMode)
		{
			g_GameManager.gameState = EMainGameState::TransitionPart1;
			playerTween1 = m_currentTheta;
			playerTween1ElapsedTime = 0;
			return;
		}

		//Shoot();
		if (!m_PressedShootButton)
		{
			m_PressedShootButton = true;
			for (int i = 0; i < AMMO_COUNT; ++i)
			{
				auto projectile = &projectiles[i];
				if (!projectile->enabled)
				{
					projectile->InitBullet(m_currentTheta);
					break;
				}
			}
		}
	}
	else
	{
		m_PressedShootButton = false;
	}

	if (g_GameManager.gameState == EMainGameState::ClockMode)
	{
		AutoMove();
	}
	else
	{
		playerTween2 = m_currentTheta;
		playerTween2ElapsedTime = 0;
		m_finishedSettingSeconds = false;

		m_currentTheta += SimulateMovePlayer(addedAcceleration, delta);
	}
	shape.setPosition(getPosition());

	for (int i = 0; i < AMMO_COUNT; ++i)
	{
		auto projectile = &projectiles[i];
		projectile->OnUpdate();
	}
}

void playerCircle::OnRender()
{
	if (enabled)
	{
		g_GameManager.window.draw(shape);

		for (int i = 0; i < AMMO_COUNT; ++i)
		{
			auto projectile = &projectiles[i];
			projectile->OnRender();
		}
		// TODO draw after image
		// Make radius shrink a bit after you achieve max velocity
	}
}

void playerCircle::OnResize()
{
	centeredCircle::OnResize();

	for (int i = 0; i < AMMO_COUNT; ++i)
	{
		auto projectile = &projectiles[i];
		projectile->OnResize();
	}
}

sf::Vector2f playerCircle::getPosition()
{
	return calculatePosition(m_currentTheta);
}

float playerCircle::getRadius()
{
	auto playerRadius = centeredCircle::getRadius() / SIZE_FACTOR;
	return playerRadius;
}

float enemyBullet::getRadius()
{
	auto bulletRadius = centeredCircle::getRadius() / SIZE_FACTOR;
	return bulletRadius;
}

sf::Vector2f enemyBullet::calculatePosition(float theta)
{
	float radius = centeredCircle::getRadius() * m_currentRadius;
	float x = radius * std::cos(theta);
	float y = radius * std::sin(theta);
	sf::Vector2f radialPosition{ x, y };
	return layout->getCenter() + radialPosition;
}

sf::Vector2f enemyBullet::getPosition()
{
	return calculatePosition(startTheta);
}


void enemyBullet::OnUpdate()
{
	if (m_currentRadius > 1.0f)
	{
		m_currentRadius = 0.0f;
		shape.setPosition(getPosition());
		return;
	}

	m_currentRadius += g_GameManager.deltaTime * .4f;
	shape.setPosition(getPosition());
}
