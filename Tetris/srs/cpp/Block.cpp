#include "Block.h"


const Block::Layout Block::layouts[7]
{
	//――――

	{sf::Vector2f{+0.0f, +0.0f}, {sf::Vector2f{-2.0f, +0.0f}, sf::Vector2f{-1.0f, +0.0f}, sf::Vector2f{+0.0f, +0.0f}, sf::Vector2f{+1.0f, +0.0f}}},

	//―
	//―――

	{sf::Vector2f{+0.0f, +1.0f}, {sf::Vector2f{-1.0f, -1.0f}, sf::Vector2f{-1.0f, +0.0f}, sf::Vector2f{+0.0f, +0.0f}, sf::Vector2f{+1.0f, +0.0f}}},

	//    ―
	//―――

	{sf::Vector2f{+0.0f, +1.0f}, {sf::Vector2f{+0.0f, -1.0f}, sf::Vector2f{-2.0f, +0.0f}, sf::Vector2f{-1.0f, +0.0f}, sf::Vector2f{+0.0f, +0.0f}}},

	//――
	//――

	{sf::Vector2f{+0.0f, +1.0f}, {sf::Vector2f{-1.0f, -1.0f}, sf::Vector2f{+0.0f, -1.0f}, sf::Vector2f{-1.0f, +0.0f}, sf::Vector2f{+0.0f, +0.0f}}},

	//  ――
	//――

	{sf::Vector2f{+0.0f, +1.0f}, {sf::Vector2f{+0.0f, -1.0f}, sf::Vector2f{+1.0f, -1.0f}, sf::Vector2f{-1.0f, +0.0f}, sf::Vector2f{+0.0f, +0.0f}}},
	
	//  ―
	//―――

	{sf::Vector2f{+0.0f, +1.0f}, {sf::Vector2f{+0.0f, -1.0f}, sf::Vector2f{-1.0f, +0.0f}, sf::Vector2f{+0.0f, +0.0f}, sf::Vector2f{+1.0f, +0.0f}}},

	//――
	//  ――

	{sf::Vector2f{+0.0f, +1.0f}, {sf::Vector2f{-1.0f, -1.0f}, sf::Vector2f{+0.0f, -1.0f}, sf::Vector2f{+0.0f, +0.0f}, sf::Vector2f{+1.0f, +0.0f}}}
};

unsigned int Block::mBlocks{};

Block::Block(const sf::Color& color, const Block::Layout& layout)
	:mLayout(layout)
{
	++mBlocks;
	for (int i = 0; i < std::size(mLayout.mPoints); i++)
	{
		mSquares.emplace_back(color);
		mSquares.at(i).mSquare.setOrigin(-mSquares.at(i).mSize * layout.mPoints[i].x, -mSquares.at(i).mSize * layout.mPoints[i].y);
		mSquares.at(i).mSquare.setPosition(mSquares.at(i).mSize*8, 0.0f+ mSquares.at(i).mSize * layout.mPosition.y);
	}
}

Block::~Block()
{
	--mBlocks;
}

void Block::move(float dx, float dy)
{
	for (auto& square : mSquares)
	{
		square.mSquare.move(square.mSize * dx, square.mSize * dy);
		square.setPosition();
	}
}

void Block::rotate(float angle)
{
	for (auto& square : mSquares)
	{
		square.mSquare.rotate(angle);
		square.setPosition();
	}
}

void Block::draw(sf::RenderWindow& rw)
{
	for (auto& square : mSquares)
		rw.draw(square.mSquare);
}

Block Block::newBlock()
{
	static const sf::Color colors[6]
	{
		sf::Color::Red,
		sf::Color::Yellow,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Cyan,
		sf::Color::Magenta,
	};
	srand(static_cast<unsigned int>(std::time(0)) + mBlocks);
	return Block{ colors[rand() % 6], layouts[rand() % 7] };
}



bool Block::collision(sf::RectangleShape another)
{
	for (auto& square : mSquares)
		if(square.mSquare.getGlobalBounds().intersects(another.getGlobalBounds())) return true;
	return false;
}
