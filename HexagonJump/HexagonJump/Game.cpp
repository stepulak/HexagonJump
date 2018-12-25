#include "Game.hpp"

#include <cmath>

namespace hexagon {

float Game::CameraSpeedAccordingToDifficulty(float virtualWidth, Difficulty dif)
{
	switch (dif) {
	case Difficulty::EASY:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY;
	case Difficulty::MEDIUM:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY * CAMERA_SPEED_DIFFICULTY_MULTIPLIER;
	case Difficulty::HARD:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY * std::pow(CAMERA_SPEED_DIFFICULTY_MULTIPLIER, 2);
	case Difficulty::PRO:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY * std::pow(CAMERA_SPEED_DIFFICULTY_MULTIPLIER, 3);
	}
	return 0.f;
}

}