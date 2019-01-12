#include "WorldSetCreator.hpp"
#include "Platform.hpp"
#include "Spike.hpp"
#include "World.hpp"

namespace hexagon {

WorldSetCreator::WorldSetCreator()
{
	InitSet1();
	InitSet2();
	InitSet3();
	InitSet4();
	InitSet5();
	InitSet6();
	InitSet7();
	InitSet8();
	InitSet9();
	InitSet10();
	InitSet11();
}

void WorldSetCreator::SpawnRandomSet(World& world, const sf::FloatRect& setArea) const
{
	if (_setCreators.size() == 0u) {
		throw std::runtime_error("No set available");
	}
	_setCreators[static_cast<size_t>(rand()) % _setCreators.size()](world, setArea);
}

void WorldSetCreator::AddSpike(World& world, 
	const sf::FloatRect& area, 
	float xLeftRelative, 
	float yTopRelative, 
	Direction direction)
{
	auto spike = std::make_unique<Spike>(xLeftRelative + area.left,
		yTopRelative + area.top,
		SPIKE_WIDTH, 
		SPIKE_HEIGHT,
		direction, 
		world.GetBeatUnitManager().GetRandomUnit());

	world.GetObstacleManager().GetObstaclePool().Add(std::move(spike));
}

void WorldSetCreator::AddPlatform(World& world, 
	const sf::FloatRect& area, 
	float xLeftRelative,
	float yTopRelative, 
	float width, 
	float height)
{
	auto platform = std::make_unique<Platform>(xLeftRelative + area.left,
		yTopRelative + area.top,
		width, 
		height);

	world.GetObstacleManager().GetObstaclePool().Add(std::move(platform));
}

void WorldSetCreator::AddSquareWithSpikes(World& world, 
	const sf::FloatRect& area, 
	float xLeftRelative, 
	float yTopRelative,
	bool skipBottom)
{
	auto size = SPIKE_WIDTH;

	AddPlatform(world, area, xLeftRelative, yTopRelative, size, size);
	AddSpike(world, area, xLeftRelative + size / 2.f, yTopRelative, Direction::UP);
	AddSpike(world, area, xLeftRelative, yTopRelative + size / 2.f, Direction::LEFT);
	AddSpike(world, area, xLeftRelative + size, yTopRelative + size / 2.f, Direction::RIGHT);

	if (!skipBottom) {
		AddSpike(world, area, xLeftRelative + size / 2.f, yTopRelative + size, Direction::DOWN);
	}
}

void WorldSetCreator::InitSet1()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		size_t numGroups = 2u;
		size_t numSpikesPerGroup = 3u;
		auto offset = (area.width - numGroups * numSpikesPerGroup * SPIKE_WIDTH) / (1 + numGroups);

		for (size_t i = 0u; i < numGroups; i++) {
			for (size_t j = 0u; j < numSpikesPerGroup; j++) {
				AddSpike(world, 
					area, 
					offset + i * offset + j * SPIKE_WIDTH + SPIKE_WIDTH / 2,
					area.height,
					Direction::UP);
			}
		}
	});
}

