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
	// ADD
	[Priority: MED ] Add in Completed line removals					[]
	[Priority: LOW ] Add Scores										[]

	// BUG
	(player 2 is doing it to player 1)
	[Priority: HIGH] Fix bug with block removing					[DONE]
	[Priority: MED ] Fix cant move without player 2 being existant	[]
	[Priority: MED ] Fix server spam when its only 1 player			[]
	 
	 // MISC
	[Priority: HIGH] Clean up Server & Client Code					[]
	[Priority: MED ] Clean up Board Code							[]
*/

