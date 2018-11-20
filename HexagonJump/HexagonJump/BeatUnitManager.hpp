#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include "BeatUnit.hpp"

namespace hexagon {

class BeatUnitManager {
public:

	BeatUnitManager(size_t numBeatUnits)
		: _beatUnits(numBeatUnits) {}

	size_t NumberOfBeatUnits() const { return _beatUnits.size(); }

	BeatUnit& GetUnit(size_t index) { return _beatUnits[index]; }
	const BeatUnit& GetUnit(size_t index) const { return _beatUnits[index]; }

	void ShuffleUnits() { 
		auto seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(_beatUnits.begin(), _beatUnits.end(), std::default_random_engine(seed));
	}

	void UpdateUnits(float deltaTime) {
		for (auto& unit : _beatUnits) {
			unit.Update(deltaTime);
		}
	}

private:

	std::vector<BeatUnit> _beatUnits;
};

}