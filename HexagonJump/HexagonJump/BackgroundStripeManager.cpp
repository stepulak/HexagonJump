#include "BackgroundStripeManager.hpp"
#include "Utils.hpp"

namespace hexagon {

void BackgroundStripeManager::Move(float horizontalDist)
{
	for (auto& stripe : _pool) {
		stripe.Move(horizontalDist);
	}
}

void BackgroundStripeManager::Update(const Camera& camera, float deltaTime, bool spawnInsideCamera)
{
	UpdateStripes(camera, deltaTime);
	_stripeSpawnTimer += deltaTime;
	if (_stripeSpawnTimer >= _stripeSpawnTime) {
		_stripeSpawnTimer = 0.f;
		_stripeSpawnTime = Random(
			static_cast<int>(STRIPE_SPAWN_TIME_MIN) * 10,
			static_cast<int>(STRIPE_SPAWN_TIME_MAX) * 10) / 10.f + 0.1f;

		SpawnStripe(CountSpawnPosition(camera, spawnInsideCamera));
	}
}

void BackgroundStripeManager::Draw(sf::RenderWindow& window,
	const Camera& camera,
	const sf::Color& color) const
{
	for (auto& stripe : _pool) {
		stripe.Draw(window, camera, color);
	}
}

void BackgroundStripeManager::UpdateStripes(const Camera& camera, float deltaTime)
{
	for (auto& stripe : _pool) {
		stripe.Update(deltaTime);
	}
	_pool.RemoveAll([&](const auto& stripe) { return stripe.PassedCamera(camera); });
}

sf::Vector2f BackgroundStripeManager::CountSpawnPosition(
	const Camera& camera,
	bool spawnInsideCamera) const
{
	if (spawnInsideCamera) {
		return {
			static_cast<float>(Random(0, static_cast<int>(camera.GetVirtualWidth()))),
			-BackgroundStripe::HEIGHT
		};
	}
	return {
		camera.GetPosition() + camera.GetVirtualWidth(),
		static_cast<float>(Random(0, static_cast<int>(camera.GetVirtualHeight() / 2.f)))
	};
}

}
