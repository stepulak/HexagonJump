#pragma once

#include "Particle.hpp"
#include "Pool.hpp"

namespace hexagon {

class ParticleSystem {
public:

	ParticleSystem() = default;

	Particle& AddParticle();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window, const Camera& camera) const;

private:

	static constexpr size_t DEFAULT_POOL_SIZE = 128u;
	
	Pool<Particle> _pool{ DEFAULT_POOL_SIZE };
};

}