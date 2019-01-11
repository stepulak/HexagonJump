#include "BeatUnitManager.hpp" 

#include <algorithm>
#include <chrono>
#include <random>

namespace hexagon {

BeatUnitManager::BeatUnitManager(MusicVisualization&& visualization, size_t numBeatUnits, float gameTimerate)
	: _beatUnits(numBeatUnits > 0 ? numBeatUnits : throw std::runtime_error("Number of beat units cannot be zero"))
	, _musicVisualization(std::move(visualization))
	, _gameTimerate(gameTimerate)
{
	Reset();
}

const BeatUnit & BeatUnitManager::GetRandomUnit() const 
{
	auto max = static_cast<int>(_beatUnits.size() - 1u);
	return _beatUnits[static_cast<size_t>(Random(0, max))];
}

float BeatUnitManager::CurrentHighestBeatRatio() const
{
	static constexpr auto minMaxDiff = (BeatUnit::MAX_HEIGHT - BeatUnit::MIN_HEIGHT);
	return (CurrentHighestBeat() - BeatUnit::MIN_HEIGHT) / minMaxDiff;
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

void BeatUnitManager::Reset()
{
	_visualizationDataIndex = 0u;
	_beatTimer = 0.f;
	_shuffleTimer = 0.f;
	_lastHighestBeat = 0.f;
}

float BeatUnitManager::CurrentHighestBeat() const
{
	return std::max_element(_beatUnits.begin(), _beatUnits.end(),
		[](const auto& unit1, const auto& unit2) {
		return unit1.Height() < unit2.Height();
	})->Height();
}

void BeatUnitManager::ShuffleUnits()
{
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	auto engine = std::default_random_engine(static_cast<unsigned int>(seed));
	std::shuffle(_beatUnits.begin(), _beatUnits.end(), std::move(engine));
}

void BeatUnitManager::SetNewHeights()
{
	if (_visualizationDataIndex >= _musicVisualization.data.size()) {
		return;
	}
	const auto sampleAvg = _musicVisualization.averageSampleValue;
	const auto& data = _musicVisualization.data[_visualizationDataIndex];

	for (size_t i = 0u; i < data.size(); i++) {
		auto pow = std::pow(data[i] * VISUALIZATION_DATA_RATIO, VISUALIZATION_DATA_POW);
		auto height = static_cast<float>(pow * VISUALIZATION_DATA_POW_RATIO / sampleAvg);
		_beatUnits[i].SetHeight(height);
	}
}

void BeatUnitManager::UpdateBeat(float deltaTime)
{
	_beatTimer += deltaTime;

	if (_beatTimer < _gameTimerate || _visualizationDataIndex >= _musicVisualization.data.size()) {
		return;
	}
	while (_beatTimer >= _gameTimerate) {
		_beatTimer -= _gameTimerate;
		_visualizationDataIndex++;
	}
}

void BeatUnitManager::UpdateShuffle(float deltaTime)
{
	auto highestBeat = CurrentHighestBeat();
	_shuffleTimer += deltaTime;

	if (_shuffleTimer > SHUFFLE_TIME_DEFAULT || 
		highestBeat > _lastHighestBeat * PASS_HIGHEST_BEAT_RATIO) {
		_shuffleTimer = 0.f;
		ShuffleUnits();
	}
	SetNewHeights();
	_lastHighestBeat = highestBeat;
}

}