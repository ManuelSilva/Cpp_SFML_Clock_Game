#include "gameManager.h"

int gameManager::Init()
{
	xirodFont = std::make_unique<sf::Font>();
	if (!xirodFont->loadFromFile("../Resources/xirod/xirod.ttf"))
	{
		return EXIT_FAILURE;
	}

	mainTrack = std::make_unique<sf::Music>();
	if (!mainTrack->openFromFile("../Resources/Music/extenz - Sky.ogg"))
	{
		return EXIT_FAILURE;
	}

	clickSoundBuffer = std::make_unique<sf::SoundBuffer>();
	if (!clickSoundBuffer->loadFromFile("../Resources/Sfxs/click.ogg"))
	{
		return EXIT_FAILURE;
	}

	clickSoundFx = std::make_unique<sf::Sound>();
	clickSoundFx->setBuffer(*clickSoundBuffer);

	clackSoundBuffer = std::make_unique<sf::SoundBuffer>();
	if (!clackSoundBuffer->loadFromFile("../Resources/Sfxs/overheat.ogg"))
	{
		return EXIT_FAILURE;
	}

	clackSoundFx = std::make_unique<sf::Sound>();
	clackSoundFx->setBuffer(*clackSoundBuffer);

	view = window.getDefaultView();
	auto size = view.getSize();
	view.setViewport({ 0, 0, size.x, size.y });

	mainClock = sf::Clock::Clock();
	previousTime = mainClock.getElapsedTime();

	return 0;
}

void gameManager::UpdateClock()
{
	systemClock = localtime_xp(std::time(0));
	
	currentTime = mainClock.getElapsedTime();
	deltaTime = (currentTime.asSeconds() - previousTime.asSeconds());
	previousTime = currentTime;

	if (systemClock.tm_sec != currentSec)
	{
		currentSec = systemClock.tm_sec;
		currentMs = 0;
	}
	else
	{
		currentMs += deltaTime;
	}

}

void gameManager::PollEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (event.type == sf::Event::Resized)
		{

			sf::FloatRect visibleArea(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
			window.setView(sf::View(visibleArea));
			view.setViewport(visibleArea);

			m_resizeUpdate = true;
		}
	}
}
