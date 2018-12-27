#include "WorldSetCreator.hpp"
#include "Platform.hpp"
#include "Spike.hpp"
#include "World.hpp"

namespace hexagon {

WorldSetCreator::WorldSetCreator()
{
	/*_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto spikeHeight = GetSpikeHeight(area);
		size_t numSpikeGroups = 3u;
		size_t numSpikesPerGroup = 2u;
		auto groupOffset = (area.width - numSpikeGroups * numSpikesPerGroup * spikeWidth) / (1 + numSpikeGroups);

		for (size_t i = 0u; i < numSpikeGroups; i++) {
			for (size_t j = 0u; j < numSpikesPerGroup; j++) {
				AddSpike(world, area, groupOffset + i * groupOffset + j * spikeWidth + spikeWidth / 2, area.height, Direction::UP);
			}
		}
	});*/

	/*_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto spikeHeight = GetSpikeHeight(area);
		size_t numSpikesOnPlatform = 3u;
		auto platformWidth = numSpikesOnPlatform * spikeWidth;
		auto platformHeight = 2 * spikeHeight;
		auto platformX = area.width * 0.3f;
		auto platformY = area.height - platformHeight;

		AddPlatform(world, area, platformX, platformY, platformWidth, platformHeight);

		for (size_t i = 0u; i < numSpikesOnPlatform; i++) {
			AddSpike(world, area, platformX + spikeWidth / 2.f + i * spikeWidth, platformY, Direction::UP);
		}

		auto spikesOnGroundX = platformX + platformWidth * 2;
		size_t numSpikesOnGround = 5u;

		for (size_t i = 0u; i < numSpikesOnGround; i++) {
			AddSpike(world, area, spikesOnGroundX + spikeWidth / 2.f + i * spikeWidth, area.height, Direction::UP);
		}
	});*/

	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto spikeHeight = GetSpikeHeight(area);

		auto platform1Width = 4 * spikeWidth;
		auto platform1Height = spikeWidth;
		auto platform1X = area.width * 0.25f;
		auto platform1Y = area.height - platform1Height;
		
		AddPlatform(world, area, platform1X, platform1Y, platform1Width, platform1Height);
		AddSpike(world, area, platform1X, platform1Y + platform1Height / 2, Direction::LEFT);

		size_t numSpikesBetweenPlatforms = 3u;
		auto platform2Width = 2.5 * spikeWidth;
		auto platform2Height = 3 * spikeHeight;
		auto platform2X = platform1X + platform1Width + spikeWidth * numSpikesBetweenPlatforms;
		auto platform2Y = area.height - platform2Height;

		AddPlatform(world, area, platform2X, platform2Y, platform2Width, platform2Height);
		
		for (size_t i = 0u; i < numSpikesBetweenPlatforms; i++) {
			AddSpike(world, area, platform2X - spikeWidth / 2 - i * spikeWidth, area.height, Direction::UP);
		}
		AddSpike(world, area, platform2X + platform2Width - spikeWidth / 2, platform2Y, Direction::UP);

		size_t numSpikesAfterPlatform2 = 2u;

		for (size_t i = 0u; i < numSpikesAfterPlatform2; i++) {
			AddSpike(world, area, platform2X + platform2Width  + spikeWidth / 2 + i * spikeWidth, area.height, Direction::UP);
		}
	});
}

void WorldSetCreator::CreateRandomSet(World& world, const sf::FloatRect& setArea) const
{
	if (_setCreators.size() == 0u) {
		throw std::runtime_error("No set available");
	}
	_setCreators[static_cast<size_t>(rand()) % _setCreators.size()](world, setArea);
}

const BeatUnit& WorldSetCreator::GetRandomBeatUnit(World& world) const
{
	auto& manager = world.GetBeatUnitManager();
	return manager.GetUnit(static_cast<size_t>(rand()) % manager.GetNumberOfBeatUnits());
}

void WorldSetCreator::AddSpike(World& world, const sf::FloatRect& area, float xRelative, float yRelative, Direction direction) const
{
	world.AddObstacle(std::make_unique<Spike>(xRelative + area.left, yRelative + area.top, GetSpikeWidth(area), GetSpikeHeight(area), direction, GetRandomBeatUnit(world)));
}

void WorldSetCreator::AddPlatform(World& world, const sf::FloatRect& area, float xRelative, float yRelative, float width, float height) const
{
	world.AddObstacle(std::make_unique<Platform>(xRelative + area.left, yRelative + area.top, width, height));
}

}