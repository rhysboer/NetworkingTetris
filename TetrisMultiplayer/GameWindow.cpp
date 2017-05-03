#include "GameWindow.h"

void GameWindow::StartDraw() {
	m_window->clear();
}

void GameWindow::EndDraw() {
	m_window->display();
	InputManager::Clear();
}

bool GameWindow::Update() {
	// Poll Events
	sf::Event e;
	while (m_window->pollEvent(e)) {
		if (e.type == sf::Event::Closed) {
			m_window->close();
			return true;
		}

		if (e.type == sf::Event::KeyPressed || e.type == sf::Event::KeyReleased)
			InputManager::UpdateInputs(e);
	}

	return false;
}

sf::RenderWindow * GameWindow::Renderer() {
	return m_window;
}

GameWindow::GameWindow(const char * title, int x, int y) {
	m_window = new sf::RenderWindow(sf::VideoMode(x, y), title);
	m_window->setKeyRepeatEnabled(false);
	m_window->setFramerateLimit(120);
}

GameWindow::~GameWindow() {
	if (m_window != nullptr)
		delete m_window;
	
	m_window = nullptr;
}
