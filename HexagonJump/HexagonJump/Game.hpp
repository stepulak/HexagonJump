#pragma once

#include "Runnable.hpp"
#include "World.hpp"

namespace hexagon {

class Game final : public Runnable {
public:

	enum class Difficulty {
		EASY,
		MEDIUM,
		HARD,
		PRO
	};

	static constexpr float TIMERATE = 1 / 20.f;

	Game(sf::Music& music, std::pair<MusicVisualizationData, double>& musicData, Camera& camera, Difficulty difficulty);

	void Start();
	void Stop();

	void KeyPressed(sf::Keyboard::Key key) override;
	void KeyReleased(sf::Keyboard::Key key) override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) const override;

	static float CameraSpeedAccordingToDifficulty(float virtualWidth, Difficulty dif);

private:

	static constexpr float MUSIC_WITH_BEAT_MANAGER_SYNC_TIME = 5.f;
	static constexpr float CAMERA_SPEED_LOWEST_DIFFICULTY = 0.2f;
	static constexpr float CAMERA_SPEED_DIFFICULTY_MULTIPLIER = 2.f;
	static constexpr auto PLAYER_KEY_JUMP = sf::Keyboard::Space;
	static constexpr auto PLAYER_KEY_FALL_DOWN_FAST = sf::Keyboard::C;
	static constexpr auto PLAYER_KEY_MOVE_LEFT = sf::Keyboard::A;
	static constexpr auto PLAYER_KEY_MOVE_RIGHT = sf::Keyboard::D;
	static constexpr auto PAUSE_KEY = sf::Keyboard::P;

	void SyncMusicAndBeatManager(float deltaTime);

	sf::Music& _music;
	BeatUnitManager _beatUnitManager;
	World _world;
	bool _stopped = true;
	float _musicBeatManagerSyncTimer = 0.f;
};

}