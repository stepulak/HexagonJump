#include "MusicVisualizationManager.hpp"
#include "Utils.hpp"
#include "AppInfo.hpp"

#include <fstream>
#include <sstream>

#ifndef _WIN32
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#else
#pragma warning(disable:4996) // use getenv without error
#endif

namespace hexagon {

MusicVisulizationManager::MusicVisulizationManager(size_t spectrumColumns)
	: _spectrumColumns(spectrumColumns)
{
#ifdef _WIN32
	_applicationDataPath = getenv("APPDATA");
#else
	auto homeFolder = getenv("HOME");
	_applicationDataPath = homeFolder != nullptr ? homeFolder : getpwuid(getuid())->pw_dir;
#endif

	_applicationDataPath += std::string("\\") + APP_NAME + "\\";
	std::filesystem::create_directory(_applicationDataPath);
	LoadExistingMusicList();
}

std::string MusicVisulizationManager::ConvertNewMusic(const std::string& path, float gameTimerate)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path)) {
		throw std::runtime_error("Unable to load music: " + path);
	}

	auto musicName = FilenameWithoutExtension(path);
	auto musicPath = _applicationDataPath + musicName;
	auto visualizationData = CountMusicVisualizationData(buffer, gameTimerate, _spectrumColumns);

	SaveMusicVisualizationToFile(visualizationData, musicPath + DATA_FILE_SUFFIX);
	SaveMusicStats({ 0, 0.f }, musicPath + STATS_FILE_SUFFIX);

	return musicName;
}

MusicData MusicVisulizationManager::LoadMusic(const std::string& musicName) const
{
	auto musicPath = _applicationDataPath + musicName;
	auto musicVisualization = LoadMusicVisualizationFromFile(musicPath + DATA_FILE_SUFFIX, _spectrumColumns);
	auto musicStats = LoadMusicStats(musicPath + STATS_FILE_SUFFIX);
	return { musicName, musicStats, musicVisualization };
}

void MusicVisulizationManager::UpdateStatsIfBetter(const std::string& musicName, const MusicStats& stats)
{
	auto statsFilename = _applicationDataPath + musicName + STATS_FILE_SUFFIX;
	auto oldStats = LoadMusicStats(statsFilename);
	MusicStats newStats { std::max(stats.bestScore, oldStats.bestScore), std::max(stats.bestTime, oldStats.bestTime) };
	
	SaveMusicStats(newStats, statsFilename);
	_music[musicName] = newStats;
}

void MusicVisulizationManager::LoadExistingMusicList()
{
	for (const auto& filename : std::filesystem::directory_iterator(_applicationDataPath)) {
		auto path = filename.path().string();
		auto musicName = FilenameWithoutExtension(path);
		if (_music.find(musicName) == _music.end()) {
			_music[musicName] = LoadMusicStats(path);
		}
	}
}

MusicVisualization MusicVisulizationManager::LoadMusicVisualizationFromFile(const std::string& filename, size_t spectrumColumns)
{
	std::fstream file(filename, std::ios_base::in);
	if (!file.good()) {
		throw std::runtime_error("Unable to open music visualization data file: " + filename);
	}

	double averageSampleValue;
	file >> averageSampleValue;

	size_t dataSize;
	file >> dataSize;

	MusicVisualizationData visualizationData;
	visualizationData.reserve(dataSize);

	for (size_t i = 0u; i < dataSize; i++) {
		std::string line;
		std::getline(file, line);
		std::istringstream stream(line);
		MusicVisualizationColumnData columnData(spectrumColumns);
		char sep;

		for (size_t j = 0u; j < spectrumColumns; j++) {
			stream >> columnData[j] >> sep;
		}
		visualizationData.emplace_back(std::move(columnData));
	}

	return { visualizationData, averageSampleValue };
}

void MusicVisulizationManager::SaveMusicVisualizationToFile(const MusicVisualization& visualization, const std::string& filename)
{
	std::fstream file(filename, std::ios_base::out);
	if (!file.good()) {
		throw std::runtime_error("Unable to save music visualization data file: " + filename);
	}

	file << visualization.averageSampleValue << std::endl;
	file << visualization.data.size() << std::endl;

	for (const auto& column : visualization.data) {
		for (const auto& value : column) {
			file << value << ',';
		}
		file << std::endl;
	}
}

MusicStats MusicVisulizationManager::LoadMusicStats(const std::string& filename)
{
	std::fstream file(filename, std::ios_base::in);
	if (!file.good()) {
		throw std::runtime_error("Unable to open music stats file: " + filename);
	}

	MusicStats stats;
	file >> stats.bestScore;
	file >> stats.bestTime;

	return stats;
}

void MusicVisulizationManager::SaveMusicStats(const MusicStats& stats, const std::string& filename)
{
	std::fstream file(filename, std::ios_base::out);
	if (!file.good()) {
		throw std::runtime_error("Unable to save music stats file: " + filename);
	}

	file << stats.bestScore << std::endl;
	file << stats.bestTime << std::endl;
}

}