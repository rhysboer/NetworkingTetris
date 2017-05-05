#pragma once
#include "InputManager.h"
#include "Board.h"

class Input {
public:
	Input();
	~Input();

	void Update(Board &board);

private:

};

/*
TODO:
	Add in Completed line removals
	Score

	Fix bug with block removing
	Clean up Server & Client Code
	Clean up Board Code

	Clean up code in Board
*/

