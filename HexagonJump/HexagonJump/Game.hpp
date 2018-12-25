#pragma once

#include "Camera.hpp"

namespace hexagon {

class Game {
public:

	enum class Difficulty {
		EASY,
		MEDIUM,
		HARD,
		PRO
	};

	static float CameraSpeedAccordingToDifficulty(float virtualWidth, Difficulty dif);

private:

	static constexpr float CAMERA_SPEED_LOWEST_DIFFICULTY = 0.2f;
	static constexpr float CAMERA_SPEED_DIFFICULTY_MULTIPLIER = 2.f;
};

}