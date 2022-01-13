#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Tile
{
	sf::Sprite m_sprite;

public:
	Tile(sf::Texture *texture, sf::Vector2f position)
	{
		m_sprite.setTexture(*texture);
		m_sprite.setPosition(position);
	}

	Tile()
	{
	}

	void setTexture(sf::Texture texture)
	{
		m_sprite.setTexture(texture);
	}

	sf::Sprite getSprite()
	{
		return m_sprite;
	}
};

int main()
{
	const int textureSize{ 24 };
	const int boardSize{ 3 };

	sf::RenderWindow window(sf::VideoMode(300, 300), "Uhm");

	int error[3]{};

	sf::Texture tileTexture;
	error[0] = tileTexture.loadFromFile("Resources/Tile.png");
	sf::Texture circleTexture;
	error[1] = circleTexture.loadFromFile("Resources/Circle.png");
	sf::Texture crossTexture;
	error[2] = crossTexture.loadFromFile("Resources/Cross.png");

	for (int i{}; i < *std::end(error); ++i)
	{
		if (error[i] == -1)
		{
			std::cout << "Image " << i << " not loaded";
			return -1;
		}
	}
	std::cout << "Textures loaded successfully\n";

	std::vector<Tile> tileVector;

	for (unsigned int x{}; x < boardSize; ++x)
	{
		for (unsigned int y{}; y < boardSize; ++y)
		{
			tileVector.push_back(Tile(&tileTexture, sf::Vector2f(static_cast<float>(textureSize) * x, static_cast<float>(textureSize) * y)));
		}
	}

	sf::Sprite spriteTest;
	spriteTest.setTexture(tileTexture);

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

		window.draw(spriteTest);

		window.display();
	}

	return 0;
}