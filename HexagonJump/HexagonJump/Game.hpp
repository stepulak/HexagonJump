#pragma once

#include "Runnable.hpp"
#include "World.hpp"
#include "MusicVisualizationManager.hpp"

namespace hexagon {

class Game final : public Runnable {
public:

	static constexpr float TIMERATE = 1 / 20.f;
	
	Game(sf::Music& music, MusicVisualization&& visualization, size_t numBeatUnits, Camera& camera);

	void Start();
	void Stop();

	void KeyPressed(sf::Keyboard::Key key) override;
	void KeyReleased(sf::Keyboard::Key key) override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) const override;
	
private:

	static constexpr float MUSIC_WITH_BEAT_MANAGER_SYNC_TIME = 5.f;
	static constexpr float CAMERA_VELOCITY = 700.f;
	static constexpr auto PLAYER_KEY_JUMP = sf::Keyboard::Space;
	static constexpr auto PLAYER_KEY_FALL_DOWN_FAST = sf::Keyboard::C;
	static constexpr auto PLAYER_KEY_MOVE_LEFT = sf::Keyboard::A;
	static constexpr auto PLAYER_KEY_MOVE_RIGHT = sf::Keyboard::D;
	static constexpr auto PAUSE_KEY = sf::Keyboard::P;
	static constexpr float PLAYER_DEATH_WAIT_TIME = 3.f;

	void Reset();
	void ResolvePlayerDeath(float deltaTime);
	void MoveCameraAndPlayer(float deltaTime);
	void SyncMusicAndBeatManager(float deltaTime);

	sf::Music& _music;
	Camera& _camera;
	BeatUnitManager _beatUnitManager;
	std::unique_ptr<World> _world;
	bool _stopped = true;
	float _musicBeatManagerSyncTimer;
	float _playerDeathWaitTimer;
};

}