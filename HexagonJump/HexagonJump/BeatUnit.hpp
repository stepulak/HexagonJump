#pragma once

#include <SFML/Graphics.hpp>

namespace hexagon {

class BeatUnit {
public:

	static constexpr float MIN_HEIGHT = .33f;
	static constexpr float MAX_HEIGHT = 1.f;

	BeatUnit() = default;

	float Height() const { return _height; }
	
	void SetHeight(float height) {
		// Set height within range of <MIN_HEIGHT;MAX_HEIGHT>
		_height = std::min(MAX_HEIGHT, std::max(std::max(height, _height), MIN_HEIGHT));
	}

	void Update(float deltaTime) {
		_height = std::max(MIN_HEIGHT, _height - HEIGHT_UPDATE_VEL * deltaTime);
	}

private:

	static constexpr float HEIGHT_UPDATE_VEL = 2.f;

	float _height = MIN_HEIGHT;
};

}