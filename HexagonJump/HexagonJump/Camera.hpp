#pragma once

#include <utility>

namespace hexagon {

class Camera {
public:

	Camera(float virtualScreenWidth, float virtualScreenHeight, float realScreenWidth, float realScreenHeight)
		: _virtualScreenWidth(virtualScreenWidth)
		, _virtualScreenHeight(virtualScreenHeight)
		, _realScreenWidth(realScreenWidth)
		, _realScreenHeight(realScreenHeight) {
	}

	std::pair<float, float> GetResolutionScale() {
		return std::make_pair(_realScreenWidth / _virtualScreenWidth, _realScreenHeight / _virtualScreenHeight);
	}

	std::pair<float, float> GetResolutionScaleInversed() {
		return std::make_pair(_virtualScreenWidth / _realScreenWidth, _virtualScreenHeight / _realScreenHeight);
	}

private:

	//float _x;
	//float _y;
	float _virtualScreenWidth;
	float _virtualScreenHeight;
	float _realScreenWidth;
	float _realScreenHeight;
};
}