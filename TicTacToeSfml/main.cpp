#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

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
	tileState state{ tileState::empty };

	sf::Texture* stateTextures[2];

public:
	Tile(sf::Texture *circleTexture, sf::Texture *crossTexture, sf::Texture *tileTexture, sf::Vector2f position) : stateTextures{circleTexture, crossTexture}
	{
		m_tileSprite.setTexture(*tileTexture);
		m_tileSprite.setPosition(position);
	}

	void setTexture(sf::Texture texture)
	{
		m_tileSprite.setTexture(texture);

		return;
	}

	sf::Sprite getSprite()
	{
		return m_tileSprite;
	}

	void setState(tileState newState)
	{
		state = newState;
		updateState();

		return;
	}

	void updateState()
	{
		switch (state)
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
};

int main()
{
	const int textureSize{ 24 };
	const int boardSize{ 3 };
	const int viewSize{ textureSize * boardSize };

	sf::RenderWindow window(sf::VideoMode(boardSize * 100, boardSize * 100), "Uhm");
	sf::View view(sf::Vector2f(viewSize / 2, viewSize / 2), sf::Vector2f(viewSize, viewSize));
	window.setView(view);

	int error[3]{};
	sf::Texture tileTexture;
	error[0] = tileTexture.loadFromFile("Resources/Tile.png");
	sf::Texture circleTexture;
	error[1] = circleTexture.loadFromFile("Resources/Circle.png");
	sf::Texture crossTexture;
	error[2] = crossTexture.loadFromFile("Resources/Cross.png");

	for (int i{}; i < sizeof(error) / sizeof(error[0]); ++i)
	{
		std::cout << "Image " << i << '\n';
		if (error[i] == -1)
		{
			std::cout << "Image " << i << " not loaded\n";
			return -1;
		}
	}
	std::cout << "Textures loaded successfully\n";

	std::vector<Tile> tileVector;

	for (unsigned int x{}; x < boardSize; ++x)
	{
		for (unsigned int y{}; y < boardSize; ++y)
		{
			tileVector.push_back(Tile(&circleTexture, &crossTexture, &tileTexture, sf::Vector2f(textureSize * x, textureSize * y)));
		}
	}

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear();

		for (unsigned int i{}; i < tileVector.size(); ++i)
		{
			window.draw(tileVector[i].getSprite());
		}

		window.display();
	}

	return 0;
}