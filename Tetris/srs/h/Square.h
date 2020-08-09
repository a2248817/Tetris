#pragma once
#include <SFML/Graphics.hpp>
class Square
{
public:
	float              mSize;
	float			   mX;
	float              mY;
	sf::RectangleShape mSquare;
public:
	Square(sf::Color color = sf::Color::Magenta, float outline_size = -1.0f, float size = 40.0f);
	void setPosition();
	

	
};

