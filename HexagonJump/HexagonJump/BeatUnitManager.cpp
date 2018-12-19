#include "BeatUnitManager.hpp" 

#include <algorithm>
#include <chrono>
#include <random>

namespace hexagon {

BeatUnitManager::BeatUnitManager(MusicVisualizationData data, double averageSampleValue)
	: _beatUnits(data.front().size())
	, _visualizationData(std::move(data))
	, _averageSampleValue(averageSampleValue)
{
}

void BeatUnitManager::Update(float deltaTime, float gameTimerate)
{
	if (_active) {
		UpdateBeatAndShuffle(deltaTime, gameTimerate);
	}
	for (auto& unit : _beatUnits) {
		unit.Update(deltaTime);
	}
}

float BeatUnitManager::CurrentHighestBeat() const
{
	float maxHeight = 0.f;
	for (const auto& unit : _beatUnits) {
		if (unit.Height() >= maxHeight) {
			maxHeight = unit.Height();
		}
	}
	return maxHeight;
}

void BeatUnitManager::ShuffleUnits()
{
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(_beatUnits.begin(), _beatUnits.end(), std::default_random_engine(seed));
}

void BeatUnitManager::SetNewHeights()
{
	const auto& data = _visualizationData[_visualizationDataIndex];
	for (size_t i = 0u; i < data.size(); i++) {
		_beatUnits[i].SetHeight(std::pow(data[i], 1.5) * 20 / _averageSampleValue);
	}
}

void BeatUnitManager::UpdateBeatAndShuffle(float deltaTime, float gameTimerate) {
	_beatTimer += deltaTime;
	_shuffleTimer += deltaTime;

	if (_beatTimer >= gameTimerate) {
		_beatTimer -= gameTimerate;
		_visualizationDataIndex++;

		auto highestBeat = CurrentHighestBeat();

		if (_shuffleTimer > SHUFFLE_TIME_DEFAULT || highestBeat > _lastHighestBeat * PASS_HIGHEST_BEAT_RATIO) {
			_shuffleTimer = 0.f;
			ShuffleUnits();
		}

		SetNewHeights();
		_lastHighestBeat = highestBeat;
	}
}

}