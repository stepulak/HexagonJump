#pragma once

#include <vector>
#include <string>
#include <SFML/Audio.hpp>

namespace hexagon {

class MusicVisulizationManager {
public:

	using MusicNamesContainer = std::vector<std::string>;
	using MusicVisualizationColumnData = std::vector<double>;
	using MusicVisualizationData = std::vector<MusicVisualizationColumnData>;

	struct MusicVisualization {
		MusicVisualizationData data;
		double averageSampleValue;
	};

	struct MusicData {
		std::string name;
		size_t bestScore;
		size_t bestTime;
		MusicVisualization visulization;
	};

	MusicVisulizationManager();

	const MusicNamesContainer& GetMusicNames() const { _musicNames; }
	std::string AddMusic(const std::string& path);
	MusicData OpenMusic(const std::string& musicName) const;
	void UpdateStatsIfBetter(const std::string& musicName, size_t bestScore, size_t bestTime);

private:

	static MusicVisualization CountMusicVisualizationData(const sf::SoundBuffer& buffer, float gameTimerate, uint8_t spectrumColumns);

	MusicNamesContainer _musicNames;
};

}