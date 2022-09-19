#include "centeredCircle.h"



centeredCircle::centeredCircle(sf::Color fillColor, sf::Color outlineColor, float outlineThickness, int numberOfPoints, Layout* layout) : 
	gameObject(layout),
	_fillColor(fillColor),
	_outlineColor(outlineColor),
	_outlineThickness(outlineThickness),
	_numberOfPoints(numberOfPoints)
{
	assert(!(outlineThickness < 0 && outlineThickness > 1));

}

centeredCircle::centeredCircle(Layout* layout) : gameObject(layout)
{

}

void centeredCircle::InitOnce()
{
	shape.setFillColor(_fillColor);
	shape.setPointCount(_numberOfPoints);
	shape.setOutlineColor(_outlineColor);
	OnResize();
}

float centeredCircle::getRadius()
{
	return layout->getSmallestSize() / 2.0f;
}

sf::Vector2f centeredCircle::getPosition()
{
	return layout->getCenter(); // default is the screen center
}

void centeredCircle::OnResize()
{
	auto radius = getRadius();
	auto thickness = radius * _outlineThickness;
	radius -= thickness*.99f;

	auto positiion = getPosition();
	shape.setOutlineThickness(thickness);
	shape.setPosition(0, 0);
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	shape.setPosition(positiion.x, positiion.y);
}

void centeredCircle::OnUpdate()
{
	// maybe change color on hit later on
}

void centeredCircle::OnRender()
{
	if (enabled)
	{
		g_GameManager.window.draw(shape);


#if _DEBUG
		auto rect = shape.getGlobalBounds();
		sf::ConvexShape debugShape{ 4 };
		debugShape.setPoint(0, { rect.left, rect.top });
		debugShape.setPoint(1, { rect.left, rect.top + rect.height });
		debugShape.setPoint(2, { rect.left + rect.width, rect.top + rect.height});
		debugShape.setPoint(3, { rect.left + rect.width, rect.top });
		debugShape.setFillColor({static_cast<sf::Uint8>(255 - _fillColor.r), static_cast<sf::Uint8>(255 - _fillColor.g), 255/3, 255});
		
		if (drawDebug)
		{
			g_GameManager.window.draw(debugShape);
		}
#endif

		g_GameManager.window.draw(shape);
	}
}
