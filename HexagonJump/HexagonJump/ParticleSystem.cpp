#include "ParticleSystem.hpp"

namespace hexagon {

Particle & ParticleSystem::AddNewParticle()
{
	return _pool[0];
}

void ParticleSystem::Update(float deltaTime)
{
}

}