#pragma once

#include "GuiManager.hpp"
#include "Runnable.hpp"
#include "World.hpp"
#include "MusicVisualizationManager.hpp"

namespace hexagon {

class Game final : public Runnable {
public:

	static constexpr float TIMERATE = 1 / 20.f;
	
	Game(const sf::Font& font,
		sf::Music& music,
		MusicVisualization&& visualization, 
		Camera& camera,
		size_t numBeatUnits);

	void Start();
	void Stop();

	void KeyPressed(sf::Keyboard::Key key) override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) const override;
	
private:

	static constexpr float MUSIC_WITH_BEAT_MANAGER_SYNC_TIME = 5.f;
	static constexpr float CAMERA_VELOCITY = 700.f;
	static constexpr auto PLAYER_KEY_JUMP = sf::Keyboard::Space;
	static constexpr auto PLAYER_KEY_FALL_DOWN_FAST = sf::Keyboard::C;
	static constexpr auto PAUSE_KEY = sf::Keyboard::P;
	static constexpr float PLAYER_DEATH_WAIT_TIME = 1.f;
	static constexpr float LAST_SECONDS_WITHOUT_OBSTACLES = 3.f;
	
	void Reset();
	void UpdatePlayerAndCamera(float deltaTime);
	void UpdatePlayerDeath(float deltaTime);
	void UpdateGui(float deltaTime);
	void SyncMusicAndBeatManager(float deltaTime);

	bool ShouldSpawnObstacles() const;

	const sf::Font& _font;
	sf::Music& _music;
	Camera& _camera;
	BeatUnitManager _beatUnitManager;
	std::unique_ptr<World> _world;
	std::unique_ptr<gui::GuiManager> _guiManager;
	std::optional<std::reference_wrapper<gui::GameStatsHUD>> _statsHUD;
	bool _stopped = true;
	float _musicBeatManagerSyncTimer;
	float _playerDeathWaitTimer;
};

}