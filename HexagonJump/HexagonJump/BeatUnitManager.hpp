#pragma once

#include "BeatUnit.hpp"

namespace hexagon {

class BeatUnitManager {
public:

	BeatUnitManager(size_t numBeatUnits);

	void ShufflePositions();

	BeatUnit& GetUnit(size_t index);
	const BeatUnit& GetUnit(size_t index) const;

private:

};

}