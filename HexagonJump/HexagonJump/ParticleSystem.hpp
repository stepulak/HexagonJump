#pragma once

#include "Particle.hpp"
#include <vector>

namespace hexagon {

class ParticleSystem {
public:

	ParticleSystem() = default;

	Particle& AddParticle();

	void Update(float deltaTime);
	void Draw(const Camera& camera, sf::RenderWindow& window);

private:

	static constexpr size_t DEFAULT_POOL_SIZE = 128u;
	
	std::vector<Particle> _pool{ DEFAULT_POOL_SIZE };
	size_t _particleIndex = 0u;
};

}