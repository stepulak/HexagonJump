#pragma once

#include "BackgroundStripe.hpp"
#include "Pool.hpp"

namespace hexagon {

class BackgroundStripeManager {
public:

	BackgroundStripeManager() = default;

	void Move(float horizontalDist);
	void Update(const Camera& camera, float deltaTime);
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;

private:

	static constexpr size_t STRIPES_DEFAULT_POOL_SIZE = 16u;
	static constexpr float STRIPE_SPAWN_TIME_MIN = 0.5f;
	static constexpr float STRIPE_SPAWN_TIME_MAX = 1.0f;

	void UpdateStripes(const Camera& camera, float deltaTime);
	void SpawnStripe(float x, float y);

	Pool<BackgroundStripe> _pool{ STRIPES_DEFAULT_POOL_SIZE };
	float _stripeSpawnTime = (STRIPE_SPAWN_TIME_MIN + STRIPE_SPAWN_TIME_MAX) / 2.f;
	float _stripeSpawnTimer = 0.f;
};

}