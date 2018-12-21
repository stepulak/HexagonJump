#pragma once

#include "BeatUnit.hpp"
#include "MusicVisualization.hpp"

namespace hexagon {

class BeatUnitManager {
public:

	BeatUnitManager(MusicVisualizationData data, double averageSampleValue);

	size_t NumberOfBeatUnits() const { return _beatUnits.size(); }

	BeatUnit& GetUnit(size_t index) { return _beatUnits[index]; }
	const BeatUnit& GetUnit(size_t index) const { return _beatUnits[index]; }

	size_t VisualizationDataIndex() const { return _visualizationDataIndex; }

	void Start() { _active = true; }
	void Stop() { _active = false; }

	void Update(float deltaTime, float gameTimerate);

private:

	static constexpr float SHUFFLE_TIME_DEFAULT = 2.f;
	static constexpr float PASS_HIGHEST_BEAT_RATIO = 1.5f;

	float CurrentHighestBeat() const;
	void ShuffleUnits();
	void SetNewHeights();
	void UpdateBeatAndShuffle(float deltaTime, float gameTimerate);

	std::vector<BeatUnit> _beatUnits;
	MusicVisualizationData _visualizationData;
	double _averageSampleValue;
	size_t _visualizationDataIndex = 0u;
	bool _active = false;
	float _beatTimer = 0.f;
	float _shuffleTimer = 0.f;
	float _lastHighestBeat = 0.f;
};

}