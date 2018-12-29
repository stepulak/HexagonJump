#pragma once

#include <SFML/Graphics.hpp>

namespace hexagon {

class BeatUnit {
public:

	static constexpr float MIN_HEIGHT = .33f;
	static constexpr float MAX_HEIGHT = 1.f;

	BeatUnit() = default;

	float Height() const { return _height; }
	
	void SetHeight(float height);
	void Update(float deltaTime);

private:

	static constexpr float HEIGHT_UPDATE_VEL = 2.f;

	float _height = MIN_HEIGHT;
};

}