void WorldSetCreator::InitSet2()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		size_t numSpikesOnPlatform = 4u;
		auto platformWidth = numSpikesOnPlatform * SPIKE_WIDTH;
		auto platformHeight = SPIKE_HEIGHT;
		auto platformX = area.width * 0.2f;
		auto platformY = area.height - platformHeight;

		AddPlatform(world, area, platformX, platformY, platformWidth, platformHeight);

		for (size_t i = 0u; i < numSpikesOnPlatform; i++) {
			AddSpike(world, 
				area, 
				platformX + SPIKE_WIDTH / 2.f + i * SPIKE_WIDTH, 
				platformY, 
				Direction::UP);
		}

		auto spikesOnGroundX = platformX + platformWidth * 2;
		size_t numSpikesOnGround = 5u;

		for (size_t i = 0u; i < 5u; i++) {
			AddSpike(world,
				area,
				spikesOnGroundX + SPIKE_WIDTH / 2.f + i * SPIKE_WIDTH,
				area.height,
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet3()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platform1Width = 4 * SPIKE_WIDTH;
		auto platform1Height = SPIKE_WIDTH;
		auto platform1X = area.width * 0.25f;
		auto platform1Y = area.height - platform1Height;

		AddPlatform(world, area, platform1X, platform1Y, platform1Width, platform1Height);
		AddSpike(world, area, platform1X, platform1Y + platform1Height / 2, Direction::LEFT);

		size_t numSpikesBetweenPlatforms = 3u;
		auto platform2Width = 3 * SPIKE_WIDTH;
		auto platform2Height = 3 * SPIKE_HEIGHT;
		auto platform2X = platform1X + platform1Width + SPIKE_WIDTH * numSpikesBetweenPlatforms;
		auto platform2Y = area.height - platform2Height;

		AddPlatform(world, area, platform2X, platform2Y, platform2Width, platform2Height);

		for (size_t i = 0u; i < numSpikesBetweenPlatforms; i++) {
			AddSpike(world, 
				area, 
				platform2X - SPIKE_WIDTH / 2 - i * SPIKE_WIDTH, 
				area.height,
				Direction::UP);
		}
		AddSpike(world, area, platform2X + platform2Width - SPIKE_WIDTH / 2, platform2Y, Direction::UP);

		size_t numSpikesAfterPlatform2 = 2u;

		for (size_t i = 0u; i < numSpikesAfterPlatform2; i++) {
			AddSpike(world, 
				area,
				platform2X + platform2Width + SPIKE_WIDTH / 2 + i * SPIKE_WIDTH,
				area.height,
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet4()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto squareOffset = area.width * 0.15f;
		auto squareHeight = area.height * 0.5f;

		AddSquareWithSpikes(world, area, area.width / 2.f - squareOffset, squareHeight);
		AddSquareWithSpikes(world, area, area.width / 2.f + squareOffset, squareHeight);

		float spikesX = area.width * 0.25f;
		float spikesOffset = area.width / 2.f - spikesX;
		size_t numSpikesGroup = 3u;

		for (size_t i = 0u; i < numSpikesGroup; i++) {
			AddSpike(world, 
				area, 
				spikesX + spikesOffset * i - SPIKE_WIDTH / 2.f, 
				area.height, 
				Direction::UP);

			AddSpike(world,
				area, 
				spikesX + spikesOffset * i + SPIKE_WIDTH / 2.f, 
				area.height, 
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet5()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto squareOffset = area.width * 0.25f;
		auto square1Height = area.height - SPIKE_WIDTH;
		auto square2Height = area.height * 0.4f;

		AddSquareWithSpikes(world, area, area.width / 2.f - squareOffset, square1Height, true);
		AddSquareWithSpikes(world, area, area.width / 2.f + squareOffset, square1Height, true);
		AddSquareWithSpikes(world, area, area.width / 2.f - SPIKE_WIDTH / 2.f, square2Height);

		size_t numSpikes = 3u;
		auto groupWidth = SPIKE_WIDTH * numSpikes;

		for (size_t i = 0u; i < numSpikes; i++) {
			AddSpike(world, 
				area, 
				area.width / 2.f - groupWidth / 2.f + SPIKE_WIDTH * i + SPIKE_WIDTH / 2.f,
				area.height, 
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet6()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platformX = area.width / 2.f - area.width * 0.35f;
		auto platformWidth = SPIKE_HEIGHT * 3.f;
		auto platformHeightBase = SPIKE_WIDTH;
		size_t numPlatforms = 3u;

		for (size_t i = 0u; i < numPlatforms; i++) {
			auto height = (i + 1) * platformHeightBase;

			AddPlatform(world,
				area, 
				platformX + i * platformWidth, 
				area.height - height,
				platformWidth,
				height);

			AddSpike(world,
				area,
				platformX + i * platformWidth, 
				area.height - height + SPIKE_WIDTH / 2.f,
				Direction::LEFT);
		}
		auto spikesX = area.width / 2.f + area.width * 0.18f;
		size_t numSpikes = 3u;

		for (size_t i = 0; i < numSpikes; i++) {
			AddSpike(world,
				area, 
				spikesX + i * SPIKE_WIDTH + SPIKE_WIDTH / 2.f, 
				area.height, 
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet7()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto spikesX = area.width / 2.f - area.width * 0.25f;
		size_t numSpikes = 15u;

		for (size_t i = 0u; i < numSpikes; i++) {
			AddSpike(world,
				area, 
				spikesX + i * SPIKE_WIDTH + SPIKE_WIDTH / 2.f, 
				area.height, 
				Direction::UP);
		}

		auto platformWidth = SPIKE_WIDTH * 1.5f;
		auto platformHeight = platformWidth * 0.5f;
		auto platformY = area.height - platformHeight - SPIKE_HEIGHT * 1.5f;
		auto platformOffset = area.width * 0.1f;

		AddPlatform(world, 
			area, 
			area.width / 2.f - platformOffset, 
			platformY, 
			platformWidth, 
			platformHeight);

		AddPlatform(world, 
			area, 
			area.width / 2.f + platformOffset,
			platformY, 
			platformWidth, 
			platformHeight);
	});
}

void WorldSetCreator::InitSet8()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platformWidth = SPIKE_WIDTH;
		auto platformHeight = SPIKE_HEIGHT * 2u;
		auto platformX = area.width / 2.f - platformWidth / 2.f;
		size_t numSpikesAtOneSide = 4u;

		AddPlatform(world, 
			area, 
			area.width / 2.f - platformWidth / 2.f,
			area.height - platformHeight, 
			platformWidth, 
			platformHeight);

		for (size_t i = 0u; i < numSpikesAtOneSide; i++) {
			AddSpike(world, 
				area, 
				platformX - i * SPIKE_WIDTH - SPIKE_WIDTH / 2.f,
				area.height, 
				Direction::UP);

			AddSpike(world, 
				area, 
				platformX + platformWidth + i * SPIKE_WIDTH + SPIKE_WIDTH / 2.f,
				area.height,
				Direction::UP);
		}

		AddSquareWithSpikes(world, area, platformX, area.height * 0.3f);
	});
}

void WorldSetCreator::InitSet9()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platformOffset = area.width * 0.25f;
		auto platformX = area.width / 2.f - platformOffset;
		auto squareHeight = area.height * 0.3f;

		for (size_t i = 0u; i < 3u; i++) {
			auto x = platformX + i * platformOffset;
			auto y = area.height - SPIKE_HEIGHT;

			AddPlatform(world, area, x, y, SPIKE_WIDTH, SPIKE_HEIGHT);
			AddSpike(world, area, x + SPIKE_WIDTH / 2.f, y, Direction::UP);
			AddSquareWithSpikes(world, area, x, squareHeight);
		}
	});
}

void WorldSetCreator::InitSet10()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto offset = area.width * 0.2f;
		auto x = area.width / 2.f - offset * 2u;

		AddSquareWithSpikes(world, area, x, area.height - SPIKE_WIDTH, true);
		AddSquareWithSpikes(world, area, x + 2u * offset, area.height * 0.35f);

		for (size_t i = 0u; i < 2u; i++) {
			for (size_t j = 0u; j < 2u; j++) {
				AddSpike(world, area, x + (i + 1) * offset + j * SPIKE_WIDTH, area.height, Direction::UP);
			}
		}
	});
}

void WorldSetCreator::InitSet11()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platformWidth = SPIKE_WIDTH * 6u;
		auto platformHeight = SPIKE_WIDTH * 2u;
		auto platformX = area.width / 2.f - platformWidth / 2.f;
		auto platformY = area.height - platformHeight;

		AddPlatform(world, area, platformX, platformY, platformWidth, platformHeight);

		size_t numSpikesAroundPlatform = 2u;

		for (size_t i = 0u; i < 2u; i++) {
			for (size_t j = 0u; j < numSpikesAroundPlatform; j++) {
				auto x = platformX - SPIKE_WIDTH / 2.f - j * SPIKE_WIDTH +
					i * (platformWidth + numSpikesAroundPlatform * SPIKE_WIDTH);

				AddSpike(world, area, x, area.height, Direction::UP);
			}

			AddSpike(world,
				area,
				platformX + platformWidth - SPIKE_WIDTH / 2.f - i * SPIKE_WIDTH, 
				platformY, 
				Direction::UP);
		}

		AddSquareWithSpikes(world, area, platformX, area.height * 0.35f);
	});
}

}