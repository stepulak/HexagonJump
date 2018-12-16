#include "ParticleSystem.hpp"
#include <iostream>

namespace hexagon {

Particle& ParticleSystem::AddParticle()
{
	if (_particleIndex >= _pool.size()) {
		_pool.resize(_pool.size() * 2);
	}
	return _pool[_particleIndex++].Clear();
}

void ParticleSystem::Update(float deltaTime)
{
	for (size_t i = 0u; i < _particleIndex;) {
		auto& particle = _pool[i];
		particle.Update(deltaTime);
		if (particle.IsFree()) {
			_particleIndex--;
			if (i < _particleIndex) {
				std::swap(_pool[i], _pool[_particleIndex]);
			}
		}
		else {
			i++;
		}
	}
}

void ParticleSystem::Draw(const Camera& camera, sf::RenderWindow& window)
{
	for (size_t i = 0u; i < _particleIndex; i++) {
		_pool[i].Draw(camera, window);
	}
}

}