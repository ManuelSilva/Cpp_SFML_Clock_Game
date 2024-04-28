#pragma once

class clockLines : public gameObject
{
	playerCircle* playerRef;

	struct lineState
	{
		bool isLocked = false;
		float currentDelta = 0.0f;
		int lineSize = 0;
		sf::RectangleShape line{};
	};

	lineState states[360]{ };

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
		bool shouldLock = g_GameManager.gameState != EMainGameState::ClockMode;
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
			states[i].line.setSize({ smallestSize / 75, smallestSize / 75 });
			sf::Vector2f og = { states[i].line.getSize().x / 2.0f, states[i].line.getSize().y / 2.0f };

			//if
			(MoveLinesAside(theta, i, thetaAdjust, testF, v, smallestSize, shouldLock, &states[i]));
			{
				states[i].line.setOrigin(og + v);
				states[i].line.setPosition(layout->getCenter());
				states[i].line.setRotation(i);
			}
		}

		for (int i = 0; i < 360; i += 15)
		{
			if (i % 30 == 0)
			{
				continue;
			}

			sf::Vector2f v = { smallestSize / 2.2f, 0 };

			//if
			(MoveLinesAside(theta, i, thetaAdjust, testF, v, smallestSize, shouldLock, &states[i]));
			{
				states[i].line.setSize({ smallestSize / 50, smallestSize / 50 });
				sf::Vector2f og = { states[i].line.getSize().x / 2.0f, states[i].line.getSize().y / 2.0f };
				states[i].line.setOrigin(og + v);
				states[i].line.setPosition(layout->getCenter());
				states[i].line.setRotation(i);
			}
		}

		for (int i = 0; i < 360; i += 30)
		{
			sf::Vector2f v = { smallestSize / 2.2f, 0 };

			MoveLinesAside(theta, i, thetaAdjust, testF, v, smallestSize, shouldLock, &states[i]);

			states[i].line.setSize({ smallestSize / 25, smallestSize / 50 });
			sf::Vector2f og = { states[i].line.getSize().x / 2.0f, states[i].line.getSize().y / 2.0f };
			states[i].line.setOrigin(og + v);
			states[i].line.setPosition(layout->getCenter());
			states[i].line.setRotation(i);
		}
	}

	int mod(int a, int b)
	{
		return (a % b + b) % b;
	}

	void OnRender() override
	{
		for (int i = 0; i < 360; i += 5)
		{
			g_GameManager.window.draw(states[i].line);			
		}
	}


	static int AngleDifference(int angle1, int angle2)
	{
		int aux = (angle2 - angle1 + 180);
		int diff = (aux % 360) - 180;
		return std::abs(diff < -180 ? diff + 360 : diff);
	}

	bool MoveLinesAside(int theta, int i, int thetaAdjust, float testF, sf::Vector2f& v, float smallestSize, bool shouldLock, lineState* state)
	{
		int range = 5;//6
		int currentAngleStart = mod(theta - range, 360);
		int currentAngleEnd = mod(theta + range, 360);
		int indexedAngle = mod((i + thetaAdjust), 360);
		float strength = 0.25f;//.2

		const float baseStartPos = 2.2f;
		float startPos = baseStartPos - state->currentDelta;
		if ((shouldLock && state->isLocked) || (state->isLocked && state->currentDelta > 0.01f))
		{
			startPos = startPos - strength;
		}

		if (g_GameManager.gameState == EMainGameState::MainGameMode)
		{
			state->isLocked = true;
		}

		float distance = 1.0f - ((AngleDifference(indexedAngle, testF) / 180.0f)) - (state->currentDelta)/3.0f;
		if (distance < 0)
		{
			distance = 0;
		}
#if _DEBUG
		state->line.setFillColor(sf::Color(255, 0, 0, distance * 255));
#else
		state->line.setFillColor(sf::Color(0, 255, 0, distance * 255));
#endif
		float angleDiff = std::abs(testF - std::abs(indexedAngle));
		if (currentAngleStart > currentAngleEnd)
		{
			if (indexedAngle >= currentAngleStart && indexedAngle <= 360 || indexedAngle >= 0 && indexedAngle <= currentAngleEnd)
			{
				if (angleDiff > 300)
				{
					angleDiff = std::abs(testF - std::abs(indexedAngle + 360));	
				}

				auto diff = strength * ((range - angleDiff) / range);

				if (std::abs(std::abs(diff) - std::abs(strength)) < 0.1f)
				{
					state->isLocked = shouldLock;
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
			if (indexedAngle >= currentAngleStart && indexedAngle <= currentAngleEnd)
			{
				if (angleDiff > 300)
				{
					angleDiff = std::abs(testF - std::abs(indexedAngle + 360));
				}

				auto diff = strength * ((range - angleDiff) / range);

				if (std::abs(std::abs( diff ) - std::abs( strength )) < 0.1f)
				{
					state->isLocked = shouldLock;
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
		
		if (state->isLocked && startPos < baseStartPos && startPos > 0)
		{
			state->currentDelta += g_GameManager.deltaTime * 0.3f;
		}

		v = { smallestSize / (startPos), 0 };
		
		return false;
	}

protected:
	void InitOnce() override
	{
		OnResize();
	}

};


