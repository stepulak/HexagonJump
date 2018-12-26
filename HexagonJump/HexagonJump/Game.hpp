#pragma once

#include "Runnable.hpp"
#include "World.hpp"

namespace hexagon {

class Game final : public Runnable {
public:

	enum class Difficulty {
		EASY,
		MEDIUM,
		HARD,
		PRO
	};

	Game(Camera& camera, Difficulty difficulty);

	void KeyPressed(sf::Keyboard::Key key) override;
	void KeyReleased(sf::Keyboard::Key key) override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) const override;

	static float CameraSpeedAccordingToDifficulty(float virtualWidth, Difficulty dif);

private:

	static constexpr float CAMERA_SPEED_LOWEST_DIFFICULTY = 0.2f;
	static constexpr float CAMERA_SPEED_DIFFICULTY_MULTIPLIER = 2.f;
	static constexpr auto PLAYER_KEY_JUMP = sf::Keyboard::Space;
	static constexpr auto PLAYER_KEY_FALL_DOWN_FAST = sf::Keyboard::C;
	static constexpr auto PLAYER_KEY_MOVE_LEFT = sf::Keyboard::A;
	static constexpr auto PLAYER_KEY_MOVE_RIGHT = sf::Keyboard::D;
	
	World _world;
};

}