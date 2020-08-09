#include <thread>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Square.h"
#include "Block.h"



constexpr int window_position_x { 400  };
constexpr int window_position_y { 5    };
constexpr int window_width      { 1000 };
constexpr int window_height     { 1000 };
constexpr int square_size       { 40   };

bool game_over{ false };
bool isFalling{ true };


bool collision(std::vector<Block>& blocker)
{
	for (int i = 0; i < blocker.size() - 1; i++)
		for (int j = 0; j < blocker.at(i).mSquares.size(); j++)
			if (blocker.at(blocker.size() - 1).collision(blocker.at(i).mSquares.at(j).mSquare)) return true;
	return false;
}

void clear(std::vector<Block>& blocker, int y=24)
{
	if (y < 0)
		return;
	int num{};
	sf::RectangleShape rs{ sf::Vector2f{600.0f, 20.0f} };
	rs.setPosition(sf::Vector2f{0.0f , 10 + 40.0f * y });
	for (int i = 0; i < blocker.size()-1; i++)
		for (int j = 0; j < blocker.at(i).mSquares.size(); j++)
			if (rs.getGlobalBounds().intersects(blocker.at(i).mSquares.at(j).mSquare.getGlobalBounds())) ++num;

	if (num >= 15)
	{
		for (int i = 0; i < blocker.size() - 1; i++)
			for (int j = 0; j < blocker.at(i).mSquares.size(); j++)
			{
				if (rs.getGlobalBounds().intersects(blocker.at(i).mSquares.at(j).mSquare.getGlobalBounds()))
				{
					blocker.at(i).mSquares.erase(blocker.at(i).mSquares.begin() + j);
					j--;
					continue;
				}

				if (blocker.at(i).mSquares.at(j).mSquare.getGlobalBounds().top < rs.getGlobalBounds().top)
					blocker.at(i).mSquares.at(j).mSquare.move(0.0f, 40.0f);
			}
		return clear(blocker, y);
	}
					
	return clear(blocker, y - 1);

}

int main()
{
	sf::RenderWindow rw{ sf::VideoMode{window_width, window_height}, "Teris", sf::Style::Close};
	rw.setPosition(sf::Vector2i{ window_position_x, window_position_y });
	rw.setVerticalSyncEnabled(true);

	sf::Texture background;
	background.loadFromFile("background.png");
	
	sf::Sprite bg;
	bg.setTexture(background);

	//top_bound
	sf::RectangleShape top{ sf::Vector2f{600.0f, 1.0f} };
	top.setPosition(sf::Vector2f{ 0.0f, -2.0f });

	//down_bound
	sf::RectangleShape down{ sf::Vector2f{600.0f, 1.0f} };
	down.setPosition(sf::Vector2f{ 0.0f, 1001.0f });

	//left_bound
	sf::RectangleShape left{ sf::Vector2f{1.0f, 1000.0f} };
	left.setPosition(sf::Vector2f{ -2.0f, 0.0f });

	//right_bound
	sf::RectangleShape right{ sf::Vector2f{1.0f, 1000.0f} };
	right.setPosition(sf::Vector2f{ 601.0f, 0.0f });
	right.setFillColor(sf::Color::Black);

	sf::Font font;
	font.loadFromFile("NotoSansCJKtc-Black.otf");

	sf::Text fps;
	fps.setFont(font);
	fps.setFillColor(sf::Color::Green);
	fps.setPosition(sf::Vector2f{ 700, 50 });
	fps.setCharacterSize(50);

	sf::Text htp;
	htp.setFont(font);
	htp.setFillColor(sf::Color::Magenta);
	htp.setPosition(sf::Vector2f{ 650, 300 });
	htp.setCharacterSize(20);
	htp.setString(L"How to play?\n\nGo left: ¡ö\nGo right: ¡÷\nGo down: ¡õ\nGo to bottom: Space\nRotate clockwise: <\nRotate counterclockwise: >\n Quit: Esc");
	
	int frame{};

	std::vector<Block> blocker;
	blocker.push_back(Block::newBlock());
	Block* block = &blocker.at(blocker.size()-1);

	std::thread autodown{ 
	[&]()
	{
	 static sf::Time time{sf::Time::Zero};
	 static sf::Clock clock; 
	 while (rw.isOpen() && !game_over)
	 {
		time += clock.restart();
		if (time.asMilliseconds() >= 500 && isFalling)
		{
			block->move(0.0f, 1.0f);
			block->collision(down);
			if (block->collision(down) || collision(blocker))
			{
				block->move(0.0f, -1.0f);
				blocker.push_back(Block::newBlock());
				block = &blocker.at(blocker.size() - 1);
				if (collision(blocker)) game_over = true;
			}
			time = sf::Time::Zero;
		}
	 }
	} 
	};
	autodown.detach();
	
	while (rw.isOpen() && !game_over)
	{
		static sf::Clock clock;
		sf::Event event;
		while (rw.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					block->move(0.0f, 1.0f);
					block->collision(down);
					if (block->collision(down) || collision(blocker))
					{
						block->move(0.0f, -1.0f);
						blocker.push_back(Block::newBlock());
						block = &blocker.at(blocker.size() - 1);
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					block->move(-1.0f, 0.0f);
					block->collision(left);
					if (block->collision(left) || collision(blocker))
						block->move(1.0f, 0.0f);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					block->move(1.0f, 0.0f);
					block->collision(right);
					if (block->collision(right) || collision(blocker))
						block->move(-1.0f, 0.0f);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
				{
					block->rotate(-90.0f);
					if (block->collision(top) ||
						block->collision(down) ||
						block->collision(left) ||
						block->collision(right) || 
						collision(blocker))block->rotate(90.0f);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
				{
					block->rotate(+90.0f);
					if (block->collision(top) ||
						block->collision(down) ||
						block->collision(left) ||
						block->collision(right) ||
						collision(blocker))block->rotate(-90.0f);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					static sf::Time time{ sf::Time::Zero };
					static sf::Clock clock;
					time += clock.restart();
					if (time.asMilliseconds() >= 250 && !collision(blocker))
					{
						isFalling = false;
						while (!block->collision(down) && !collision(blocker))
							block->move(0.0f, 1.0f);
						block->move(0.0f, -1.0f);
						blocker.emplace_back(Block::newBlock());
						block = &blocker.at(blocker.size() - 1);
						time = sf::Time::Zero;
						isFalling = true;

					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					rw.close();
				break;
			case sf::Event::Closed:
				rw.close();
				break;
			}
		}

	rw.clear(sf::Color::White);

	rw.draw(bg);

	for (auto& block : blocker)
		block.draw(rw);
	rw.draw(right);
	
	
	frame = 1 / clock.restart().asSeconds();
	fps.setString("FPS: "+std::to_string(frame));
	rw.draw(fps);
	rw.draw(htp);
	rw.display();
	clear(blocker);
	}
}