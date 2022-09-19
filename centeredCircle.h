#pragma once
#include "gameObject.h"

class centeredCircle : public gameObject
{
public:
	sf::Color _fillColor = sf::Color::Transparent;
	sf::Color _outlineColor = sf::Color::Blue;
	float _outlineThickness = 0.03f;
	int _numberOfPoints = 150;
	sf::CircleShape shape{};
	bool drawDebug = false;

	centeredCircle(Layout* layout);

	centeredCircle(sf::Color fillColor, sf::Color outlineColor, float outlineThickness, int numberOfPoints, Layout* layout);

protected:

	void InitOnce() override;

	virtual float getRadius();

	virtual sf::Vector2f getPosition();

protected:

public:
	void OnResize() override;

	void OnUpdate() override;

	void OnRender() override;
};

