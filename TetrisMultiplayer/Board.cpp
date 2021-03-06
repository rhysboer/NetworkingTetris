#include "Board.h"

// TEST
#include "InputManager.h"

Board::Board(int size_x, int size_y) {
	const float x_sideOffset = 32;
	const float y_sideOffset = 32;

	m_size.x = size_x;
	m_size.y = size_y;

	for (int y = 0; y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			Piece* newBlock = new Piece(sf::Vector2f(x * x_sideOffset, y * y_sideOffset), Colour::EMPTY, CurrentStatus::EMPTY);
			m_grid.push_back(newBlock);
		}
	}


	// DEBUG 
	// load font
	m_font.loadFromFile("bin/fonts/AdobeFanHeitiStd-Bold.otf");


	CreateNewPiece();
}

Board::~Board() {
}

// BUG CANT MOVE WHEN PLAYER 2 ISNT HERE

/*
Dont let the board hold the texture for the single piece
make the player hold onto the texture piece
*/

/* Update Every Frame */
void Board::Update() {
	// DELETE AFTER
	if(InputManager::KeyPressed(sf::Keyboard::T))
		m_currTime = 1000.0f;
	


	if (m_player1.m_startPos.x != -1 && m_player1.m_startPos.y != -1 && MyClient::IsGameReady()) {
		// Drop the current piece down
		DropPiece();
		// Update Player 2 position (same thing as DropPiece();)
		GetPlayer2Block();
		GetPlayer2Position();
		GetPlayer2Rotation();

		// Draw Player 2
		if(m_player2.m_block != nullptr) {
			for(int i = 0; i < 4; i++) {
				Piece* piece = PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition);
				//piece->ChangeColour(m_player2.m_block->m_colour);
				piece->SetStatus(CurrentStatus::OTHER_P);
			}
		}

		// Draw Player 1
		for(int i = 0; i < 4; i++) {
			Piece* piece = PositionInGrid(m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition);
			//piece->ChangeColour(m_block->m_colour);
			piece->SetStatus(CurrentStatus::PLAYER);
		}

	} else {
		SendServerNewPosition();

		GetStartingPosition(); 
		GetPlayer2Block(); /* Get Player 2 piece */
		GetPlayer2StartPos(); /* Get Player 2 start position */
	}

	CheckPlayer2Stats();
}

/* Draw Board & Players Every Frame*/
void Board::Draw(sf::RenderWindow & render) {
	std::vector<Piece*>::iterator iter = m_grid.begin();
	for (; iter != m_grid.end(); iter++) {
		(*iter)->Draw(render);
	}

	for(int i = 0; i < 4; i++) {
		if(m_player1.m_block != nullptr) {
			Piece* piece = PositionInGrid(m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition);
			if(piece != nullptr) {
				piece->TempDraw(render, m_player1.m_block->m_colour);
			}
		}

		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
			if(m_player2.m_block != nullptr) {
				Piece* piece = PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition);
				if(piece != nullptr) {
					piece->TempDraw(render, m_player2.m_block->m_colour);
				}
			}
		}
	}


	// DEBUG
	ShowStatus(render);
}

/* Check for Collisions & Move block position */
bool Board::MovePiece(Direction dire) {
	if (MyClient::IsGameReady()) {
		switch (dire) {
		case Direction::Left:
			if (SideCollisionCheck((Shapes::MostLeftPosition(*m_player1.m_block) + sf::Vector2f(-1, 0)) + m_player1.m_blockPosition)) {
				if (BlockCollisionCheck(*m_player1.m_block, sf::Vector2f(-1, 0)) == false) {
					// FIND MOST RIGHEST PIECE
					ClearLastSpot();
					m_player1.m_blockPosition += sf::Vector2f(-1, 0);
					// CLEAR OLD SPOT

					SendServerNewPosition();
				}
			}
			break;
		case Direction::Right:
			if (SideCollisionCheck((Shapes::MostRightPosition(*m_player1.m_block) + sf::Vector2f(1, 0)) + m_player1.m_blockPosition)) {
				if (BlockCollisionCheck(*m_player1.m_block, sf::Vector2f(1, 0)) == false) {
					ClearLastSpot();
					m_player1.m_blockPosition += sf::Vector2f(1, 0);

					SendServerNewPosition();
				}
			}
			break;
		case Direction::Down:
			// FIND LOWEST POINT IN SHAPE + CURRENT POS + NEW MOVE SPOT
			if (SideCollisionCheck((Shapes::LowestPosition(*m_player1.m_block) + m_player1.m_blockPosition) + sf::Vector2f(0, 1))) {
				if (BlockCollisionCheck(*m_player1.m_block, sf::Vector2f(0, 1)) == false) {
					ClearLastSpot();
					m_player1.m_blockPosition += sf::Vector2f(0, 1);

					SendServerNewPosition();
				}
			}
			break;
		}
	}

	return false;
}

