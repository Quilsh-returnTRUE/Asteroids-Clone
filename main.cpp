
#include <SFML/Graphics.hpp>
#include "sprites/player.hpp"
#include "sprites/bullet.hpp"
#include "sprites/asteroids.hpp"
#include "sprites/constants.hpp"
#include "sprites/menu.hpp"
#include <vector>
#include <ctime>
#include <Windows.h>
#include <TlHelp32.h>
#include <cmath>
#include <iostream>


struct Vec2
{
	float x, y;
};



bool checkCircleCollision(const sf::CircleShape& a, const sf::CircleShape& b)
{
	const float dx = a.getPosition().x - b.getPosition().x;
	const float dy = a.getPosition().y - b.getPosition().y;
	const float distance = std::sqrt(dx * dx + dy * dy);
	return distance < (a.getRadius() + b.getRadius());
}

bool checkRectCircleCollision(const sf::FloatRect& rect, const sf::CircleShape& circle)
{
	const float closestX = std::clamp(circle.getPosition().x, rect.left, rect.left + rect.width);
	const float closestY = std::clamp(circle.getPosition().y, rect.top, rect.top +rect.height);

	const float dx = circle.getPosition().x - closestX;
	const float dy = circle.getPosition().y - closestY;

	return (dx * dx + dy * dy) < (circle.getRadius() * circle.getRadius());
}

int breakAsteroid(std::vector<Asteroid>& asteroids, size_t index, int score) {
	Asteroid& asteroid = asteroids[index];
	float originalRadius = asteroid.getRadius();
	float newRadius = originalRadius / 2.0f;

	if (newRadius < MIN_ASTEROID_RADIUS) {
		asteroids.erase(asteroids.begin() + index);
		score += 10;
	}
	else {
		for (int i = 0; i < 2; ++i) {	
			float offsetX = (rand() % 100 - 50) / 50.0f;
			float offsetY = (rand() % 100 - 50) / 50.0f;
			sf::Vector2f offset(offsetX, offsetY);

			// Create new asteroid w smaller radius and some velocity
			Asteroid newAsteroid(
				asteroid.getPosition().x,
				asteroid.getPosition().y,							
				newRadius,            
				asteroid.getSpeed().x + offset.x,
				asteroid.getSpeed().y + offset.y
			);
			asteroids.push_back(newAsteroid);
		}
		// Remove the original asteroid
		asteroids.erase(asteroids.begin() + index);
	}
	return score;
}

int main()
{
	
	int score = 0;

	sf::Font font;
	sf::Text scoreText;

	sf::Text livesText;
	sf::Text start;

	if (!font.loadFromFile("sprites/AmaticSC-Regular.ttf"))
	{
		std::cerr << "Error to load font" << std::endl;
		return -1;
	}

	start.setFont(font);
	start.setCharacterSize(24);
	start.setFillColor(sf::Color::White);
	start.setPosition(450.0f, 450.0f);

	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10.0f, 10.0f);


	livesText.setFont(font);
	livesText.setCharacterSize(24);
	livesText.setFillColor(sf::Color::White);
	scoreText.setPosition(10.0f, 40.0f);


	sf::RenderWindow window(sf::VideoMode(900, 900), "Asteroids");
	

	sf::Vector2f player_size = { 60,30 };

	Player player(player_size);
	player.setPos({ 0, 900 });

	sf::Vector2f velocity(0, 0);
	float thrustAcceleration = 200.0f;

	std::vector<Bullet> bullets;
	
	int numAsteroids = 9;

	std::vector<Asteroid> asteroids = createAsteroids(numAsteroids);

	float speedMultiplier = 15.0f;

	sf::Clock clock;



	while (window.isOpen())
	{
		sf::Time deltaTime = clock.restart();


		sf::Event event;

		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				float angle = player.getRotation();
				sf::Vector2f pos = player.get_position();
				bullets.emplace_back(pos, angle);
			}
			
		}



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			player.rotateLeft(90 * deltaTime.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{  
			player.rotateRight(90 * deltaTime.asSeconds());
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			
			velocity = player.rotation(velocity, thrustAcceleration, deltaTime);
		}


		sf::Vector2f currentPos = player.get_position();

		if (player.get_position().x > 900)
		{
			player.setPos({ 0,currentPos.y });
		}

		if (player.get_position().x < 0)
		{
			player.setPos({ 900, currentPos.y });
		}

		if (player.get_position().y > 900)
		{
			player.setPos({ currentPos.x,0 });
		}

		if (player.get_position().y < 0)
		{
			player.setPos({ currentPos.x, 900 });
		}

		scoreText.setString("Score: " + std::to_string(score));
		livesText.setString("Lives: " + std::to_string(player.life));



		for (auto it = bullets.begin(); it != bullets.end();)
		{
			it->update(deltaTime.asSeconds());
			if (it->isOffScreen(window))
			{
				it = bullets.erase(it);
			}
			else
			{
				++it;
			}
		}

		sf::FloatRect playerBounds = player.getBounds();
		for (auto& asteroid : asteroids)
		{
			if (checkRectCircleCollision(playerBounds, asteroid.getShape()))
			{
				player.life -= 1;
				player.setPos({ 450, 450 });
			}
		}

		if (player.life <= 0)
		{
			std::cout << "YOURE DEAD NIGGA" << std::endl;
		}


		for (auto it = bullets.begin(); it != bullets.end();)
		{
			bool bulletRemoved = false;
			for (auto astIt = asteroids.begin(); astIt != asteroids.end();)
			{
				if (checkCircleCollision(it->getShape(), astIt->getShape()))
				{
					std::cout << "bullet hit an asteroid!" << std::endl;
					score = breakAsteroid(asteroids, std::distance(asteroids.begin(), astIt), score);
					it = bullets.erase(it);
					bulletRemoved = true;
					break;
				}
				else
				{
					++astIt;
				}
			}
			if (!bulletRemoved)
			{
				++it;
			}
		}

		// this nga yappin 
		speedMultiplier += deltaTime.asSeconds() * 0.01f;
		for (auto& asteroid : asteroids)
		{
			asteroid.setSpeedMultiplier(speedMultiplier);
		}



		for (auto& asteroid : asteroids)
		{
			asteroid.update(deltaTime.asSeconds());
		}


		player.move(velocity * deltaTime.asSeconds());


		window.clear();
		
		player.drawTo(window);

		for (const auto& bullet : bullets)
		{
			bullet.draw(window);
		}

		for (const auto& asteroid : asteroids)
		{
			asteroid.draw(window);
		}

		window.draw(scoreText);
		window.draw(livesText);

		window.display();

	}
	

	return 0;
}
