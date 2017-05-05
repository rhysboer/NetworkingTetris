#pragma once
#include "BaseApplication.h"
#include "InputManager.h"
#include "GameWindow.h"
#include "Input.h"
#include "Board.h"

class Application : public BaseApplication {
public:
	Application();
	~Application();

	const bool Update() override;
	void Draw() override;

	void OnStartUp() override;
	void OnShutDown() override;
private:

	int test = 0;

	
	Board* m_board;
	GameWindow* m_window = nullptr;
	Input* m_player = nullptr;
	
};

