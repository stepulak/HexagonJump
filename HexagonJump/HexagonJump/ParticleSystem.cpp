#include "ParticleSystem.hpp"

namespace hexagon {

Particle& ParticleSystem::AddParticle()
{
	return _pool.Add().Clear();
}

void ParticleSystem::Update(float deltaTime)
{
	for (auto& particle : _pool) {
		particle.Update(deltaTime);
	}
	_pool.RemoveAll([](auto& particle) { return particle.IsFree(); });
}

void ParticleSystem::Draw(sf::RenderWindow& window, const Camera& camera) const
{
	for (auto& particle : _pool) {
		particle.Draw(window, camera);
	}
}

}