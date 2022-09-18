#include "mainTimerLabel.h"
#include "playerCircle.h"
#include <assert.h>
#include <stack>
#include "layout.h"
#include "gameManager.h"
#include "clockLines.h"


gameManager g_GameManager;

int main()
{
	g_GameManager.Init();

	Layout rootLayout = Layout::GetNewRootLayout();
	auto baseCinematicLayout = rootLayout.getNewLayout({ 16, 9 }, sf::Color::Blue);
	auto baseSquareLayout = baseCinematicLayout.getNewLayout({ 1,1 }, sf::Color::Green);
	auto leftOfBaseSquareLayout = baseSquareLayout.getLeftLayout(sf::Color(255.0f / 3.0f, 255 / 3.0f, 0));
	auto rightOfBaseSquareLayout = baseSquareLayout.getRightLayout(sf::Color(255.0f / 3.0f, 0, 255 / 3.0f));
	auto square97 = baseSquareLayout.getNewLayout(0.98f, sf::Color::Yellow);
	auto squareInsideMainCircle = square97.getNewLayout(0.8f, sf::Color::Cyan);
	auto gameTimerArea = squareInsideMainCircle.getNewLayout({ 21, 9 }, sf::Color::Black);
	auto gameTimerAreaSplit1 = gameTimerArea.getNewLayout(0, 0, 1.0f / 5.0f, 1.0f, sf::Color::Blue);
	auto gameTimerAreaSplit2 = gameTimerArea.getNewLayout(1.0f / 5.0f, 0, 2.0f / 5.0f, 1.0f, sf::Color::Magenta);
	auto gameTimerAreaSplit3 = gameTimerArea.getNewLayout(2.0f / 5.0f, 0, 3.0f / 5.0f, 1.0f, sf::Color::Blue);
	auto gameTimerAreaSplit4 = gameTimerArea.getNewLayout(3.0f / 5.0f, 0, 4.0f / 5.0f, 1.0f, sf::Color::Magenta);
	auto gameTimerAreaSplit5 = gameTimerArea.getNewLayout(4.0f / 5.0f, 0, 5.0f / 5.0f, 1.0f, sf::Color::Blue);

	auto ammoBarLayoutArea = rightOfBaseSquareLayout.getNewLayout(0.95f, sf::Color::Magenta);
	auto ammoBarLayoutArea1 = ammoBarLayoutArea.getNewLayout(0,0,1.0f/5.0f,1.0f, sf::Color::Black);
	auto ammoBarLayoutArea2 = ammoBarLayoutArea.getNewLayout(1.0f / 5.0f, 0, 2.0f / 5.0f, 1.0f, sf::Color::White);
	auto ammoBarLayoutArea3 = ammoBarLayoutArea.getNewLayout(2.0f / 5.0f, 0, 1.0f, 1.0f, sf::Color::Black);


	float centerHelperWidth = 0.005f;
	auto centerHelper1 = rootLayout.getNewLayout(0.5f - centerHelperWidth, 0.0f, 0.5f + centerHelperWidth, 1.0f, sf::Color::Red);
	auto centerHelper2 = rootLayout.getNewLayout(0.0f, 0.5f - centerHelperWidth, 1.0f, 0.5f + centerHelperWidth, sf::Color::Red);

	// make sure leafs are last
	Layout* layoutSet[] =
	{
		&rootLayout,
			&baseCinematicLayout,
				&baseSquareLayout,
					&leftOfBaseSquareLayout,
					&rightOfBaseSquareLayout,
						&ammoBarLayoutArea,
							&ammoBarLayoutArea1,
							&ammoBarLayoutArea2,
							&ammoBarLayoutArea3,

					&square97,
						&squareInsideMainCircle,
							&gameTimerArea,
								&gameTimerAreaSplit1,
								&gameTimerAreaSplit2,
								&gameTimerAreaSplit3,
								&gameTimerAreaSplit4,
								&gameTimerAreaSplit5,
			&centerHelper1,
			&centerHelper2
	};

	for (auto layout : layoutSet)
	{
		layout->UpdateAspectRatio();
	}

	centeredLabel timerLabelMin1(ESupportedFonts::Xirod, sf::Text::Bold, sf::Color::Green, &gameTimerAreaSplit1);
	centeredLabel timerLabelMin2(ESupportedFonts::Xirod, sf::Text::Bold, sf::Color::Green, &gameTimerAreaSplit2);
	centeredLabel timerLabel(ESupportedFonts::Xirod, sf::Text::Bold, sf::Color::Green, &gameTimerArea);
	centeredLabel timerLabelSec1(ESupportedFonts::Xirod, sf::Text::Bold, sf::Color::Green, &gameTimerAreaSplit4);
	centeredLabel timerLabelSec2(ESupportedFonts::Xirod, sf::Text::Bold, sf::Color::Green, &gameTimerAreaSplit5);
	mainTimerText timerObj(1000 * 10, &timerLabel, &timerLabelMin1, &timerLabelMin2, &timerLabelSec1, &timerLabelSec2);

	centeredCircle mainCircle(&square97);
	playerCircle player(&square97);
	clockLines line(&square97, &player);
	enemyBullet bullet(&square97);

	gameObject* staticObjects[]
	{
		&timerObj,
		&mainCircle,
		&player,
		&line,
		//&bullet,
	};

	for (auto obj : staticObjects)
	{
		obj->Init();
	}

	sf::RenderWindow& window = g_GameManager.window;
	window.setVerticalSyncEnabled(true);

	g_GameManager.currentTime = g_GameManager.mainClock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		g_GameManager.UpdateClock();

#if _DEBUG
		window.setTitle("FPS: " + std::to_string(1/g_GameManager.frameTime));
#endif
		// get player input
		g_GameManager.PollEvents();

		// clear window
		window.clear(sf::Color::Black);


		// idea make screen shrink and dissapear (by percentage) when you re in the last 5-10 seconds
		//auto oldS = window.getSize();
		//auto newS = sf::Vector2u( static_cast<unsigned int>(0.9999f * oldS.x), static_cast<unsigned int>(.9999f * oldS.y));
		//window.setSize(newS);

		if (g_GameManager.m_resizeUpdate)
		{
			// resize dynamic elements
			for (auto layout : layoutSet)
			{
				layout->UpdateAspectRatio();
			}

			for (auto obj : staticObjects)
			{
				obj->OnResize();
			}

			g_GameManager.m_resizeUpdate = false;
		}


		while (g_GameManager.accumulator >= g_GameManager.deltaTime)
		{
			g_GameManager.systemClock = g_GameManager.localtime_xp(std::time(0));
			if (g_GameManager.systemClock.tm_sec != g_GameManager.currentSec)
			{
				g_GameManager.currentSec = g_GameManager.systemClock.tm_sec;
				g_GameManager.currentMs = 0;
			}
			else
			{
				g_GameManager.currentMs += g_GameManager.deltaTime;
			}

			// update logic
			for (auto obj : staticObjects)
			{
				obj->OnUpdate();
			}
			g_GameManager.accumulator -= g_GameManager.deltaTime;
			g_GameManager.currentTime += g_GameManager.deltaTime;
		}


		for (auto layout : layoutSet)
		{
			layout->DebugRender();
		}
		// render it to the window
		for (auto obj : staticObjects)
		{
			obj->OnRender();
		}

		window.display();
	}
}