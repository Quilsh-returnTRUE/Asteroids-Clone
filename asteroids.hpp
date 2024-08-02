#include <SFML/Graphics.hpp>
#include <cmath>


class Asteroid
{
public:
	Asteroid(float x, float y, float radius, float speedX, float speedY)
		: position(x, y), radius(radius), speed(speedX, speedY)
	{
		shape.setRadius(radius);
		shape.setFillColor(sf::Color::Yellow);
		shape.setPosition(position);

	}
	
	const sf::CircleShape& getShape() const {
		return shape;
	}

	float getRadius() const {
		return shape.getRadius();
	}

	void update(float deltaTime)
	{
		position += speed * speedMultiplier * deltaTime;
		shape.setPosition(position);

		if (position.x < 0) position.x += 900;
		if (position.x > 900) position.x -= 900;
		if (position.y < 0) position.y += 900;
		if (position.y > 900) position.y -= 900;
	}

	void draw(sf::RenderWindow& window) const
	{
		window.draw(shape);
	}

	void setSpeedMultiplier(float multiplier)
	{
		speedMultiplier = multiplier;
	}

	sf::Vector2f getPosition() const {
		return position;
	}

	sf::Vector2f getSpeed() const {
		return speed;
	}





private:
	sf::Vector2f position;
	sf::Vector2f speed;
	float radius;
	sf::CircleShape shape;
	float speedMultiplier = 1.0f;
};


std::vector<Asteroid> createAsteroids(int count)
{
	std::vector<Asteroid> asteroids;
	for (int i = 0; i < count; ++i)
	{
		float x = rand() % 900;
		float y = rand() % 900;
		float radius = (rand() % 20) + 10;
		float speedX = (rand() % 200 - 100) / 50.0f;
		float speedY = (rand() % 200 - 100) / 50.0f;

		asteroids.emplace_back(x, y, radius, speedX, speedY);
	}
	return asteroids;
}