/* Check for Collisions & Rotate block */
bool Board::RotatePiece(Shapes::Rotation rotate) {
	// CHECK WITH SERVER

	switch (rotate) {
	case Shapes::Rotation::Right:
		if (RotationCollisionCheck(Shapes::GetNextRotation(*m_player1.m_block, rotate)) == true) {
			ClearLastSpot();
			Shapes::RotateBlock(*m_player1.m_block, rotate);

			SendServerNewPosition();
		}
		break;
	case Shapes::Rotation::Left:
		if (RotationCollisionCheck(Shapes::GetNextRotation(*m_player1.m_block, rotate)) == true) {
			ClearLastSpot();
			Shapes::RotateBlock(*m_player1.m_block, rotate);

			SendServerNewPosition();
		}
		break;
	}

	return true;
}

/* Drop Piece down every few seconds*/
void Board::DropPiece() {
	m_currTime += DeltaTime::Time();
	
	if (m_currTime > DROPSPEED) {
		m_currTime = 0.0f;

		// If block has hit the bottom or another piece
		if (GridCollisionCheck() == false || BlockCollisionCheck(*m_player1.m_block, sf::Vector2f(0, 1)) == true) {
			// BLOCK HAS STOPPED, PLACE IT
			for (int i = 0; i < 4; i++) {
				PositionInGrid(m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition)->SetStatus(CurrentStatus::FILLED);
				PositionInGrid(m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition)->ChangeColour(m_player1.m_block->m_colour);
			}

			CreateNewPiece();
		} else {
			// BLOCK IS CLEAR DROP PIECE DOWN ONE
			ClearLastSpot();
			++m_player1.m_blockPosition.y;

			SendServerNewPosition();
		}
	}
}

/* Returns piece pointer at position in grid */
Piece* Board::PositionInGrid(sf::Vector2f position) {
	int pos = position.x + (position.y * m_size.x);
	if (pos > m_grid.size()) 
		return nullptr;

	return m_grid.at(pos);
	// TODO check if out of range
}
/* Returns piece pointer at position in grid */
Piece* Board::PositionInGrid(int x, int y) {
	return PositionInGrid(sf::Vector2f(x, y));
}

/* Clears the board from players 1 last spot */
void Board::ClearLastSpot() {
	for (int i = 0; i < 4; i++) {
		//sitionInGrid(m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition)->ChangeColour(Colour::EMPTY);
		PositionInGrid(m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition)->SetStatus(CurrentStatus::EMPTY);
	}
}

/* Clears the board from players 2 last spot */
void Board::ClearLastSpotPlayer2() {
	for(int i = 0; i < 4; i++) {
		if(m_player2.m_block != nullptr) {
			Piece* piece = PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition);
			//if(piece->GetStatus() != CurrentStatus::FILLED) {
				//piece->ChangeColour(Colour::EMPTY);
				piece->SetStatus(CurrentStatus::EMPTY);
			//}
		}
	}
}

/* Once player 2 has 'Landed' place blocks */
void Board::PlacePlayer2Piece() {
	if(m_player2.m_block != nullptr) {
		for(int i = 0; i < 4; i++) {
			PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition)->SetStatus(CurrentStatus::FILLED);
			PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition)->ChangeColour(m_player2.m_block->m_colour);
		}
	}
}

/* Check for collisions on side of the grid */
bool Board::SideCollisionCheck(sf::Vector2f position) {
	// Grid Side Collisions
	if (position.x > m_size.x - 1 || position.x < 0)
		return false;
	if (position.y > m_size.y - 1 || position.y < 0)
		return false;

	return true;
}

