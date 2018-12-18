#pragma once

#include <vector>
#include <SFML/Audio.hpp>

namespace hexagon {

using MusicVisualizationColumnData = std::vector<double>;
using MusicVisualizationData = std::vector<MusicVisualizationColumnData>;

std::pair<MusicVisualizationData, double> CountMusicVisualizationData(const sf::SoundBuffer& buffer, float gameTimerate, uint8_t spectrumColumns);

}