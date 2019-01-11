#pragma once

#include "MusicVisualization.hpp"
#include "ThreadSafeProgressBar.hpp"

#include <future>
#include <optional>
#include <string>
#include <unordered_set>
#include <SFML/Audio.hpp>

namespace hexagon {

struct MusicData {
	std::string name;
	std::string path;
	MusicVisualization visualization;
};

class MusicVisulizationManager {
public:

	using MusicContainer = std::unordered_set<std::string>;
	using MaybeString = std::optional<std::string>;
	using ConvertResult = std::future<MaybeString>;

	MusicVisulizationManager(size_t spectrumColumns);

	const MusicContainer& GetMusicList() const { return _musicList; }

	ConvertResult ConvertMusicAsync(const std::string& path,
		float gameTimerate,
		gui::ThreadSafeProgressBar& progressBar) const;

	MusicData OpenMusic(const std::string& musicName) const;

private:

	static constexpr auto DATA_FILE_SUFFIX = ".data";

	void LoadExistingMusicList();

	static MusicVisualization LoadMusicVisualizationFromFile(const std::string& filename,
		size_t spectrumColumns);

	static void SaveMusicVisualizationToFile(const MusicVisualization& visualization,
		const std::string& filename);
	
	const size_t _spectrumColumns;
	std::string _applicationDataPath;
	MusicContainer _musicList;
};

}