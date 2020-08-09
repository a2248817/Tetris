#pragma once
#include <iostream>
#include <time.h>
#include <random>
#include <vector>
#include <array>

#include "Square.h"

class Block
{
private:
	struct Layout
	{
		sf::Vector2f mPosition;
		sf::Vector2f mPoints[4];
	};
	static const Layout    layouts[7];
	static unsigned int    mBlocks;

public:
	std::vector<Square> mSquares;
	Block::Layout       mLayout;

public:
	Block(const sf::Color& color = sf::Color::Magenta, const Block::Layout& layout = layouts[0]);
	~Block();
	void move(float dx, float dy);
	void rotate(float angle);
	void draw(sf::RenderWindow& rw);
	static Block newBlock();
	bool collision(sf::RectangleShape another) ;
};
