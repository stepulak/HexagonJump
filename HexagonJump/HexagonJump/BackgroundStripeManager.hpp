#pragma once

#include "BackgroundStripe.hpp"
#include "Pool.hpp"

namespace hexagon {

class BackgroundStripeManager {
public:

	BackgroundStripeManager() = default;

	void Move(float horizontalDist);
	void Update(const Camera& camera, float deltaTime, bool spawnInsideCamera = false);
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;

private:

	static constexpr size_t STRIPES_DEFAULT_POOL_SIZE = 16u;
	static constexpr float STRIPE_SPAWN_TIME_MIN = 0.5f;
	static constexpr float STRIPE_SPAWN_TIME_MAX = 1.0f;

	sf::Vector2f CountSpawnPosition(const Camera& camera, bool spawnInsideCamera) const;

	void UpdateStripes(const Camera& camera, float deltaTime);
	void SpawnStripe(const sf::Vector2f& position);

	Pool<BackgroundStripe> _pool{ STRIPES_DEFAULT_POOL_SIZE };
	float _stripeSpawnTime = (STRIPE_SPAWN_TIME_MIN + STRIPE_SPAWN_TIME_MAX) / 2.f;
	float _stripeSpawnTimer = 0.f;
};

}