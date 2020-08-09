#include "Square.h"

Square::Square(sf::Color color, float outline_size, float size)
	:mSize(size), mSquare{ sf::Vector2f{mSize, mSize} }
{
	mSquare.setFillColor(color);
	mSquare.setOutlineThickness(outline_size);
	mSquare.setOutlineColor(sf::Color::Black);
}

void Square::setPosition()
{
	mX = mSquare.getPosition().x - mSquare.getOrigin().x;
	mY = mSquare.getPosition().y - mSquare.getOrigin().y;
}


