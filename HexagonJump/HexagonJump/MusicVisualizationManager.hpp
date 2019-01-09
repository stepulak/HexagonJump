#pragma once

#include "MusicVisualization.hpp"
#include "ThreadSafeProgressBar.hpp"

#include <future>
#include <map>
#include <string>
#include <SFML/Audio.hpp>

namespace hexagon {

struct MusicData {
	std::string name;
	std::string path;
	unsigned bestScore;
	MusicVisualization visualization;
};

class MusicVisulizationManager {
public:

	using MusicContainer = std::map<std::string, unsigned>;
	using ConvertResult = std::future<std::string>;

	MusicVisulizationManager(size_t spectrumColumns);

	const MusicContainer& GetMusicList() const { return _music; }

	ConvertResult ConvertNewMusicAsync(const std::string& path,
		float gameTimerate, 
		gui::ThreadSafeProgressBar& progressBar);

	MusicData LoadMusic(const std::string& musicName) const;
	void UpdateScoreIfBetter(const std::string& musicName, unsigned score);

private:

	static constexpr auto SCORE_FILE_SUFFIX = ".score";
	static constexpr auto DATA_FILE_SUFFIX = ".data";

	void LoadExistingMusicList();

	static MusicVisualization LoadMusicVisualizationFromFile(const std::string& filename, size_t spectrumColumns);
	static void SaveMusicVisualizationToFile(const MusicVisualization& visualization, const std::string& filename);
	static unsigned LoadMusicScore(const std::string& filename);
	static void SaveMusicScore(unsigned score, const std::string& filename);

	const size_t _spectrumColumns;
	std::string _applicationDataPath;
	MusicContainer _music;
};

}