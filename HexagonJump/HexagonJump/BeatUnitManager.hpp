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

	const BeatUnit& GetRandomUnit() const { return _beatUnits[Random(0, _beatUnits.size() - 1)]; }

	size_t GetNumberOfBeatUnits() const { return _beatUnits.size(); }
	size_t GetVisualizationDataIndex() const { return _visualizationDataIndex; }

	void Start() { _active = true; }
	void Stop() { _active = false; }

	void Update(float deltaTime);
	float CurrentHighestBeatRatio() const;
	void SyncTimingWithMusic(float musicTime);

private:

	using BeatUnitContainer = std::vector<BeatUnit>;

	static constexpr float SHUFFLE_TIME_DEFAULT = 2.f;
	static constexpr float PASS_HIGHEST_BEAT_RATIO = 1.5f;
	static constexpr double VISUALIZATION_DATA_POW = 1.5;
	static constexpr double VISUALIZATION_DATA_RATIO = 0.00015;

	float CurrentHighestBeat() const;
	void ShuffleUnits();
	void SetNewHeights();
	void UpdateBeat(float deltaTime);
	void UpdateShuffle(float deltaTime);

	BeatUnitContainer _beatUnits;
	MusicVisualization _musicVisualization;
	const float _gameTimerate;
	size_t _visualizationDataIndex = 0u;
	bool _active = false;
	float _beatTimer = 0.f;
	float _shuffleTimer = 0.f;
	float _lastHighestBeat = 0.f;
};

}