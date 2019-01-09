#include "MusicVisualizationManager.hpp"
#include "Utils.hpp"
#include "AppInfo.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

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

std::string MusicVisulizationManager::ConvertNewMusic(const std::string& path, 
	float gameTimerate, 
	gui::ThreadSafeProgressBar& progressBar)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path)) {
		throw std::runtime_error("Unable to load music: " + path);
	}

	auto musicName = FilenameWithoutExtension(path);
	auto musicPath = _applicationDataPath + musicName;
	auto visualizationData = CountMusicVisualizationData(buffer,
		gameTimerate, 
		_spectrumColumns,
		progressBar);

	SaveMusicVisualizationToFile(visualizationData, musicPath + DATA_FILE_SUFFIX);
	SaveMusicScore(0u, musicPath + SCORE_FILE_SUFFIX);

	// Copy the music into app's directory
	std::filesystem::remove(musicPath);
	std::filesystem::copy(path, musicPath);
	
	return musicName;
}

MusicData MusicVisulizationManager::LoadMusic(const std::string& musicName) const
{
	auto musicPath = _applicationDataPath + musicName;
	auto musicVisualization = LoadMusicVisualizationFromFile(musicPath + DATA_FILE_SUFFIX, _spectrumColumns);
	auto musicStats = LoadMusicScore(musicPath + SCORE_FILE_SUFFIX);
	return { musicName, musicPath, musicStats, musicVisualization };
}

void MusicVisulizationManager::UpdateScoreIfBetter(const std::string& musicName, unsigned score)
{
	auto statsFilename = _applicationDataPath + musicName + SCORE_FILE_SUFFIX;
	auto oldScore = LoadMusicScore(statsFilename);
	auto betterScore = std::max(score, oldScore);

	_music[musicName] = betterScore;
	SaveMusicScore(betterScore, statsFilename);
}

void MusicVisulizationManager::LoadExistingMusicList()
{
	for (const auto& filename : std::filesystem::directory_iterator(_applicationDataPath)) {
		auto path = filename.path().string();
		auto musicName = FilenameWithoutExtension(path);
		if (_music.find(musicName) == _music.end()) {
			_music[musicName] = LoadMusicScore(path);
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

unsigned MusicVisulizationManager::LoadMusicScore(const std::string& filename)
{
	std::fstream file(filename, std::ios_base::in);
	if (!file.good()) {
		throw std::runtime_error("Unable to open file with music score: " + filename);
	}

	unsigned score;
	file >> score;

	return score;
}

void MusicVisulizationManager::SaveMusicScore(unsigned score, const std::string& filename)
{
	std::fstream file(filename, std::ios_base::out);
	if (!file.good()) {
		throw std::runtime_error("Unable to save file with music score: " + filename);
	}

	file << score << std::endl;
}

}