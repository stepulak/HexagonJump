#pragma once

#include "GuiManager.hpp"
#include "Runnable.hpp"
#include "World.hpp"
#include "MusicVisualizationManager.hpp"

namespace hexagon {

class Game final : public Runnable {
public:

	using Ptr = std::unique_ptr<Game>;

	static constexpr float TIMERATE = 1 / 20.f;
	static constexpr size_t NUM_BEAT_UNITS = 8u;

	Game(const sf::Font& font,
		Camera& camera,
		const std::string& musicName,
		MusicVisulizationManager& manager);

	bool WantToQuit() const override { return _wantToQuit; }

	void Start();
	void Stop();

	void KeyPressed(const sf::Keyboard::Key& key) override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) const override;
	
private:

	static constexpr float MAX_DELTA_TIME = 1 / 24.f;
	static constexpr float CAMERA_VELOCITY = 700.f;
	static constexpr float LAST_SECONDS_WITHOUT_OBSTACLES = 10.f;
	static constexpr float MUSIC_BEAT_MANAGER_SYNC_TIME = 5.f;

	static constexpr float FONT_SIZE = 40.f;

	static constexpr float PLAYER_DIED_WAIT_TIME = 1.f;
	static constexpr auto PLAYER_DIED_RESULT_MESSAGE = "You died!";
	static constexpr auto PLAYER_WON_RESULT_MESSAGE = "You won!";

	static constexpr auto QUIT_DIALOG_NAME = "quit_dialog";
	static constexpr auto QUIT_DIALOG_TEXT = "Man, you sure you wanna give up?";
	static constexpr float QUIT_DIALOG_WIDTH = 600;
	static constexpr float QUIT_DIALOG_HEIGHT = 200;

	static constexpr auto TIME_HUD_NAME = "time_hud";
	static constexpr auto END_SCREEN_HUD_NAME = "end_screen_hud";

	bool MusicEnded() const { return _music.getStatus() == sf::SoundSource::Stopped; }

	gui::GameStatsHUD& GetTimeHUD() { 
		return _guiManager->GetGuiElement<gui::GameStatsHUD>(TIME_HUD_NAME);
	}

	const gui::GameStatsHUD& GetTimeHUD() const {
		return _guiManager->GetGuiElement<gui::GameStatsHUD>(TIME_HUD_NAME);
	}

	float GetMusicTime() const;

	void Reset();
	void CreateGUI();
	void CreateQuitDialog();
	void CreateEndScreenHUD(const std::string& resultMessage);
	void UpdateGUI(float deltaTime);
	void TestIfPlayerWon();
	void UpdatePlayerAndCamera(float deltaTime);
	void UpdatePlayerDeath(float deltaTime);
	void SyncMusicAndBeatManager(float deltaTime);
	void GameEnded(const std::string& resultMessage);

	bool ShouldSpawnObstacles() const;

	const sf::Font& _font;
	Camera& _camera;
	MusicVisulizationManager& _musicVisualizationManager;
	const std::string _musicName;
	sf::Music _music;
	std::unique_ptr<BeatUnitManager> _beatUnitManager;
	std::unique_ptr<World> _world;
	std::unique_ptr<gui::GuiManager> _guiManager;
	float _musicBeatManagerSyncTimer;
	float _playerDiedWaitTimer;
	bool _gameEnded;
	bool _stopped = true;
	bool _wantToQuit = false;
};

}