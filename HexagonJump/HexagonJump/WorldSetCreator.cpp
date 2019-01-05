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
		GetSpikeWidth(), 
		GetSpikeHeight(),
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
	auto size = GetSpikeWidth();

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
		size_t numSpikeGroups = 2u;
		size_t numSpikesPerGroup = 3u;
		auto groupOffset = (area.width - numSpikeGroups * numSpikesPerGroup * GetSpikeWidth()) / (1 + numSpikeGroups);

		for (size_t i = 0u; i < numSpikeGroups; i++) {
			for (size_t j = 0u; j < numSpikesPerGroup; j++) {
				AddSpike(world, 
					area, 
					groupOffset + i * groupOffset + j * GetSpikeWidth() + GetSpikeWidth() / 2,
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
		auto platformWidth = numSpikesOnPlatform * GetSpikeWidth();
		auto platformHeight = GetSpikeHeight();
		auto platformX = area.width * 0.2f;
		auto platformY = area.height - platformHeight;

		AddPlatform(world, area, platformX, platformY, platformWidth, platformHeight);

		for (size_t i = 0u; i < numSpikesOnPlatform; i++) {
			AddSpike(world, 
				area, 
				platformX + GetSpikeWidth() / 2.f + i * GetSpikeWidth(), 
				platformY, 
				Direction::UP);
		}

		auto spikesOnGroundX = platformX + platformWidth * 2;
		size_t numSpikesOnGround = 5u;

		for (size_t i = 0u; i < 5u; i++) {
			AddSpike(world,
				area,
				spikesOnGroundX + GetSpikeWidth() / 2.f + i * GetSpikeWidth(),
				area.height,
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet3()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platform1Width = 4 * GetSpikeWidth();
		auto platform1Height = GetSpikeWidth();
		auto platform1X = area.width * 0.25f;
		auto platform1Y = area.height - platform1Height;

		AddPlatform(world, area, platform1X, platform1Y, platform1Width, platform1Height);
		AddSpike(world, area, platform1X, platform1Y + platform1Height / 2, Direction::LEFT);

		size_t numSpikesBetweenPlatforms = 3u;
		auto platform2Width = 3 * GetSpikeWidth();
		auto platform2Height = 3 * GetSpikeHeight();
		auto platform2X = platform1X + platform1Width + GetSpikeWidth() * numSpikesBetweenPlatforms;
		auto platform2Y = area.height - platform2Height;

		AddPlatform(world, area, platform2X, platform2Y, platform2Width, platform2Height);

		for (size_t i = 0u; i < numSpikesBetweenPlatforms; i++) {
			AddSpike(world, 
				area, 
				platform2X - GetSpikeWidth() / 2 - i * GetSpikeWidth(), 
				area.height,
				Direction::UP);
		}
		AddSpike(world, area, platform2X + platform2Width - GetSpikeWidth() / 2, platform2Y, Direction::UP);

		size_t numSpikesAfterPlatform2 = 2u;

		for (size_t i = 0u; i < numSpikesAfterPlatform2; i++) {
			AddSpike(world, 
				area,
				platform2X + platform2Width + GetSpikeWidth() / 2 + i * GetSpikeWidth(),
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
				spikesX + spikesOffset * i - GetSpikeWidth() / 2.f, 
				area.height, 
				Direction::UP);

			AddSpike(world,
				area, 
				spikesX + spikesOffset * i + GetSpikeWidth() / 2.f, 
				area.height, 
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet5()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto squareOffset = area.width * 0.25f;
		auto square1Height = area.height - GetSpikeWidth();
		auto square2Height = area.height * 0.4f;
		AddSquareWithSpikes(world, area, area.width / 2.f - squareOffset, square1Height, true);
		AddSquareWithSpikes(world, area, area.width / 2.f + squareOffset, square1Height, true);
		AddSquareWithSpikes(world, area, area.width / 2.f - GetSpikeWidth() / 2.f, square2Height);

		size_t numSpikes = 3u;
		auto groupWidth = GetSpikeWidth() * numSpikes;

		for (size_t i = 0u; i < numSpikes; i++) {
			AddSpike(world, 
				area, 
				area.width / 2.f - groupWidth / 2.f + GetSpikeWidth() * i + GetSpikeWidth() / 2.f,
				area.height, 
				Direction::UP);
		}
	});
}

void WorldSetCreator::InitSet6()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platformX = area.width / 2.f - area.width * 0.35f;
		auto platformWidth = GetSpikeHeight() * 3.f;
		auto platformHeightBase = GetSpikeWidth();
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
				area.height - height + GetSpikeWidth() / 2.f,
				Direction::LEFT);
		}
		auto spikesX = area.width / 2.f + area.width * 0.18f;
		size_t numSpikes = 3u;

		for (size_t i = 0; i < numSpikes; i++) {
			AddSpike(world, area, spikesX + i * GetSpikeWidth() + GetSpikeWidth() / 2.f, area.height, Direction::UP);
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
				spikesX + i * GetSpikeWidth() + GetSpikeWidth() / 2.f, 
				area.height, 
				Direction::UP);
		}

		auto platformWidth = GetSpikeWidth() * 1.5f;
		auto platformHeight = platformWidth * 0.5f;
		auto platformY = area.height - platformHeight - GetSpikeHeight() * 2u;
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
		auto platformWidth = GetSpikeWidth();
		auto platformHeight = GetSpikeHeight() * 2u;
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
				platformX - i * GetSpikeWidth() - GetSpikeWidth() / 2.f,
				area.height, 
				Direction::UP);

			AddSpike(world, 
				area, 
				platformX + platformWidth + i * GetSpikeWidth() + GetSpikeWidth() / 2.f,
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
			auto y = area.height - GetSpikeHeight();

			AddPlatform(world, area, x, y, GetSpikeWidth(), GetSpikeHeight());
			AddSpike(world, area, x + GetSpikeWidth() / 2.f, y, Direction::UP);
			AddSquareWithSpikes(world, area, x, squareHeight);
		}
	});
}

