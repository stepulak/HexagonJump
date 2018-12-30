#include "BackgroundStripeManager.hpp"
#include "Utils.hpp"

namespace hexagon {

void BackgroundStripeManager::Move(float horizontalDist)
{
	for (size_t i = 0u; i < _stripeIndex; i++) {
		_stripes[i].Move(horizontalDist);
	}
}

void BackgroundStripeManager::Update(const Camera& camera, float deltaTime)
{
	UpdateStripes(camera, deltaTime);
	_stripeSpawnTimer += deltaTime;
	if (_stripeSpawnTimer >= _stripeSpawnTime) {
		_stripeSpawnTimer = 0.f;
		_stripeSpawnTime = Random(static_cast<int>(STRIPE_SPAWN_TIME_MIN) * 10, static_cast<int>(STRIPE_SPAWN_TIME_MAX) * 10) / 10.f;
		SpawnStripe(camera.GetPosition() + camera.GetVirtualWidth(), Random(0.f, static_cast<int>(camera.GetVirtualHeight() / 2.f)));
	}
}

void BackgroundStripeManager::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	for (size_t i = 0u; i < _stripeIndex; i++) {
		_stripes[i].Draw(window, camera, color);
	}
}

void BackgroundStripeManager::UpdateStripes(const Camera& camera, float deltaTime)
{
	for (size_t i = 0u; i < _stripeIndex;) {
		auto& stripe = _stripes[i];
		stripe.Update(deltaTime);
		if (stripe.PassedCamera(camera)) {
			_stripeIndex--;
			if (i < _stripeIndex) {
				std::swap(stripe, _stripes[_stripeIndex]);
			}
		}
		else {
			i++;
		}
	}
}

void BackgroundStripeManager::SpawnStripe(float x, float y)
{
	_stripeIndex++;
	_stripes.emplace_back(x, y);
}

}
