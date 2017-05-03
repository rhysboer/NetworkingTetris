#pragma once

#define WINDOW_TITLE "Tetris"
#define WINDOW_X 1280
#define WINDOW_Y 720

class BaseApplication {
public:
	virtual const bool Update() = 0;
	virtual void Draw() = 0;

	virtual void OnStartUp() = 0;
	virtual void OnShutDown() = 0;
};