void WorldSetCreator::InitSet10()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto offset = area.width * 0.2f;
		auto x = area.width / 2.f - offset * 2u;

		AddSquareWithSpikes(world, area, x, area.height - GetSpikeWidth(), true);
		AddSquareWithSpikes(world, area, x + 2u * offset, area.height * 0.35f);

		for (size_t i = 0u; i < 2u; i++) {
			for (size_t j = 0u; j < 2u; j++) {
				AddSpike(world, area, x + (i + 1) * offset + j * GetSpikeWidth(), area.height, Direction::UP);
			}
		}
	});
}

void WorldSetCreator::InitSet11()
{
	_setCreators.emplace_back([&](auto& world, const auto& area) {
		auto platformWidth = GetSpikeWidth() * 6u;
		auto platformHeight = GetSpikeWidth() * 2u;
		auto platformX = area.width / 2.f - platformWidth / 2.f;
		auto platformY = area.height - platformHeight;

		AddPlatform(world, area, platformX, platformY, platformWidth, platformHeight);

		size_t numSpikesAroundPlatform = 2u;

		for (size_t i = 0u; i < 2u; i++) {
			for (size_t j = 0u; j < numSpikesAroundPlatform; j++) {
				auto x = platformX - GetSpikeWidth() / 2.f - j * GetSpikeWidth() +
					i * (platformWidth + numSpikesAroundPlatform * GetSpikeWidth());

				AddSpike(world, area, x, area.height, Direction::UP);
			}

			AddSpike(world,
				area,
				platformX + platformWidth - GetSpikeWidth() / 2.f - i * GetSpikeWidth(), 
				platformY, 
				Direction::UP);
		}

		AddSquareWithSpikes(world, area, platformX, area.height * 0.35f);
	});
}

}