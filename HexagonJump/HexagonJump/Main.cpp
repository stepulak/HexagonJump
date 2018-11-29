#include "Player.hpp"
#include "Utils.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace hexagon;

int main()
{
	Player p(100, 100, 50);
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	
	sf::Clock deltaClock;
	float time = 0.f;
	bool keyleft = false;
	bool keyright = false;

	while (window.isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					p.TryToJump();
				}
				if (event.key.code == sf::Keyboard::C) {
					p.TryToFallDownFast();
				}
				if (event.key.code == sf::Keyboard::A) {
					keyleft = true;
				}
				if (event.key.code == sf::Keyboard::D) {
					keyright = true;
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::A) {
					keyleft = false;
				}
				if (event.key.code == sf::Keyboard::D) {
					keyright = false;
				}
			}
		}
		window.clear(sf::Color::Black);

		p.Update(dt, 9.81 * 100);

		sf::CircleShape c(p.GetRadius(), 6);
		c.setPosition(p.GetPosition());
		c.setFillColor(sf::Color(255, 255, 255));
		c.setRotation(p.GetAngle() * 180.f / utils::PI);
		c.setOrigin(p.GetRadius() / 2.f, p.GetRadius() / 2.f);

		/*if (p.GetPosition().y > 200 && p.IsFalling()) {
			p.StopFalling();
		}*/

		if (keyright) {
			p.StartMovingRight(100);
		}
		else {
			p.StopMovingRight();
		}

		window.draw(c);
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}