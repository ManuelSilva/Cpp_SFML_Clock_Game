#pragma once
#include <assert.h>
#include "gameManager.h"

class Layout
{
	Layout* priorityLayout = nullptr;
	bool isLeft;

	Layout* parent;
	float ratioWidth;
	float ratioHeight;

	float ratioPosX;
	float ratioPosY;

	bool useAspectRatio;
	sf::Vector2f aspectRatio;

#if _DEBUG
	sf::Color debugColor;
	sf::ConvexShape shape { 4 };
	sf::Text temp1{};
#endif
public:
	float getRatioedX()
	{
		if (parent == nullptr)
		{
			return ratioPosX * g_GameManager.view.getViewport().width;
		}

		auto widthAvailable = parent->getRatioedWidth();
		auto x = parent->getRatioedX() + ratioPosX * widthAvailable;
		return x;
	}

	float getRatioedY()
	{
		if (parent == nullptr)
		{
			return ratioPosY * g_GameManager.view.getViewport().height;
		}
		auto heightAvailable = parent->getRatioedHeight();
		auto y = parent->getRatioedY() + ratioPosY * heightAvailable;
		return y;
	}

	sf::Vector2f getRatioedPosition()
	{
		return { getRatioedX(), getRatioedY() };
	}

	float getRatioedWidth()
	{
		if (parent == nullptr)
		{
			return g_GameManager.view.getViewport().width * (ratioWidth - ratioPosX);
		}

		return parent->getRatioedWidth() * (ratioWidth - ratioPosX);
	}
	
	float getRatioedHeight()
	{
		if (parent == nullptr)
		{
			return g_GameManager.view.getViewport().height * (ratioHeight - ratioPosY);
		}

		return parent->getRatioedHeight() * (ratioHeight - ratioPosY);
	}

	sf::Vector2f getRatioedSize()
	{
		return { getRatioedWidth(),  getRatioedHeight() };
	}

	float getSmallestSize()
	{
		auto size = getRatioedSize();
		return size.x > size.y ? size.y : size.x;
	}

	void DebugRender()
	{
#if _DEBUG
		temp1.setFont(*g_GameManager.xirodFont);
		temp1.setString("s");
		auto position = getRatioedPosition();
		auto size = getRatioedSize();
		temp1.setPosition(position);
		sf::Vector2f fontSizeSquare(size.y / 15.0f, size.y / 15.0f);
		temp1.setCharacterSize(fontSizeSquare.x);


		shape.setOrigin(0,0);
		shape.setPosition(0,0);
		shape.setPoint(0, position);
		shape.setPoint(1, { position.x + size.x, position.y });
		shape.setPoint(2, position + size);
		shape.setPoint(3, { position.x, position.y + size.y });
		shape.setFillColor(debugColor);

		g_GameManager.window.draw(shape);
		g_GameManager.window.draw(temp1);
		temp1.setPosition(position + size - fontSizeSquare);
		temp1.setString("e");
		g_GameManager.window.draw(temp1);
#endif
	}

	static Layout GetNewRootLayout()
	{
		return { 0, 0, 1, 1, nullptr, sf::Color::Magenta };
	}

	Layout getNewLayout(float xR, float yR, float wR, float hR, sf::Color color)
	{
		return { xR, yR, wR, hR, this, color };
	}

	Layout getNewLayout(sf::Vector2f aspect, sf::Color color)
	{
		return { aspect, this, color };
	}

	Layout getNewLayout(float percent, sf::Color color)
	{
		return { 1.0f - percent, 1.0f - percent, percent, percent, this, color };
	}

	Layout getLeftLayout(sf::Color color)
	{
		assert(useAspectRatio);
		return {this, true, color};
	}

	Layout getRightLayout(sf::Color color)
	{
		assert(useAspectRatio);
		return { this, false, color };
	}

	void UpdateAspectRatio()
	{
		if (priorityLayout)
		{
			// only works for horizontal set ups (all I need for now)
			if (isLeft)
			{
				UpdateValues(0, 0, priorityLayout->ratioPosX, 1);
			}
			else
			{
				UpdateValues(priorityLayout->ratioWidth, 0, 1, 1);
			}
		}
		else if (useAspectRatio)
		{
			KeepAspectRatio(aspectRatio);
		}
	}

	sf::Vector2f getCenter()
	{
		auto position = getRatioedPosition();
		auto halfSize = getRatioedSize() / 2.0f;
		return position + halfSize;
	}

private:
	Layout(float xR, float yR, float wR, float hR, Layout* layout, sf::Color color) 
		: ratioPosX(xR), ratioPosY(yR), 
		ratioWidth(wR), ratioHeight(hR),
		parent(layout),
		useAspectRatio(false)
#if _DEBUG
		,debugColor(color)
#endif
	{
		// all values must be between 0 and 1
		assert(!(xR < 0 || xR > 1));
		assert(!(yR < 0 || yR > 1));
		assert(!(wR < 0 || wR > 1));
		assert(!(hR < 0 || hR > 1));
	}

	Layout(sf::Vector2f aspect, Layout* layout, sf::Color color) : 
		useAspectRatio(true), 
		aspectRatio(aspect), 
		parent(layout)
#if _DEBUG
		, debugColor(color)
#endif
	{

	}

	Layout(Layout* priority, bool left, sf::Color color) :
		priorityLayout(priority),
		parent(priority->parent),
		isLeft(left)
#if _DEBUG
		, debugColor(color)
#endif
	{

	}

	void KeepAspectRatio(sf::Vector2f aspectRatio)
	{
		float rX = 1;
		float rY = 1;

		SquareTheLayout(aspectRatio, rY, rX, parent->getRatioedSize());
		UpdateValues(1 - rX, 1 - rY, rX, rY);
	}

	void UpdateValues(float xR, float yR, float wR, float hR)
	{
		ratioPosX = xR;
		ratioPosY = yR;

		ratioWidth = wR;
		ratioHeight = hR;
	}

	void SquareTheLayout(sf::Vector2f aspectRatio, float& ratioH, float& ratioW, sf::Vector2f size)
	{
		//ratioH = 1;
		//ratioW = 1;
		// square the root
		float width = size.x;
		float height = size.y;

		// default square is 1:1 you can change to 16:9 4:3 ...
		height *= aspectRatio.x;
		width *= aspectRatio.y;

		if (height > width)
		{
			ratioH = (1 - (width / height)) / 2;
			if (ratioH < .5f)
			{
				ratioH = 1 - ratioH;
			}
		}
		else
		{
			ratioW = (1 - (height / width)) / 2;
			if (ratioW < .5f)
			{
				ratioW = 1 - ratioW;
			}
		}
	}
};