// CHECK FOR COLLISONS WITH OTHER BLOCKS
/* Params (Current Block, Block Position Offset) | Return (True if collision & False no if collision) */
bool Board::BlockCollisionCheck(Shapes::ShapeBase block, sf::Vector2f offset) {
	sf::Vector2f position;// = m_blockPosition + offset;
	Piece* piece;

	for (int i = 0; i < 4; i++) {
		position = block.m_currShape[i] + (m_player1.m_blockPosition + offset);
		piece = PositionInGrid(sf::Vector2f(position));
		if (piece == nullptr) return true; 

		// if (piece->GetStatus() != CurrentStatus::PLAYER) { // Not colliding withself
		// 	if (piece->GetStatus() != CurrentStatus::EMPTY) { // Not colliding with nothing
		// 		// COLLISION!
		// 		return true;
		// 	}
		// }

		if(piece->GetStatus() == CurrentStatus::FILLED) {
			return true;
		}

	}

	return false;
}

/* Check for rotation Collisions */
bool Board::RotationCollisionCheck(Shapes::shape block) {
	for(int i = 0; i < 4; i++) {
		if((block[i].m_x + m_player1.m_blockPosition.x) > m_size.x - 1 || (block[i].m_x + m_player1.m_blockPosition.x) < 0)
			return false;
		if((block[i].m_y + m_player1.m_blockPosition.y) > m_size.y - 1 || (block[i].m_y + m_player1.m_blockPosition.y) < 0)
			return false;

		Piece* piece = PositionInGrid(sf::Vector2f(block[i].m_x, block[i].m_y) + m_player1.m_blockPosition);
		if(piece->GetStatus() == CurrentStatus::FILLED || piece->GetStatus() == CurrentStatus::OTHER_P) {
			return false;
		}
	}

	// TODO check collisions with other pieces

	return true;
}

/* Checks each piece for side collisions */
bool Board::GridCollisionCheck() {
	// CHECK EVERY PIECE FOR A COLLISION WITH GRID BOTTOM / PIECES
	for (int i = 0; i < 4; i++) {
		sf::Vector2f position = (m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition) + sf::Vector2f(0, 1);
		if (SideCollisionCheck(position) == false) {
			return false;
		}
	}

	// Piece Collision here

	return true;
}

/* Generate a new block (shape) */
void Board::CreateNewPiece() {
	if (m_player1.m_block != nullptr)
		delete m_player1.m_block;
	
	m_player1.m_block = Shapes::GetRandomBlock();
	m_player1.m_blockPosition = m_player1.m_startPos;

	// TELL SERVER OUR NEW PIECE
	SendServerNewBlock();
}

/* Get the starting position from the server */
void Board::GetStartingPosition() {
	Position pos = MyClient::GetStartPos();
	if (pos.m_x != -1 && pos.m_y != -1) {
		m_player1.m_startPos = sf::Vector2f(pos.m_x, pos.m_y);
		m_player1.m_blockPosition = m_player1.m_startPos;
	}
}

/* Check if we have all the player 2 infomation */
bool Board::CheckPlayer2Stats() {
	if (m_player2.m_block == nullptr)
		return false;
	if (m_player2.m_startPos == sf::Vector2f(0, 0))
		return false;

	// Tell server we are ready (only sends once)
	if(m_readyPacketSent == false) {
		UserPacket packet(MyClient::GetClient()->GetClientID(), GameMessages::ID_CLIENT_READY);
		packet.SendPacket(MyClient::GetClient()->GetPeerInterface());
		m_readyPacketSent = true;
	}

	return true;
}

/* Get the second players current block */
void Board::GetPlayer2Block() {
	Shapes::BlockType type = MyClient::GetOthersBlock();
	
	if(m_player2.m_block == nullptr) {
		switch(type) {
		case Shapes::BlockType::NONE: break;
		case Shapes::BlockType::O: m_player2.m_block = new Shapes::ShapeO; printf("player 2 is: I\n"); break;
		case Shapes::BlockType::I: m_player2.m_block = new Shapes::ShapeI; printf("player 2 is: O\n"); break;
		case Shapes::BlockType::S: m_player2.m_block = new Shapes::ShapeS; printf("player 2 is: S\n"); break;
		case Shapes::BlockType::Z: m_player2.m_block = new Shapes::ShapeZ; printf("player 2 is: Z\n"); break;
		case Shapes::BlockType::L: m_player2.m_block = new Shapes::ShapeL; printf("player 2 is: L\n"); break;
		case Shapes::BlockType::J: m_player2.m_block = new Shapes::ShapeJ; printf("player 2 is: J\n"); break;
		case Shapes::BlockType::T: m_player2.m_block = new Shapes::ShapeT; printf("player 2 is: T\n"); break;
		default: break;
		}
	}
}

