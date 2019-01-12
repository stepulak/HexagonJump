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
	
	void SpawnRandomSet(World& world, const sf::FloatRect& setArea) const;

private:
	
	using SetCreator = std::function<void(World& world, const sf::FloatRect& setArea)>;

	static constexpr float SPIKE_WIDTH = 58.f;
	static constexpr float SPIKE_HEIGHT = 67.f;

	static void AddSpike(World& world,
		const sf::FloatRect& area,
		float xCenterRelative, 
		float yCenterRelative, 
		Direction direction);

	static void AddPlatform(World& world, 
		const sf::FloatRect& area,
		float xLeftRelative, 
		float yTopRelative,
		float width, 
		float height);

	static void AddSquareWithSpikes(World& world,
		const sf::FloatRect& area, 
		float xLeftRelative, 
		float yTopRelative, 
		bool skipBottom = false);

	// Ugly, but necessary solution
	void InitSet1();
	void InitSet2();
	void InitSet3();
	void InitSet4();
	void InitSet5();
	void InitSet6();
	void InitSet7();
	void InitSet8();
	void InitSet9();
	void InitSet10();
	void InitSet11();

	std::vector<SetCreator> _setCreators;
};

}