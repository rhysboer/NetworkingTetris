#pragma once
#include <string>
#include "SFML\Main.hpp"
#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "InputManager.h"

class GameWindow {
public:
	
	void StartDraw();
	void EndDraw();
	bool Update();

	sf::RenderWindow* Renderer();

	GameWindow(const char* title, int x, int y);
	~GameWindow();
private:

	sf::RenderWindow* m_window = nullptr;
};

