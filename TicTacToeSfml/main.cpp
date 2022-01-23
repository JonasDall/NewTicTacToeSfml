#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <chrono>
#include <thread>
#include <math.h>

enum class tileState
{
	empty,
	circle,
	cross
};

class Tile
{
	sf::Sprite m_tileSprite;
	sf::Sprite m_stateSprite;
	tileState m_state{ tileState::empty };

	sf::Texture* stateTextures[2];

public:
	Tile(sf::Texture *circleTexture, sf::Texture *crossTexture, sf::Texture *tileTexture, sf::Vector2f position) : stateTextures{circleTexture, crossTexture}
	{
		m_tileSprite.setTexture(*tileTexture);
		m_tileSprite.setPosition(position);
		m_stateSprite.setPosition(position);
	}

	void setTexture(sf::Texture texture)
	{
		m_tileSprite.setTexture(texture);

		return;
	}

	void drawTile(sf::RenderWindow &window)
	{
		window.draw(m_tileSprite);
		
		if (m_state == tileState::empty)
		{
			return;
		}
		window.draw(m_stateSprite);
		return;
	}

	void setState(tileState newState)
	{
		m_state = newState;
		updateState();

		return;
	}

	tileState getState()
	{
		return m_state;
	}

	void updateState()
	{
		switch (m_state)
		{
		case tileState::circle:
			m_stateSprite.setTexture(*stateTextures[0]);
			break;
		case tileState::cross:
			m_stateSprite.setTexture(*stateTextures[1]);
			break;
		}

		return;
	}

	bool checkClick(sf::Vector2f location)
	{
		bool isWithinX{ (location.x > m_tileSprite.getPosition().x) && (location.x < (m_tileSprite.getPosition().x + m_tileSprite.getTextureRect().width)) };
		bool isWithinY{ (location.y > m_tileSprite.getPosition().y) && (location.y < (m_tileSprite.getPosition().y + m_tileSprite.getTextureRect().height)) };

		if ((isWithinX && isWithinY) && (m_state == tileState::empty))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void setTileColor(sf::Color color)
	{
		m_tileSprite.setColor(color);
		return;
	}
};

std::string printEnum(tileState state)
{
	std::string returnValue{};

	switch (state)
	{
	case tileState::empty:
		returnValue = "EMPTY";
		break;

	case tileState::circle:
		returnValue = "CIRCLE";
		break;
	case tileState::cross:
		returnValue = "CROSS";
		break;
	}
	return returnValue;
}

std::vector<Tile> getLine()
{
	std::vector<Tile> vector;

	return vector;
}

bool uniformCheck(std::vector<Tile>& inputVector)
{
	bool result{ false };

	for (unsigned int i{1}; i < inputVector.size(); ++i)
	{
		std::cout << "Value " << printEnum(inputVector[0].getState()) << " against " << printEnum(inputVector[i].getState()) << '\n';

		if (inputVector[0].getState() != inputVector[i].getState())
		{
			std::cout << "True!";
			result = true;
			break;
		}
	}

	return result;
}

std::vector<Tile> getLine(std::vector<Tile>& inputVector, unsigned int line, unsigned int rowSize)
{
	std::vector<Tile> outputVector;

	for (unsigned int i{}; i < rowSize; ++i)
	{
		outputVector.push_back(inputVector[(rowSize * i) + line]);
	}

	return outputVector;
}


bool checkWinner(std::vector<Tile>& vector)
{
	bool result{ false };
	unsigned int rowSize{ static_cast<unsigned int>(sqrt(vector.size())) };

	//Check rows
	for (unsigned int i{}; i < rowSize; ++i)
	{
		std::vector<Tile> currentRow{getLine(vector, i, rowSize)};

		result = uniformCheck(currentRow);
		if (result)
		{
			break;
		}
	}

	std::cout << "Rows have a winner: ";
	if (result)
	{
		std::cout << "True!\n";
	}
	else
	{
		std::cout << "False\n";
	}

	//Seperate into rows
	//Check if any rows are uniform
	//Set row tiles as green

	//Seperate into columns
	//Check if any columns are uniform
	//Set column tiles as green

	//Seperate into diagonals
	//Check if any diagonals are uniform
	//Set diagonal tiles as uniform

	return false;
}

int main()
{
	const unsigned int textureSize{ 24 };
	const unsigned int boardSize{ 3 };
	const unsigned int viewSize{ textureSize * boardSize };

	sf::RenderWindow window(sf::VideoMode(boardSize * 100, boardSize * 100), "Uhm");
	sf::View view(sf::Vector2f(viewSize / 2, viewSize / 2), sf::Vector2f(viewSize, viewSize));
	window.setView(view);

	sf::Font font;
	font.loadFromFile("Resources/game_over.ttf");

	sf::Text text;
	text.setFont(font);
	text.setString("Test");
	text.setCharacterSize(50);
	text.setPosition(static_cast<sf::Vector2f>(sf::Vector2i(5, 5)));

	sf::Texture tileTexture;
	tileTexture.loadFromFile("Resources/Tile.png");
	sf::Texture circleTexture;
	circleTexture.loadFromFile("Resources/Circle.png");
	sf::Texture crossTexture;
	crossTexture.loadFromFile("Resources/Cross.png");

	std::vector<Tile> tileVector;

	for (unsigned int x{}; x < boardSize; ++x)
	{
		for (unsigned int y{}; y < boardSize; ++y)
		{
			tileVector.push_back(Tile(&circleTexture, &crossTexture, &tileTexture, sf::Vector2f(textureSize * x, textureSize * y)));
		}
	}

	tileState currentPlayer{ tileState::circle };

	while (window.isOpen())
	{
		sf::Vector2f mouseWorldLocation{};
		bool hasWinner{ false };

		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				mouseWorldLocation = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				std::cout << "World: " << mouseWorldLocation.x << " " << mouseWorldLocation.y << '\n';
			}

		}

		for (unsigned int i{ 0 }; i < tileVector.size(); ++i)
		{
			if (tileVector[i].checkClick(mouseWorldLocation))
			{
				std::cout << i << '\n';
				tileVector[i].setState(currentPlayer);

				if (currentPlayer == tileState::circle)
				{
					currentPlayer = tileState::cross;
				}
				else
				{
					currentPlayer = tileState::circle;
				}

				//Check horizontally for winner

				//Check vertically for winner
			
				//Check diagonally for winner

				hasWinner = checkWinner(tileVector);
			}
		}



		window.clear();

		for (unsigned int i{}; i < tileVector.size(); ++i)
		{
			tileVector[i].drawTile(window);
		}

		window.draw(text);

		window.display();

		if (hasWinner)
		{
			std::this_thread::sleep_for(std::chrono::seconds(5));
			window.close();
		}

	}

	return 0;
}