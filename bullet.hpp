#include <SFML/Graphics.hpp>
#include <cmath>

class Bullet {


public:
	Bullet(const sf::Vector2f position, float angle)
	{
		shape.setRadius(3);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(position);
		shape.setRotation(angle);

		float angleRad = angle * (3.14159f / 180.0f);
		velocity.x = std::cos(angleRad) * speed;
		velocity.y = std::sin(angleRad) * speed;
	}

	void update(float deltaTime)
	{
		shape.move(velocity * deltaTime);
	}

	void draw(sf::RenderWindow& window) const {
		window.draw(shape);
	}

	bool isOffScreen(const sf::RenderWindow& window) const {
		return shape.getPosition().x < 0 || shape.getPosition().x > window.getSize().x ||
			shape.getPosition().y < 0 || shape.getPosition().y > window.getSize().y;
	}

	const sf::CircleShape& getShape() const { return shape; }


private:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	float speed = 600.0f;
};
