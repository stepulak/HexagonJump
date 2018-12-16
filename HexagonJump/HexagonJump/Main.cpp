#include "ParticleSystem.hpp"
#include "Utils.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace hexagon;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	Camera camera(960.f, 540.f, 800, 600);

	sf::Clock deltaClock;
	float time = 0.f;
	bool keyleft = false;
	bool keyright = false;

	ParticleSystem p;
	sf::Texture tex;
	tex.loadFromFile("test.jpg");

	while (window.isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					p.AddParticle()
						.SetPosition(100, 100)
						.SetProportions(100, 100)
						.SetVelocity(10)
						.SetAcceleration(50)
						.SetDirectionAngle(PI * 1.5f)
						.SetBodyAngle(PI * 3)
						.SetBodyAngleVelocity(-PI)
						.SetDirectionAngleVelocity(PI * 1.5f)
						.SetProportionsAcceleration(10.f)
						.SetFadeMode(Particle::FadeMode::FADE_OUT | Particle::FadeMode::FADE_IN)
						.SetFadeTime(1.f)
						.SetEndTime(2.f)
						.SetTexture(tex);
				}
				if (event.key.code == sf::Keyboard::C) {
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
		p.Update(dt);
		p.Draw(camera, window);
		window.setView(camera.GetVirtualView());
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}