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

	if (prevSec > sec && !m_finishedSettingSeconds)
	{
		sec += 60;
	}

	prevSec = sec;
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
		m_currentTheta = gameManager::TweenValue(playerTween2, target, playerTween2ElapsedTime, 0.4f, EEasingFunc::QuinticInAndOut, m_finishedSettingSeconds);
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
	for (int i = 0; i < AMMO_COUNT; ++i)
	{
		auto projectile = &projectiles[i];
		projectile->OnUpdate();
	}

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
	if (g_GameManager.gameState == EMainGameState::ClockMode)
	{
		AutoMove();


		if (!m_finishedSettingSeconds)
		{
			shape.setPosition(getPosition());
			return;
		}
	}
	else
	{
		playerTween2 = m_currentTheta;
		playerTween2ElapsedTime = 0;
		m_finishedSettingSeconds = false;

		float addedAcceleration = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			addedAcceleration += ACCELERATION;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			addedAcceleration -= ACCELERATION;
		}


		m_currentTheta += SimulateMovePlayer(addedAcceleration, delta);
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (g_GameManager.gameState == EMainGameState::ClockMode)
		{
			g_GameManager.gameState = EMainGameState::TransitionPart1;
			playerTween1 = m_currentTheta;
			playerTween1ElapsedTime = 0;

			shape.setPosition(getPosition());
			return;
		}

		if (g_GameManager.gameState == EMainGameState::MainGameMode)
		{
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
	}
	else
	{
		m_PressedShootButton = false;
	}

	shape.setPosition(getPosition());
}

void playerCircle::OnRender()
{
	if (enabled)
	{
		g_GameManager.window.draw(shape);

#if _DEBUG
		auto rect = shape.getGlobalBounds();
		sf::ConvexShape debugShape{ 4 };
		debugShape.setPoint(0, { rect.left, rect.top });
		debugShape.setPoint(1, { rect.left, rect.top + rect.height });
		debugShape.setPoint(2, { rect.left + rect.width, rect.top + rect.height });
		debugShape.setPoint(3, { rect.left + rect.width, rect.top });
		debugShape.setFillColor({ static_cast<sf::Uint8>(255 - _fillColor.r), static_cast<sf::Uint8>(255 - _fillColor.g), 255 / 3, 255 });
		g_GameManager.window.draw(debugShape);
#endif

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
	auto aux = bulletRadius * (m_currentRadius - 0.05f) * 3;
	aux = std::min(aux, bulletRadius);
	return aux;
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
		m_currentRadius = 0.05f;
		numberOfTimesSpawned++;
		startTheta += (numberOfTimesSpawned)*(1.1f + F_PI/10.0f);
		if (startTheta == INFINITY )
		{
			startTheta = 10000;
		}
		shape.setPosition(getPosition());

		g_GameManager.wonTime -= 3000.0f;
		return;
	}

	m_currentRadius += g_GameManager.deltaTime * .2f;

	auto bulletSize = getRadius();
	shape.setRadius(bulletSize);
	shape.setOrigin(bulletSize, bulletSize);
	shape.setPosition(getPosition());
}

void enemySpawner::OnResize()
{
	for (int i = 0; i < ENEMY_COUNT; ++i)
	{
		enemies[i].OnResize();
	}
}

void enemySpawner::OnUpdate()
{
	if (g_GameManager.gameState != EMainGameState::MainGameMode)
	{
		return;
	}

	for (int i = 0; i < ENEMY_COUNT; ++i)
	{
		enemies[i].OnUpdate();
	}
}

void enemySpawner::OnRender()
{
	if (g_GameManager.gameState != EMainGameState::MainGameMode)
	{
		return;
	}

	for (int i = 0; i < ENEMY_COUNT; ++i)
	{
		enemies[i].OnRender();
	}
}

void enemySpawner::InitOnce()
{
	for (int i = 0; i < ENEMY_COUNT; ++i)
	{
		enemies[i].Init();
	}
}
