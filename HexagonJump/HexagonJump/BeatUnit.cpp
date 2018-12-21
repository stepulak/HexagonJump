#include "BeatUnit.hpp"

namespace hexagon {

void BeatUnit::SetHeight(float height)
{
	_height = std::min(MAX_HEIGHT, std::max(std::max(height, _height), MIN_HEIGHT)); // LOL
}

void BeatUnit::Update(float deltaTime)
{
	_height = std::max(MIN_HEIGHT, _height - HEIGHT_UPDATE_VEL * deltaTime);
}

}