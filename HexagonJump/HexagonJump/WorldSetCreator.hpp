#pragma once

#include "Utils.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>
#include <vector>

namespace hexagon {

class World;
class BeatUnit;

class WorldSetCreator {
public:

	WorldSetCreator();
	
	void CreateRandomSet(World& world, const sf::FloatRect& setArea) const;

private:
	
	using SetCreator = std::function<void(World& world, const sf::FloatRect& setArea)>;

	float GetSpikeWidth(const sf::FloatRect& setArea) const { return setArea.width * (1 / 25.f); }
	float GetSpikeHeight(const sf::FloatRect& setArea) const { return setArea.height * (1 / 15.f); }

	const BeatUnit& GetRandomBeatUnit(World& world) const;
	void AddSpike(World& world, const sf::FloatRect& area, float xRelative, float yRelative, Direction direction) const;
	void AddPlatform(World& world, const sf::FloatRect& area, float xRelative, float yRelative, float width, float height) const;

	std::vector<SetCreator> _setCreators;
};

}