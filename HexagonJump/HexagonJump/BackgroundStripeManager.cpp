#include "BackgroundStripeManager.hpp"
#include "Utils.hpp"
#include <iostream>

namespace hexagon {

void BackgroundStripeManager::Move(float horizontalDist)
{
	for (auto& stripe : _pool) {
		stripe.Move(horizontalDist);
	}
}

void BackgroundStripeManager::Update(const Camera& camera, float deltaTime)
{
	UpdateStripes(camera, deltaTime);
	_stripeSpawnTimer += deltaTime;
	if (_stripeSpawnTimer >= _stripeSpawnTime) {
		_stripeSpawnTimer = 0.f;
		_stripeSpawnTime = Random(static_cast<int>(STRIPE_SPAWN_TIME_MIN) * 10, static_cast<int>(STRIPE_SPAWN_TIME_MAX) * 10) / 10.f + 0.1f;
		SpawnStripe(camera.GetPosition() + camera.GetVirtualWidth(), Random(0, static_cast<int>(camera.GetVirtualHeight() / 2.f)));
	}
}

void BackgroundStripeManager::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	for (auto& stripe : _pool) {
		stripe.Draw(window, camera, color);
	}
}

void BackgroundStripeManager::UpdateStripes(const Camera& camera, float deltaTime)
{
	//stripe.PassedCamera(camera)
	for (auto& stripe : _pool) {
		stripe.Update(deltaTime);
	}
	_pool.RemoveAll([&](const auto& stripe) { return stripe.PassedCamera(camera); });
}

void BackgroundStripeManager::SpawnStripe(float x, float y)
{
	_pool.Add({}).SetPosition(x, y);
}

}
