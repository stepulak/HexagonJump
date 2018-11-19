#include "BeatUnit.hpp"
#include <iostream>

namespace hexagon {

void BeatUnit::SetHeight(float height)
{
	_height = std::min(MAX_HEIGHT, std::max(height, MIN_HEIGHT));
}

void BeatUnit::Update(float deltaTime)
{
	if (_height > MIN_HEIGHT)
	{
		std::cout << _height << std::endl;
		_height -= HEIGHT_UPDATE_VEL * deltaTime;
		std::cout << _height << std::endl;
	}
	_height = std::max(MIN_HEIGHT, _height);
}

}