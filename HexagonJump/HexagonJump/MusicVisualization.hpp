#pragma once

#include "ThreadSafeProgressBar.hpp"

#include <vector>
#include <SFML/Audio.hpp>

namespace hexagon {

using MusicVisualizationColumnData = std::vector<double>;
using MusicVisualizationData = std::vector<MusicVisualizationColumnData>;

struct MusicVisualization {
	MusicVisualizationData data;
	double averageSampleValue;
};

MusicVisualization CountMusicVisualizationData(
	const sf::SoundBuffer& buffer,
	float gameTimerate, 
	uint8_t numSpectrumColumns,
	gui::ThreadSafeProgressBar& progressBar);

}