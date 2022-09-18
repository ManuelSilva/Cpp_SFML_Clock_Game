#pragma once

#include "centeredLabel.h"
#include <chrono>
//#include "windows.h"

class mainTimerText : public gameObject
{
	bool m_start = true;

	int startingTime;

	sf::Clock time{};

	centeredLabel* minutesLabel1;
	centeredLabel* minutesLabel2;

	centeredLabel* centerLabel;

	centeredLabel* secondsLabel1;
	centeredLabel* secondsLabel2;

	int previousValue1 = 0;
	int previousValue2 = 0;
	int previousValue3 = 0;
	int previousValue4 = 0;

	//Clock
	int m_Tween1 = 0;
	float m_Tween1TimeElapsed = 0;
	//Clock
	int m_Tween2 = 0;
	float m_Tween2TimeElapsed = 0;

	// Game
	int m_Tween3 = 0;
	float m_Tween3TimeElapsed = 0;

	// Game Blink
	int m_Tween4 = 0;
	float m_Tween4TimeElapsed = 0;

public:

	mainTimerText(int startingTimeInSeconds, centeredLabel* mainLayout, centeredLabel* minutes1, centeredLabel* minutes2, centeredLabel* seconds1, centeredLabel* seconds2)
		: centerLabel(mainLayout),
		minutesLabel1(minutes1),
		minutesLabel2(minutes2),
		secondsLabel1(seconds1),
		secondsLabel2(seconds2),
		startingTime(startingTimeInSeconds),
		gameObject(mainLayout->layout)
	{
		minutesLabel1->label.setString("0");
		minutesLabel2->label.setString("0");

		centerLabel->label.setString(":");

		secondsLabel1->label.setString("0");
		secondsLabel2->label.setString("0");
	}

	void OnResize() override
	{
		minutesLabel1->OnResize();
		minutesLabel2->OnResize();
		centerLabel->OnResize();
		secondsLabel1->OnResize();
		secondsLabel2->OnResize();
	}

	void OnUpdate() override
	{
		minutesLabel1->OnUpdate();
		minutesLabel2->OnUpdate();
		centerLabel->OnUpdate();
		secondsLabel1->OnUpdate();
		secondsLabel2->OnUpdate();

		bool isClock = g_GameManager.gameState == EMainGameState::ClockMode;
		centerLabel->enabled = enabled;

		if (isClock)
		{
			m_Tween3 = g_GameManager.systemClock.tm_hour * 60 + g_GameManager.systemClock.tm_sec;
			minutesLabel1->enabled = enabled;
			minutesLabel2->enabled = enabled;
			secondsLabel1->enabled = enabled;
			secondsLabel2->enabled = enabled;

			auto tempHoursDigit = gameManager::TweenValue(m_Tween1, g_GameManager.systemClock.tm_hour, m_Tween1TimeElapsed, .5f, EEasingFunc::QuinticInAndOut);
			auto hoursDigit1 = tempHoursDigit / 10;
			auto hoursDigit2 = tempHoursDigit % 10;

			minutesLabel1->label.setString(std::to_string(hoursDigit1));
			minutesLabel2->label.setString(std::to_string(hoursDigit2));

			centerLabel->label.setString(":");

			auto tempMinutesDigit = gameManager::TweenValue(m_Tween2, g_GameManager.systemClock.tm_min, m_Tween2TimeElapsed, .25f, EEasingFunc::QuinticInAndOut);
			auto minDigit1 = tempMinutesDigit / 10;
			auto minDigit2 = tempMinutesDigit % 10;

			secondsLabel1->label.setString(std::to_string(minDigit1));
			secondsLabel2->label.setString(std::to_string(minDigit2));

			OnResize();
			return;
		}


		int gameStartTime = time.getElapsedTime().asMilliseconds();
		int currentTime = (startingTime) - gameStartTime + g_GameManager.wonTime;
		auto currentTimeTemp = 0;
		if (g_GameManager.gameState != EMainGameState::MainGameMode)
		{
			time.restart();
			currentTime = startingTime;
			currentTimeTemp = gameManager::TweenValue(m_Tween3, currentTime, m_Tween3TimeElapsed, 0.3f, EEasingFunc::Linear);
		}
		else
		{
			currentTimeTemp = currentTime;
		}

		if (currentTime < 0 && currentTimeTemp < 0)
		{
			minutesLabel1->enabled = false;
			minutesLabel2->enabled = false;
			secondsLabel1->enabled = false;
			secondsLabel2->enabled = false;
			int secondsForBlink = (g_GameManager.currentTime * 1000) / 200;
 			if ((secondsForBlink % 2) == 0)
			{
				centerLabel->enabled = false;
			}
			else
			{
				centerLabel->enabled = enabled;
			}

			if (currentTime < -3000)
			{
				g_GameManager.gameState = EMainGameState::ClockMode;
				g_GameManager.wonTime = 0;
			}

			centerLabel->label.setString("game over");
			OnResize();
			return;
		}

		int minutes = currentTimeTemp / 1000;
		int secondsRemaining = ((currentTimeTemp / 10) % 100) * .6f;

		int seconds1 = secondsRemaining / 10;
		int seconds2 = secondsRemaining % 10;

		//if (currentTimeTemp > 0)
		{
			minutesLabel1->enabled = enabled;
			minutesLabel2->enabled = enabled;
			secondsLabel1->enabled = enabled;
			secondsLabel2->enabled = enabled;

			int secondsForBlink = (g_GameManager.currentTime * 1000) / 200;
			if (currentTimeTemp == startingTime && (secondsForBlink % 2) == 0)
			{
				minutesLabel1->enabled = false;
				minutesLabel2->enabled = false;
				secondsLabel1->enabled = false;
				secondsLabel2->enabled = false;
			}

			auto minutesDigit1 = minutes / 10;
			auto minutesDigit2 = minutes % 10;

			minutesLabel1->label.setString(std::to_string(minutesDigit1));
			minutesLabel2->label.setString(std::to_string(minutesDigit2));

			centerLabel->label.setString(":");

			secondsLabel1->label.setString(std::to_string(seconds1));
			secondsLabel2->label.setString(std::to_string(seconds2));
		}
		//else
		{
			//minutesLabel1->enabled = false;
			//minutesLabel2->enabled = false;
			//secondsLabel1->enabled = false;
			//secondsLabel2->enabled = false;

			//centerLabel->label.setString(std::to_string(currentTime));
		}

		OnResize();
	}

	void OnRender() override
	{
		minutesLabel1->OnRender();
		minutesLabel2->OnRender();
		centerLabel->OnRender();
		secondsLabel1->OnRender();
		secondsLabel2->OnRender();
	}

protected:
	void InitOnce() override
	{
		minutesLabel1->Init();
		minutesLabel2->Init();
		centerLabel->Init();
		secondsLabel1->Init();
		secondsLabel2->Init();

		time.restart();
	}

};