#include "centeredLabel.h"

centeredLabel::centeredLabel(ESupportedFonts font, sf::Text::Style style, sf::Color fillColor, Layout* layout) : gameObject(layout)
{
	switch (font)
	{
	case ESupportedFonts::Xirod:
	default:
		label.setFont(*g_GameManager.xirodFont);
	}

	label.setStyle(style);
	label.setFillColor(fillColor);
}

void centeredLabel::OnResize()
{
	auto size = layout->getRatioedSize();
	auto stringLength = label.getString().getSize();

	auto maxWidthSize = size.x / stringLength;
	auto smallest = maxWidthSize > size.y ? size.y : maxWidthSize;

	label.setCharacterSize(smallest);
	sf::FloatRect textRect = label.getLocalBounds();
	label.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	auto screenCenter = layout->getCenter();
	label.setPosition(screenCenter);


}

void centeredLabel::OnUpdate()
{

}

void centeredLabel::OnRender()
{
	if (enabled)
	{
		g_GameManager.window.draw(label);
	}
}

void centeredLabel::InitOnce()
{
	OnResize();
}
