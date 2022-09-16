#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum EMainGameState
{
	ClockMode = 0,
	MainGameMode = 1,
};

enum ESupportedFonts
{
	Xirod = 0,
};


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
};

extern gameManager g_GameManager;