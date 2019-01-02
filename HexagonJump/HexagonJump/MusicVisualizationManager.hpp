#pragma once

#include "MusicVisualization.hpp"

#include <string>
#include <SFML/Audio.hpp>

namespace hexagon {

struct MusicStats {
	size_t bestScore;
	float bestTime;
};

struct MusicData {
	std::string name;
	MusicStats stats;
	MusicVisualization visulization;
};

class MusicVisulizationManager {
public:

	using MusicContainer = std::map<std::string, MusicStats>;

	MusicVisulizationManager(size_t spectrumColumns);

	const MusicContainer& GetMusic() const { return _music; }

	std::string AddNewMusic(const std::string& path, float gameTimerate);
	MusicData LoadMusic(const std::string& musicName) const;
	void UpdateStatsIfBetter(const std::string& musicName, const MusicStats& stats);

private:

	static constexpr auto MUSIC_LIST_FILENAME = "musiclist.txt";
	static constexpr auto STATS_FILE_SUFFIX = ".stats";
	static constexpr auto DATA_FILE_SUFFIX = ".data";

	static MusicVisualization LoadMusicVisualizationFromFile(const std::string& filename, size_t spectrumColumns);
	static void SaveMusicVisualizationToFile(const MusicVisualization& visualization, const std::string& filename);
	static MusicStats LoadMusicStats(const std::string& filename);
	static void SaveMusicStats(const MusicStats& stats, const std::string& filename);

	std::string _applicationDataPath;
	const size_t _spectrumColumns;
	MusicContainer _music;
};

}