#pragma once

class clockLines : public gameObject
{
	sf::RectangleShape line{};

	playerCircle* playerRef;

	struct lineState
	{
		bool isLocked;
		float currentDelta;
	};

	lineState states[360]{ false };

public:
	clockLines(Layout* layout, playerCircle* player) : playerRef(player),  gameObject(layout)
	{

	}

	void OnResize() override
	{
		// level 1 enemy
		//auto smallestSize = layout->getSmallestSize();
		//line.setSize({ smallestSize/30, smallestSize / 2 });
		//line.setOrigin({ line.getSize().x / 2, line.getSize().y});
		//line.setPosition(layout->getCenter());

		// level 2 enemy
		//auto smallestSize = layout->getSmallestSize();
		//line.setSize({ smallestSize / 50, smallestSize });
		//line.setOrigin({ line.getSize().x/2.0f, line.getSize().y/2.0f });
		//line.setPosition(layout->getCenter());

		// tick lines
		//auto smallestSize = layout->getSmallestSize();
		//line.setSize({ smallestSize / 50, smallestSize / 50 });
		//sf::Vector2f v = { smallestSize/2.2f, 0 };
		//sf::Vector2f og = { line.getSize().x / 2.0f, line.getSize().y / 2.0f };
		//line.setOrigin(og + v);
		//line.setPosition(layout->getCenter());

	}


	void OnUpdate() override
	{
	}

	int mod(int a, int b)
	{
		return (a % b + b) % b;
	}

	void OnRender() override
	{
		bool shouldLock = g_GameManager.gameState != EMainGameState::ClockMode;
		int seconds = g_GameManager.currentTime.asSeconds();
		auto smallestSize = layout->getSmallestSize();

		float radians = playerRef->m_currentTheta;
		float angle = radians * 360 / (2 * F_PI);
		float anglei = static_cast<int>(angle);
		int theta = mod(anglei, 360);
		int thetaAdjust = 180;

		float distanceV = 0.13;
		float testF = fmod(angle, 360.0f);
		if (testF < 0)
		{
			testF += 360;
		}

		for (int i = 0; i < 360; i += 5)
		{
			if (i % 15 == 0 || i % 30 == 0)
			{
				continue;
			}

			sf::Vector2f v = { smallestSize / 2.2f, 0 };
			line.setSize({ smallestSize / 75, smallestSize / 75 });
			sf::Vector2f og = { line.getSize().x / 2.0f, line.getSize().y / 2.0f };

			//if
			(MoveLinesAside(theta, i, thetaAdjust, testF, v, smallestSize, shouldLock, &states[i]));
			{
				line.setOrigin(og + v);
				line.setPosition(layout->getCenter());
				line.setRotation(i + seconds * 0);
				g_GameManager.window.draw(line);
			}
		}

		for (int i= 0; i < 360; i+=15)
		{
			if (i % 30 == 0)
			{
				continue;
			}

			sf::Vector2f v = { smallestSize / 2.2f, 0 };
			
			//if
			(MoveLinesAside(theta, i, thetaAdjust, testF, v, smallestSize, shouldLock, &states[i]));
			{
				line.setSize({ smallestSize / 50, smallestSize / 50 });
				sf::Vector2f og = { line.getSize().x / 2.0f, line.getSize().y / 2.0f };
				line.setOrigin(og + v);
				line.setPosition(layout->getCenter());
				line.setRotation(i + seconds* 0);

				g_GameManager.window.draw(line);
			}		
		}

		for (int i = 0; i < 360; i += 30)
		{
			sf::Vector2f v = { smallestSize / 2.2f, 0 };

			MoveLinesAside(theta, i, thetaAdjust, testF, v, smallestSize, shouldLock, &states[i]);

			line.setSize({ smallestSize / 25, smallestSize / 50 });
			sf::Vector2f og = { line.getSize().x / 2.0f, line.getSize().y / 2.0f };
			line.setOrigin(og + v);
			line.setPosition(layout->getCenter());
			line.setRotation(i + seconds * 0);

			g_GameManager.window.draw(line);
		}
	}

	bool MoveLinesAside(int theta, int i, int thetaAdjust, float testF, sf::Vector2f& v, float smallestSize, bool shouldLock, lineState* state)
	{
		int range = 6;//6
		int currentAngleStart = mod(theta - range, 360);
		int currentAngleEnd = mod(theta + range, 360);
		int indexedAngle = mod((i + thetaAdjust), 360);
		float strength = 0.25f;//.2

		const float baseStartPos = 2.2f;
		float startPos = baseStartPos - state->currentDelta;
		if (shouldLock && state->isLocked)
		{
			startPos = startPos - strength;
		}

		if (currentAngleStart > currentAngleEnd)
		{
			float angleDiff = std::abs(testF - std::abs(indexedAngle));

			if (indexedAngle >= currentAngleStart && indexedAngle <= 360 || indexedAngle >= 0 && indexedAngle <= currentAngleEnd)
			{
				if (angleDiff > 300)
				{
					angleDiff = std::abs(testF - std::abs(indexedAngle + 360));	
				}

				auto diff = strength * ((range - angleDiff) / range);

				if (std::abs(std::abs(diff) - std::abs(strength)) < 0.1f)
				{
					state->isLocked = true;
					state->currentDelta = 0;
				}

				if (diff < 0)
				{
					diff = 0;
				}


				v = { smallestSize / (startPos - diff), 0 };
				return true;
			}
		}
		else
		{
			float angleDiff = std::abs(testF - std::abs(indexedAngle));

			if (indexedAngle >= currentAngleStart && indexedAngle <= currentAngleEnd)
			{
				if (angleDiff > 300)
				{
					angleDiff = std::abs(testF - std::abs(indexedAngle + 360));
				}

				auto diff = strength * ((range - angleDiff) / range);

				if (std::abs(std::abs(diff) - std::abs( strength)) < 0.1f)
				{
					state->isLocked = true;
					state->currentDelta = 0;
				}
				
				if (diff < 0)
				{
					diff = 0;
				}


				v = { smallestSize / (startPos - diff), 0 };
				return true;
			}
		}

		if (startPos < baseStartPos && startPos > 0)
		{
			state->currentDelta += g_GameManager.deltaTime * 0.3f;
		}

		v = { smallestSize / (startPos), 0 };
		
		return false;
	}

protected:
	void InitOnce() override
	{
		line.setFillColor(sf::Color::Green);
		OnResize();
	}

};


