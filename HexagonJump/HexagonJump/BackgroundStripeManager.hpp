#pragma once

#include "BackgroundStripe.hpp"

#include <vector>

namespace hexagon {

class BackgroundStripeManager {
public:

	BackgroundStripeManager() { _stripes.reserve(STRIPES_DEFAULT_POOL_SIZE); }

	void Move(float horizontalDist);
	void Update(const Camera& camera, float deltaTime);
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;

private:

	static constexpr size_t STRIPES_DEFAULT_POOL_SIZE = 16u;
	static constexpr float STRIPE_SPAWN_TIME_MIN = 1.f;
	static constexpr float STRIPE_SPAWN_TIME_MAX = 4.f;

	void UpdateStripes(const Camera& camera, float deltaTime);
	void SpawnStripe(float x, float y);

	size_t _stripeIndex = 0u;
	std::vector<BackgroundStripe> _stripes;
	float _stripeSpawnTime = (STRIPE_SPAWN_TIME_MIN + STRIPE_SPAWN_TIME_MAX) / 2.f;
	float _stripeSpawnTimer = 0.f;
};

}