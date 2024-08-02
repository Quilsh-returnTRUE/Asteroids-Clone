#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

class Player {
public:
	Player(sf::Vector2f size)
	{
		player.setSize(size);
		player.setFillColor(sf::Color::Green);
		player.setOrigin(size.x / 2.0f, size.y / 2.0f);
	}

	int life = 3;

	void drawTo(sf::RenderWindow& window)
	{
		window.draw(player);

	}
	sf::FloatRect getBounds() const {
		return player.getGlobalBounds();
	}

	void move(sf::Vector2f distance)
	{
		player.move(distance);
	}

	void setPos(sf::Vector2f newPos)
	{
		player.setPosition(newPos);
	}
	
	sf::Vector2f get_position() const {

		return player.getPosition();
	}


	void rotateLeft(float anglePerSec)
	{
		player.rotate(-anglePerSec);
	}

	void rotateRight(float anglePerSec)
	{
		player.rotate(anglePerSec);
	}


	sf::Vector2f rotation(sf::Vector2f velocity, float thrustAcceleration, sf::Time dTime)
	{
		float angle = player.getRotation() * (3.14159f / 180.0f);
		sf::Vector2f thrustDirection(std::cos(angle), std::sinf(angle));

		velocity += thrustDirection * thrustAcceleration * dTime.asSeconds();
		return velocity;
	}


	float getRotation()
	{
		return player.getRotation();
	}


private:
	sf::RectangleShape player;
	sf::Vector2f speed;
	float radius;
	sf::CircleShape shape;
	float speedMulitplier = 1.0f;
};
