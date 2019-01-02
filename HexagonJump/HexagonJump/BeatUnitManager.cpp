#include "BeatUnitManager.hpp" 

#include <algorithm>
#include <chrono>
#include <random>
#include <iostream>

namespace hexagon {

BeatUnitManager::BeatUnitManager(MusicVisualizationData data, double averageSampleValue, float gameTimerate)
	: _beatUnits(data.size() > 0 ? data.front().size() : throw std::runtime_error("Visualization data is empty"))
	, _visualizationData(std::move(data))
	, _averageSampleValue(averageSampleValue)
	, _gameTimerate(gameTimerate)
{
}

void BeatUnitManager::Update(float deltaTime)
{
	if (_active) {
		UpdateBeat(deltaTime);
		UpdateShuffle(deltaTime);
	}
	for (auto& unit : _beatUnits) {
		unit.Update(deltaTime);
	}
}

float BeatUnitManager::CurrentHighestBeatRatio() const
{
	return (CurrentHighestBeat() - BeatUnit::MIN_HEIGHT) / (BeatUnit::MAX_HEIGHT - BeatUnit::MIN_HEIGHT);
}

void BeatUnitManager::SyncTimingWithMusic(float musicTime)
{
	_visualizationDataIndex = static_cast<size_t>(musicTime / 1000.f / _gameTimerate);
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
	if (_visualizationDataIndex >= _visualizationData.size()) {
		return;
	}
	const auto& data = _visualizationData[_visualizationDataIndex];
	for (size_t i = 0u; i < data.size(); i++) {
		_beatUnits[i].SetHeight(std::pow(data[i], VISUALIZATION_DATA_POW) * VISUALIZATION_DATA_RATIO / _averageSampleValue);
	}
}

void BeatUnitManager::UpdateBeat(float deltaTime)
{
	_beatTimer += deltaTime;
	
	if (_beatTimer < _gameTimerate || _visualizationDataIndex >= _visualizationData.size()) {
		return;
	}
	while (_beatTimer >= _gameTimerate) {
		_beatTimer -= _gameTimerate;
		_visualizationDataIndex++;
	}
}

void BeatUnitManager::UpdateShuffle(float deltaTime)
{
	_shuffleTimer += deltaTime;
	auto highestBeat = CurrentHighestBeat();

	if (_shuffleTimer > SHUFFLE_TIME_DEFAULT || highestBeat > _lastHighestBeat * PASS_HIGHEST_BEAT_RATIO) {
		_shuffleTimer = 0.f;
		ShuffleUnits();
	}
	SetNewHeights();
	_lastHighestBeat = highestBeat;
}

}