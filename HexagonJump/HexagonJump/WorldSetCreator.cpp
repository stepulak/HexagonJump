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
		size_t numSpikeGroups = 2u;
		size_t numSpikesPerGroup = 3u;
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
		size_t numSpikesOnPlatform = 4u;
		auto platformWidth = numSpikesOnPlatform * spikeWidth;
		auto platformHeight = spikeHeight;
		auto platformX = area.width * 0.2f;
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

	/*_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto spikeHeight = GetSpikeHeight(area);

		auto platform1Width = 4 * spikeWidth;
		auto platform1Height = spikeWidth;
		auto platform1X = area.width * 0.25f;
		auto platform1Y = area.height - platform1Height;
		
		AddPlatform(world, area, platform1X, platform1Y, platform1Width, platform1Height);
		AddSpike(world, area, platform1X, platform1Y + platform1Height / 2, Direction::LEFT);

		size_t numSpikesBetweenPlatforms = 3u;
		auto platform2Width = 3 * spikeWidth;
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
	});*/

	/*_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto squareOffset = area.width * 0.15f;
		auto squareHeight = area.height * 0.5f;
		AddSquareWithSpikes(world, area, area.width / 2.f - squareOffset, squareHeight);
		AddSquareWithSpikes(world, area, area.width / 2.f + squareOffset, squareHeight);
		
		float spikeWidth = GetSpikeWidth(area);
		float spikesX = area.width * 0.25f;
		float spikesOffset = area.width / 2.f - spikesX;
		size_t numSpikesGroup = 3u;

		for (size_t i = 0u; i < numSpikesGroup; i++) {
			AddSpike(world, area, spikesX + spikesOffset * i - spikeWidth / 2.f, area.height, Direction::UP);
			AddSpike(world, area, spikesX + spikesOffset * i + spikeWidth / 2.f, area.height, Direction::UP);
		}
	});*/

	/*_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto squareOffset = area.width * 0.25f;
		auto square1Height = area.height - spikeWidth;
		auto square2Height = area.height * 0.4f;
		AddSquareWithSpikes(world, area, area.width / 2.f - squareOffset, square1Height, true);
		AddSquareWithSpikes(world, area, area.width / 2.f + squareOffset, square1Height, true);
		AddSquareWithSpikes(world, area, area.width / 2.f - spikeWidth / 2.f, square2Height);

		size_t numSpikes = 3u;
		auto groupWidth = spikeWidth * numSpikes;

		for (size_t i = 0u; i < numSpikes; i++) {
			AddSpike(world, area, area.width / 2.f - groupWidth / 2.f + spikeWidth * i + spikeWidth / 2.f, area.height, Direction::UP);
		}
	});*/

	/*_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto spikeHeight = GetSpikeHeight(area);
		auto platformX = area.width / 2.f - area.width * 0.35f;
		auto platformWidth = spikeHeight * 3.f;
		auto platformHeightBase = spikeWidth;
		size_t numPlatforms = 3u;

		for (size_t i = 0u; i < numPlatforms; i++) {
			auto height = (i + 1) * platformHeightBase;
			AddPlatform(world, area, platformX + i * platformWidth, area.height - height, platformWidth, height);
			AddSpike(world, area, platformX + i * platformWidth, area.height - height + spikeWidth / 2.f, Direction::LEFT);
		}
		auto spikesX = area.width / 2.f + area.width * 0.18f;
		size_t numSpikes = 3u;

		for (size_t i = 0; i < numSpikes; i++) {
			AddSpike(world, area, spikesX + i * spikeWidth + spikeWidth / 2.f, area.height, Direction::UP);
		}
	});*/

	/*_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto spikeHeight = GetSpikeHeight(area);
		auto spikesX = area.width / 2.f - area.width * 0.25f;
		size_t numSpikes = 15u;

		for (size_t i = 0u; i < numSpikes; i++) {
			AddSpike(world, area, spikesX + i * spikeWidth + spikeWidth / 2.f, area.height, Direction::UP);
		}

		auto platformWidth = spikeWidth * 1.5f;
		auto platformHeight = platformWidth * 0.5f;
		auto platformY = area.height - platformHeight - spikeHeight * 2u;
		auto platformOffset = area.width * 0.1f;

		AddPlatform(world, area, area.width / 2.f - platformOffset, platformY, platformWidth, platformHeight);
		AddPlatform(world, area, area.width / 2.f + platformOffset, platformY, platformWidth, platformHeight);
	});*/

	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikeWidth = GetSpikeWidth(area);
		auto spikeHeight = GetSpikeHeight(area);
		auto platformWidth = spikeWidth;
		auto platformHeight = spikeHeight * 2u;
		auto platformX = area.width / 2.f - platformWidth / 2.f;
		size_t numSpikesAtOneSide = 4u;

		AddPlatform(world, area, area.width / 2.f - platformWidth / 2.f, area.height - platformHeight, platformWidth, platformHeight);

		for (size_t i = 0u; i < numSpikesAtOneSide; i++) {
			AddSpike(world, area, platformX - i * spikeWidth - spikeWidth / 2.f, area.height, Direction::UP);
			AddSpike(world, area, platformX + platformWidth + i * spikeWidth + spikeWidth / 2.f, area.height, Direction::UP);
		}

		AddSquareWithSpikes(world, area, platformX, area.height * 0.3f);
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
	return world.GetBeatUnitManager().GetRandomUnit();
}

void WorldSetCreator::AddSpike(World& world, const sf::FloatRect& area, float xRelative, float yRelative, Direction direction) const
{
	world.GetObstacleManager()
		.GetObstaclePool()
		.Add(std::make_unique<Spike>(xRelative + area.left, yRelative + area.top, GetSpikeWidth(area), GetSpikeHeight(area), direction, GetRandomBeatUnit(world)));
}

void WorldSetCreator::AddPlatform(World& world, const sf::FloatRect& area, float xRelative, float yRelative, float width, float height) const
{
	world.GetObstacleManager()
		.GetObstaclePool()
		.Add(std::make_unique<Platform>(xRelative + area.left, yRelative + area.top, width, height));
}

void WorldSetCreator::AddSquareWithSpikes(World& world, const sf::FloatRect& area, float xRelative, float yRelative, bool skipBottom) const
{
	auto size = GetSpikeWidth(area);
	AddPlatform(world, area, xRelative, yRelative, size, size);
	AddSpike(world, area, xRelative + size / 2.f, yRelative, Direction::UP);
	AddSpike(world, area, xRelative, yRelative + size / 2.f, Direction::LEFT);
	AddSpike(world, area, xRelative + size, yRelative + size / 2.f, Direction::RIGHT);
	if (!skipBottom) {
		AddSpike(world, area, xRelative + size / 2.f, yRelative + size, Direction::DOWN);
	}
}

}