/* Get player 2's starting position */
void Board::GetPlayer2StartPos() {
	// Get other players piece
	Position pos = MyClient::GetOthersStartPos();
	if (pos.m_x != -1 && pos.m_y != -1) {
		m_player2.m_startPos = sf::Vector2f(pos.m_x, pos.m_y);
		m_player2.m_blockPosition = m_player2.m_startPos;
	}
}

/* Get player 2's current position */
void Board::GetPlayer2Position() {
	// Get Player 2 Position
	Position pos = MyClient::GetOthersPosition();
	if(pos.m_x != -1 && pos.m_y != -1) {
		if(pos.m_y < m_player2.m_blockPosition.y) {
			// If new position is 'higher' than the last poisiton, player 2 must have a new block

			// Removed other players piece for client
			MyClient::ClearOthersPiece();
			PlacePlayer2Piece();

			delete m_player2.m_block;
			m_player2.m_block = nullptr;
			m_player2.m_blockPosition = sf::Vector2f(pos.m_x, pos.m_y);

			GetPlayer2Block();

			return;
		}
	}

	printf("Update Pos X: %i, Y: %i\n", (int)pos.m_x, (int)pos.m_y);

	if(pos.m_x != -1 && pos.m_y != -1) {
		if(pos.m_y != m_player2.m_blockPosition.y || pos.m_x != m_player2.m_blockPosition.x) { //if(pos.m_x != m_player2.m_blockPosition.x || pos.m_y != m_player2.m_blockPosition.y) {
			ClearLastSpotPlayer2();
		}

		m_player2.m_blockPosition = sf::Vector2f(pos.m_x, pos.m_y);
		// NOT REACHING HERE!
	}
}

void Board::GetPlayer2Rotation() {
	unsigned int rotate = MyClient::GetOthersRotation();
	if(m_player2.m_block != nullptr) {
		if(rotate != m_player2.m_block->m_currRotation) {
			if(rotate != 0) {
				if(m_player2.m_block->m_type == MyClient::GetOthersBlock()) {
					ClearLastSpotPlayer2();
					Shapes::SetRotation(*m_player2.m_block, rotate);
				}
			}
		}
	}
}

void Board::SendServerNewBlock() {
	UserPacket packet(MyClient::GetClient()->GetClientID(), GameMessages::ID_CLIENT_SEND_NEW_PIECE);
	packet.AddToPacket((int)m_player1.m_block->m_type);
	packet.SendPacket(MyClient::GetClient()->GetPeerInterface());

	switch(m_player1.m_block->m_type) {
	case Shapes::BlockType::NONE: break;
	case Shapes::BlockType::O: printf("New Piece ID: O\n"); break;
	case Shapes::BlockType::I: printf("New Piece ID: I\n"); break;
	case Shapes::BlockType::S: printf("New Piece ID: S\n"); break;
	case Shapes::BlockType::Z: printf("New Piece ID: Z\n"); break;
	case Shapes::BlockType::L: printf("New Piece ID: L\n"); break;
	case Shapes::BlockType::J: printf("New Piece ID: J\n"); break;
	case Shapes::BlockType::T: printf("New Piece ID: T\n"); break;
	default: break;
	}
}

void Board::SendServerNewPosition() {
	UserPacket packet(MyClient::GetClient()->GetClientID(), GameMessages::ID_CLIENT_SEND_PLAYER_UPDATE);
	packet.AddToPacket((int)m_player1.m_block->m_type);
	packet.AddToPacket(m_player1.m_blockPosition.x);
	packet.AddToPacket(m_player1.m_blockPosition.y);
	packet.AddToPacket(m_player1.m_block->m_currRotation);
	packet.SendPacket(MyClient::GetClient()->GetPeerInterface());
}


