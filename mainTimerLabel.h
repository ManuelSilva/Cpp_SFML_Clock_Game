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

	int m_Tween1 = 0;
	float m_Tween1TimeElapsed = 0;

	int m_Tween2 = 0;
	float m_Tween2TimeElapsed = 0;

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

	float EasingFunction(float x)
	{
		return x < 0.5 ? 16 * std::pow(x, 5) : 1 - std::pow(-2 * x + 2, 5) / 2;
		//return  x * x * x;
	}

	int GetValue(int& from, int to, float& timeElapsed, float speed)
	{
		int aux = std::abs(std::abs(from) - std::abs(to));
		if (aux == 0)
		{
			timeElapsed = 0;
			return to;
		}

		if (aux == 1)
		{
			timeElapsed = 0;
			from = to;
			return to;
		}

		timeElapsed += g_GameManager.deltaTime * speed;
		if (timeElapsed > 1.0f)
		{
			timeElapsed = 0;
			from = to;
			return to;
		}

		float easedT = EasingFunction(timeElapsed);
		return (1.0f - easedT) * from + easedT * to;
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
		float frquency = 1900;
		bool isClock = g_GameManager.gameState == EMainGameState::ClockMode;

		if (isClock)
		{
			minutesLabel1->enabled = enabled;
			minutesLabel2->enabled = enabled;
			secondsLabel1->enabled = enabled;
			secondsLabel2->enabled = enabled;

			auto tempHoursDigit = GetValue(m_Tween1, g_GameManager.systemClock.tm_hour, m_Tween1TimeElapsed, .25f);
			auto hoursDigit1 = tempHoursDigit / 10;
			auto hoursDigit2 = tempHoursDigit % 10;

			if (previousValue1 != hoursDigit1)
			{
				previousValue1 = hoursDigit1;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
				}	
			}

			if (previousValue2 != hoursDigit2)
			{
				previousValue2 = hoursDigit2;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
				}
			}

			minutesLabel1->label.setString(std::to_string(hoursDigit1));
			minutesLabel2->label.setString(std::to_string(hoursDigit2));

			centerLabel->label.setString(":");

			auto tempMinutesDigit = GetValue(m_Tween2, g_GameManager.systemClock.tm_min, m_Tween2TimeElapsed, .25f);
			auto minDigit1 = tempMinutesDigit / 10;
			auto minDigit2 = tempMinutesDigit % 10;

			secondsLabel1->label.setString(std::to_string(minDigit1));
			secondsLabel2->label.setString(std::to_string(minDigit2));

			if (previousValue3 != minDigit1)
			{
				previousValue3 = minDigit1;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
				}
			}

			if (previousValue4 != minDigit2)
			{
				previousValue4 = minDigit2;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
				}
			}

			OnResize();
			return;
		}


		int timeElapsed = time.getElapsedTime().asSeconds();
		int currentTime = startingTime - timeElapsed;

		if (currentTime < 0)
		{
			centerLabel->label.setString("game over");
			OnResize();
			return;
		}

		int minutes = currentTime / 60;
		int secondsRemaining = currentTime - minutes * 60;

		int seconds1 = secondsRemaining / 10;
		int seconds2 = secondsRemaining % 10;

		int minutes1 = (minutes / 10) % 10;
		int minutes2 = minutes % 10;

		if (currentTime > 16)
		{
			minutesLabel1->enabled = enabled;
			minutesLabel2->enabled = enabled;
			secondsLabel1->enabled = enabled;
			secondsLabel2->enabled = enabled;

			auto tempMinutesDigit1 = GetValue(m_Tween1, minutes, m_Tween1TimeElapsed, .25f);
			auto minutesDigit1 = tempMinutesDigit1 / 10;
			auto minutesDigit2 = tempMinutesDigit1 % 10;

			minutesLabel1->label.setString(std::to_string(minutesDigit1));
			minutesLabel2->label.setString(std::to_string(minutesDigit2));

			if (previousValue1 != minutesDigit1)
			{
				previousValue1 = minutesDigit1;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
					//Beep(frquency, 10);
				}
			}

			if (previousValue2 != minutesDigit2)
			{
				previousValue2 = minutesDigit2;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
					//Beep(frquency, 10);
				}
			}


			centerLabel->label.setString(":");

			auto tempSecondsDigit = GetValue(m_Tween2, secondsRemaining, m_Tween2TimeElapsed, .5f);
			auto secDigit1 = tempSecondsDigit / 10;
			auto secDigit2 = tempSecondsDigit % 10;

			secondsLabel1->label.setString(std::to_string(secDigit1));
			secondsLabel2->label.setString(std::to_string(secDigit2));

			if (previousValue3 != secDigit1)
			{
				previousValue3 = secDigit1;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
					//Beep(frquency, 10);
				}
			}

			if (previousValue4 != secDigit2)
			{
				previousValue4 = secDigit2;
				if (g_GameManager.clackSoundFx->getStatus() != sf::SoundSource::Status::Playing)
				{
					//g_GameManager.clackSoundFx->play();
					//Beep(frquency, 10);
				}
			}
		}
		else
		{
			minutesLabel1->enabled = false;
			minutesLabel2->enabled = false;
			secondsLabel1->enabled = false;
			secondsLabel2->enabled = false;

			centerLabel->label.setString(std::to_string(currentTime));
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