#include "InputManager.h"

InputManager* InputManager::m_instance = nullptr;

InputManager::InputManager() {
	m_currTime = 0;
	m_timeDelay = 0.2; //0.1;
}

InputManager::~InputManager() {
}

InputManager * InputManager::Instance() {
	if (m_instance == nullptr) {
		m_instance = new InputManager();
	}

	return m_instance;
}

void InputManager::UpdateInputs(sf::Event & e) {
	InputManager* instance = Instance();
	instance->m_event = e;
}

void InputManager::Clear() {
	InputManager* instance = Instance();
	instance->m_event = sf::Event();

	for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
		if (instance->m_currKeyState[i] != KeyState::NONE) {
			int  i = 0;
		}

		instance->m_prevKeyState[i] = instance->m_currKeyState[i];
		instance->m_currKeyState[i] = KeyState::NONE;
	}

	instance->m_currTime += DeltaTime::Time();
}

bool InputManager::KeyPressed(sf::Keyboard::Key key) {
	InputManager* instance = Instance();

	if (instance->m_event.type == sf::Event::KeyPressed) {
		if (instance->m_event.key.code == key) {
			instance->m_currKeyState[key] = KeyState::DOWN;
			return true;
		}
	}

	return false;
}

// TODO fix input manager
bool InputManager::KeyDown(sf::Keyboard::Key key) {
	InputManager* instance = Instance();

	if (sf::Keyboard::isKeyPressed(key)) {
		if (instance->m_currTime >= instance->m_timeDelay) {
			instance->m_currTime = 0.0f;
			return true;
		}
	}

	return false;
}