// DEBUG ONLY
void Board::ShowStatus(sf::RenderWindow &render) {
	// Debug Status of the current piece
	sf::Text p1Name = sf::Text("Player One", m_font);
	sf::Text p2Name = sf::Text("Player Two", m_font);

	// Positions
	for(int i = 0; i < 4; i++) {
		// Player 1
		sf::String blockPos = sf::String("Block ");
		blockPos += std::to_string(i) + ": X ";
		blockPos += std::to_string((int)(m_player1.m_block->m_currShape[i].m_x + m_player1.m_blockPosition.x)) + " | Y ";
		blockPos += std::to_string((int)(m_player1.m_block->m_currShape[i].m_y + m_player1.m_blockPosition.y));

		sf::Text text = sf::Text(blockPos, m_font);
		text.setPosition(650, 30 + (i * 20));
		text.setCharacterSize(12);

		render.draw(text);
		
		// Player 2
		if(m_player2.m_block != nullptr) {
			blockPos = sf::String("Block ");
			blockPos += std::to_string(i) + ": X ";
			blockPos += std::to_string((int)(m_player2.m_block->m_currShape[i].m_x + m_player2.m_blockPosition.x)) + " | Y ";
			blockPos += std::to_string((int)(m_player2.m_block->m_currShape[i].m_y + m_player2.m_blockPosition.y));

			text = sf::Text(blockPos, m_font);
			text.setPosition(850, 30 + (i * 20));
			text.setCharacterSize(12);

			render.draw(text);
		}
	}

	// Returns the colours name in text
	auto GetColourName = [](Colour colour) -> sf::String {
		switch(colour) {
		case Colour::NONE:		return sf::String("None");
		case Colour::EMPTY:		return sf::String("Empty");
		case Colour::GREEN:		return sf::String("Green");
		case Colour::RED:		return sf::String("Red");
		case Colour::BLUE:		return sf::String("Blue");
		case Colour::CYAN:		return sf::String("Cyan");
		case Colour::PURPLE:	return sf::String("Purple");
		case Colour::YELLOW:	return sf::String("Yellow");
		case Colour::ORANGE:	return sf::String("Orange");
		}
	};

	// Debug Name
	sf::Text name = sf::Text("Block Previous Colour", m_font);
	name.setPosition(650, 150);
	name.setCharacterSize(16);
	render.draw(name);

	name.setPosition(850, 150);
	render.draw(name);

	// Block Debug
	for(int i = 0; i < 4; i++) {
		// Player 1
		Piece* piece = PositionInGrid(m_player1.m_block->m_currShape[i] + m_player1.m_blockPosition);
		sf::String colour = sf::String("None");

		if(piece != nullptr)
			colour = GetColourName(piece->GetPrevColour());

		sf::String prevColour = sf::String("Block ");
		prevColour += std::to_string(i) += ": " + colour;

		sf::Text out = sf::Text(prevColour, m_font);
		out.setPosition(650, 170 + (i * 20));
		out.setCharacterSize(12);

		render.draw(out);

		// Player 2
		colour = "None";
		if(m_player2.m_block != nullptr) {
			piece = PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition);
			if(piece != nullptr) colour = GetColourName(piece->GetPrevColour());

			prevColour = "Block" + std::to_string(i) + ": " + colour;

			out = sf::Text(prevColour, m_font);
			out.setPosition(850, 170 + (i * 20));
			out.setCharacterSize(12);

			render.draw(out);
		}
	}

	p1Name.setPosition(650, 10);
	p1Name.setCharacterSize(16);

	p2Name.setPosition(850, 10);
	p2Name.setCharacterSize(16);

	render.draw(p1Name);
	render.draw(p2Name);

	// Draw Debug Board
	for(int x = 0; x < m_size.x; x++) {
		for(int y = 0; y < m_size.y; y++) {
			sf::String string(((m_grid.at(x + (y * m_size.x))->GetStatus() != CurrentStatus::EMPTY) ? "[0]" : "[  ]"));
			sf::Text text(string, m_font);
			text.setPosition(650 + (x * 15), 250 + (y * 15));
			text.setCharacterSize(12);
			render.draw(text);
		}
	}
}
