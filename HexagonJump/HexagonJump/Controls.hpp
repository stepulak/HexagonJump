#pragma once

#include <SFML/Graphics.hpp>

namespace hexagon::controls {

static constexpr auto GUI_BUTTON_PRESS_KEY = sf::Keyboard::Return;
static constexpr auto GUI_NEXT_BUTTON_KEY = sf::Keyboard::Down;
static constexpr auto GUI_NEXT_BUTTON_KEY2 = sf::Keyboard::Left;
static constexpr auto GUI_PREVIOUS_BUTTON_KEY = sf::Keyboard::Up;
static constexpr auto GUI_PREVIOUS_BUTTON_KEY2 = sf::Keyboard::Right;
static constexpr auto GUI_INVOKE_OR_CLOSE_KEY = sf::Keyboard::Escape;

static constexpr auto PLAYER_JUMP_KEY = sf::Keyboard::Space;
static constexpr auto PLAYER_FALL_DOWN_FAST_KEY = sf::Keyboard::C;
static constexpr auto PAUSE_KEY = sf::Keyboard::P;
static constexpr auto RESTART_KEY = sf::Keyboard::K;
static constexpr auto QUIT_KEY = sf::Keyboard::Escape;

static constexpr auto PLAYER_JUMP_KEY_STR = "Space";
static constexpr auto PLAYER_FALL_DOWN_FAST_KEY_STR = "C";
static constexpr auto PAUSE_KEY_STR = "P";
static constexpr auto RESTART_KEY_STR = "K";

}