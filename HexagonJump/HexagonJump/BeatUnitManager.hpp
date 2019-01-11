#pragma once

#include "BeatUnit.hpp"
#include "MusicVisualization.hpp"
#include "Utils.hpp"

namespace hexagon {

class BeatUnitManager {
public:

	BeatUnitManager(MusicVisualization&& visualization, size_t numBeatUnits, float gameTimerate);

	BeatUnit& GetUnit(size_t index) { return _beatUnits[index]; }
	const BeatUnit& GetUnit(size_t index) const { return _beatUnits[index]; }

	size_t GetNumberOfBeatUnits() const { return _beatUnits.size(); }
	size_t GetVisualizationDataIndex() const { return _visualizationDataIndex; }

	void SyncTimingWithMusic(float musicTime) {
		_visualizationDataIndex = static_cast<size_t>(musicTime / _gameTimerate);
	}

	void Start() { _active = true; }
	void Stop() { _active = false; }

	const BeatUnit& GetRandomUnit() const;
	float CurrentHighestBeatRatio() const;
	void Update(float deltaTime);
	void Reset();

private:

	using BeatUnitContainer = std::vector<BeatUnit>;

	static constexpr float SHUFFLE_TIME_DEFAULT = 2.f;
	static constexpr float PASS_HIGHEST_BEAT_RATIO = 1.5f;
	static constexpr double VISUALIZATION_DATA_POW = 1.6;
	static constexpr double VISUALIZATION_DATA_RATIO = 10;
	static constexpr double VISUALIZATION_DATA_POW_RATIO = 0.000005;

	float CurrentHighestBeat() const;
	void ShuffleUnits();
	void SetNewHeights();
	void UpdateBeat(float deltaTime);
	void UpdateShuffle(float deltaTime);

	BeatUnitContainer _beatUnits;
	MusicVisualization _musicVisualization;
	const float _gameTimerate;
	bool _active = false;
	size_t _visualizationDataIndex;
	float _beatTimer;
	float _shuffleTimer;
	float _lastHighestBeat;
};

}