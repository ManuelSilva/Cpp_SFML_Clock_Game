#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum EMainGameState
{
	ClockMode = 0,
	TransitionPart1 = 1, // set player in proper position
	MainGameMode = 2,
};

enum EEasingFunc
{
	Linear = 0,
	QuinticInAndOut = 1,
	CubicEaseOut = 2,
	CubicEaseIn = 3,
};

enum ESupportedFonts
{
	Xirod = 0,
};

struct gameManager;

extern gameManager g_GameManager;

struct gameManager
{
	sf::RenderWindow window{ {16 * 100, 9 * 100}, "Circumference Test" };
	sf::View view;

	std::unique_ptr<sf::Font> xirodFont;
	std::unique_ptr <sf::Music> mainTrack;

	std::unique_ptr<sf::SoundBuffer> clickSoundBuffer;
	std::unique_ptr<sf::Sound> clickSoundFx;

	std::unique_ptr<sf::SoundBuffer> clackSoundBuffer;
	std::unique_ptr<sf::Sound> clackSoundFx;

	float deltaTime;

	sf::Clock mainClock;
	sf::Time previousTime;
	sf::Time currentTime;
	std::tm systemClock;
	int currentSec;
	float currentMs; // 0 to 1 value
	
	EMainGameState gameState = EMainGameState::ClockMode	;
	bool m_resizeUpdate;

public:
	int Init();

	void UpdateClock();

	void PollEvents();

	inline std::tm localtime_xp(std::time_t timer)
	{
		std::tm bt{};
#if defined(__unix__)
		localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
		localtime_s(&bt, &timer);
#else
		static std::mutex mtx;
		std::lock_guard<std::mutex> lock(mtx);
		bt = *std::localtime(&timer);
#endif
		return bt;
	}

	static float EasingFunction(EEasingFunc func, float x)
	{
		switch (func)
		{
		case EEasingFunc::CubicEaseIn:
			return x * x * x;
		case EEasingFunc::CubicEaseOut:
			return 1 - std::pow(1 - x, 3);
		case EEasingFunc::QuinticInAndOut:
			return x < 0.5 ? 16 * std::pow(x, 5) : 1 - std::pow(-2 * x + 2, 5) / 2;		
		case EEasingFunc::Linear:
		default:
			return x;
		}

	}

	static int TweenValue(int& from, int to, float& timeElapsed, float speed, EEasingFunc func)
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

		float easedT = EasingFunction(func, timeElapsed);
		return (1.0f - easedT) * from + easedT * to;
	}

	static float TweenValue(float& from, float to, float& timeElapsed, float speed, EEasingFunc func, bool& arrived)
	{
		timeElapsed += g_GameManager.deltaTime * speed;
		if (timeElapsed > 1.0f)
		{
			arrived = true;
			timeElapsed = 0;
			from = to;
			return to;
		}

		float easedT = EasingFunction(func, timeElapsed);
		return (1.0f - easedT) * from + easedT * to;
	}

};
