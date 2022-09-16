#pragma once
#include "gameObject.h"

class centeredLabel : public gameObject
{
public:
	sf::Text label{};

	centeredLabel(ESupportedFonts font, sf::Text::Style style, sf::Color fillColor, Layout* layout);

	centeredLabel() = delete;

	void OnResize() override;

	void OnUpdate() override;

	void OnRender() override;

protected:
	void InitOnce() override;

};