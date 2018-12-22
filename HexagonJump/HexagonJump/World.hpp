#pragma once

#include "Obstacle.hpp"

#include <vector>

namespace hexagon {

class World {
public:

	using ObstacleContainer = std::vector<Obstacle::Ptr>;

	void AddObstacle(Obstacle::Ptr obstacle) { _obstacles.emplace_back(std::move(obstacle)); }
	const ObstacleContainer& GetObstacles() const { return _obstacles; }

	void Draw(const Camera& camera) const;

private:

	ObstacleContainer _obstacles;
};

}