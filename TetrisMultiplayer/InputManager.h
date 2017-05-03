#pragma once
#include "SFML\Window\Keyboard.hpp"
#include "SFML\Window.hpp"
#include "DeltaTime.h"

enum class KeyState {
	NONE,
	PRESSED,
	RELEASED,
	DOWN,
};

class InputManager {
public:
	~InputManager();

	static InputManager* Instance();

	static void UpdateInputs(sf::Event& e);
	static void Clear();

	static bool KeyPressed(sf::Keyboard::Key key);
	static bool KeyDown(sf::Keyboard::Key key);

private:
	InputManager();

	static InputManager* m_instance;
	sf::Event m_event;

	KeyState m_currKeyState[sf::Keyboard::KeyCount] = { KeyState::NONE };
	KeyState m_prevKeyState[sf::Keyboard::KeyCount] = { KeyState::NONE };

	float m_timeDelay;
	float m_currTime;
};

