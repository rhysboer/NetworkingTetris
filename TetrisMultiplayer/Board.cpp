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

	CreateNewPiece();
}

Board::~Board() {
}

void Board::Update() {
	if (m_startPos.x != 0 && m_startPos.y != 0 && MyClient::IsGameReady()) {
		// Drop the current piece down
		DropPiece();
		// Update Player 2 position (same thing as DropPiece();)
		GetPlayer2Block();
		GetPlayer2Position();
		GetPlayer2Rotation();

		// Draw Player 1
		for (int i = 0; i < 4; i++) {
			Piece* piece = PositionInGrid(m_block->m_currShape[i] + m_blockPosition);
			piece->ChangeColour(m_block->m_colour);
			piece->SetStatus(CurrentStatus::PLAYER);
		}

		// Draw Player 2
		if(m_player2.m_block != nullptr) {
			for(int i = 0; i < 4; i++) {
				Piece* piece = PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition);
				piece->ChangeColour(m_player2.m_block->m_colour);
				piece->SetStatus(CurrentStatus::OTHER_P);
			}
		}


	} else {
		SendServerNewPosition();

		GetStartingPosition(); 
		GetPlayer2Block(); /* Get Player 2 piece */
		GetPlayer2StartPos(); /* Get Player 2 start position */
	}

	CheckPlayer2Stats();
}

// Draw Board
void Board::Draw(sf::RenderWindow & render) {
	std::vector<Piece*>::iterator iter = m_grid.begin();
	for (; iter != m_grid.end(); iter++) {
		(*iter)->Draw(render);
	}
}

bool Board::MovePiece(Direction dire) {
	// CHECK WITH SERVER IF PIECE IS ABLE TO MOVE

	if (m_startPos.x != 0 && m_startPos.y != 0 && CheckPlayer2Stats() == true) {
		switch (dire) {
		case Direction::Left:
			if (SideCollisionCheck((Shapes::MostLeftPosition(*m_block) + sf::Vector2f(-1, 0)) + m_blockPosition)) {
				if (BlockCollisionCheck(*m_block, sf::Vector2f(-1, 0)) == false) {
					// FIND MOST RIGHEST PIECE
					ClearLastSpot();
					m_blockPosition += sf::Vector2f(-1, 0);
					// CLEAR OLD SPOT

					SendServerNewPosition();
				}
			}
			break;
		case Direction::Right:
			if (SideCollisionCheck((Shapes::MostRightPosition(*m_block) + sf::Vector2f(1, 0)) + m_blockPosition)) {
				if (BlockCollisionCheck(*m_block, sf::Vector2f(1, 0)) == false) {
					ClearLastSpot();
					m_blockPosition += sf::Vector2f(1, 0);

					SendServerNewPosition();
				}
			}
			break;
		case Direction::Down:
			// FIND LOWEST POINT IN SHAPE + CURRENT POS + NEW MOVE SPOT
			if (SideCollisionCheck((Shapes::LowestPosition(*m_block) + m_blockPosition) + sf::Vector2f(0, 1))) {
				if (BlockCollisionCheck(*m_block, sf::Vector2f(0, 1)) == false) {
					ClearLastSpot();
					m_blockPosition += sf::Vector2f(0, 1);

					SendServerNewPosition();
				}
			}
			break;
		}
	}

	return false;
}

bool Board::RotatePiece(Shapes::Rotation rotate) {
	// CHECK WITH SERVER

	switch (rotate) {
	case Shapes::Rotation::Right:
		if (RotationCollisionCheck(Shapes::GetNextRotation(*m_block, rotate)) == true) {
			ClearLastSpot();
			Shapes::RotateBlock(*m_block, rotate);

			SendServerNewPosition();
		}
		break;
	case Shapes::Rotation::Left:
		if (RotationCollisionCheck(Shapes::GetNextRotation(*m_block, rotate)) == true) {
			ClearLastSpot();
			Shapes::RotateBlock(*m_block, rotate);

			SendServerNewPosition();
		}
		break;
	}

	return true;
}

void Board::DropPiece() {
	m_currTime += DeltaTime::Time();
	
	if (m_currTime > DROPSPEED) {
		m_currTime = 0;

		// If block has hit the bottom or another piece
		if (GridCollisionCheck() == false || BlockCollisionCheck(*m_block, sf::Vector2f(0, 1)) == true) {
			for (int i = 0; i < 4; i++) {
				PositionInGrid(m_block->m_currShape[i] + m_blockPosition)->SetStatus(CurrentStatus::FILLED);
			}

			CreateNewPiece();
		} else {
			// BLOCK IS CLEAR DROP PIECE DOWN ONE
			ClearLastSpot();
			++m_blockPosition.y;

			SendServerNewPosition();
		}
	}
}

Piece* Board::PositionInGrid(sf::Vector2f position) {
	int pos = position.x + (position.y * m_size.x);
	if (pos > m_grid.size()) 
		return nullptr;

	return m_grid.at(pos);
	// TODO check if out of range
}

Piece* Board::PositionInGrid(int x, int y) {
	return PositionInGrid(sf::Vector2f(x, y));
}

// Clears player 1's last position
void Board::ClearLastSpot() {
	for (int i = 0; i < 4; i++) {
		PositionInGrid(m_block->m_currShape[i] + m_blockPosition)->ChangeColour(ShapeColour::Colour::EMPTY);
		PositionInGrid(m_block->m_currShape[i] + m_blockPosition)->SetStatus(CurrentStatus::EMPTY);
	}
}

// Clear player 2's last position
void Board::ClearLastSpotPlayer2() {
	for(int i = 0; i < 4; i++) {
		if(m_player2.m_block != nullptr) {
			Piece* piece = PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition);
			if(piece->GetStatus() != CurrentStatus::FILLED) {
				piece->ChangeColour(ShapeColour::Colour::EMPTY);
				piece->SetStatus(CurrentStatus::EMPTY);
			}
		}
	}
}

void Board::PlacePlayer2Piece() {
	if(m_player2.m_block != nullptr) {
		for(int i = 0; i < 4; i++) {
			PositionInGrid(m_player2.m_block->m_currShape[i] + m_player2.m_blockPosition)->SetStatus(CurrentStatus::FILLED);
		}
	}
}

bool Board::SideCollisionCheck(sf::Vector2f position) {
	// Grid Side Collisions
	if (position.x > m_size.x - 1 || position.x < 0)
		return false;
	if (position.y > m_size.y - 1 || position.y < 0)
		return false;

	return true;
}

// CHECK FOR COLLISONS WITH OTHER BLOCKS
// CHANGE TO BLOCK SHAPE INSTEAD OF SHAPE BASE
bool Board::BlockCollisionCheck(Shapes::ShapeBase block, sf::Vector2f offset) {
	sf::Vector2f position;// = m_blockPosition + offset;
	Piece* piece;

	for (int i = 0; i < 4; i++) {
		position = block.m_currShape[i] + (m_blockPosition + offset);
		piece = PositionInGrid(sf::Vector2f(position));
		if (piece == nullptr) return true;
		if (piece->GetStatus() != CurrentStatus::PLAYER){
			if (piece->GetStatus() != CurrentStatus::EMPTY) {
				return true;
			}
		}
	}

	return false;
}

bool Board::RotationCollisionCheck(Shapes::shape block) {
	for(int i = 0; i < 4; i++) {
		if((block[i].m_x + m_blockPosition.x) > m_size.x - 1 || (block[i].m_x + m_blockPosition.x) < 0)
			return false;
		if((block[i].m_y + m_blockPosition.y) > m_size.y - 1 || (block[i].m_y + m_blockPosition.y) < 0)
			return false;

		Piece* piece = PositionInGrid(sf::Vector2f(block[i].m_x, block[i].m_y) + m_blockPosition);
		if(piece->GetStatus() == CurrentStatus::FILLED || piece->GetStatus() == CurrentStatus::OTHER_P) {
			return false;
		}
	}

	// TODO check collisions with other pieces

	return true;
}

bool Board::GridCollisionCheck() {
	// CHECK EVERY PIECE FOR A COLLISION WITH GRID BOTTOM / PIECES
	for (int i = 0; i < 4; i++) {
		sf::Vector2f position = (m_block->m_currShape[i] + m_blockPosition) + sf::Vector2f(0, 1);
		if (SideCollisionCheck(position) == false) {
			return false;
		}
	}

	// Piece Collision here

	return true;
}

void Board::CreateNewPiece() {
	if (m_block != nullptr)
		delete m_block;
	
	m_block = Shapes::GetRandomBlock();
	m_blockPosition = m_startPos;

	// TELL SERVER OUR NEW PIECE
	SendServerNewBlock();
}

void Board::GetStartingPosition() {
	Position pos = MyClient::GetStartPos();
	if (pos.m_x != 0 && pos.m_y != 0) {
		m_startPos = sf::Vector2f(pos.m_x, pos.m_y);
		m_blockPosition = m_startPos;
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
	if (pos.m_x != 0 && pos.m_y != 0) {
		m_player2.m_startPos = sf::Vector2f(pos.m_x, pos.m_y);
		m_player2.m_blockPosition = m_player2.m_startPos;
	}
}

void Board::GetPlayer2Position() {
	// Get Player 2 Position
	Position pos = MyClient::GetOthersPosition();
	if(pos.m_x != 0 && pos.m_y != 0) {
		if(pos.m_y < m_player2.m_blockPosition.y) {
			// If new position is 'higher' than the last poisiton, player 2 must have a new block

			delete m_player2.m_block;
			m_player2.m_block = nullptr;

			m_player2.m_blockPosition = sf::Vector2f(pos.m_x, pos.m_y);
			MyClient::ClearOthersPiece();
			GetPlayer2Block();

			return;
		}
	}

	if(pos.m_x != 0 && pos.m_y != 0) {
		if(pos.m_y != m_player2.m_blockPosition.y || pos.m_x != m_player2.m_blockPosition.x) { //if(pos.m_x != m_player2.m_blockPosition.x || pos.m_y != m_player2.m_blockPosition.y) {
			ClearLastSpotPlayer2();
		}

		m_player2.m_blockPosition = sf::Vector2f(pos.m_x, pos.m_y);
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
	packet.AddToPacket((int)m_block->m_type);
	packet.SendPacket(MyClient::GetClient()->GetPeerInterface());

	switch(m_block->m_type) {
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
	packet.AddToPacket((int)m_block->m_type);
	packet.AddToPacket(m_blockPosition.x);
	packet.AddToPacket(m_blockPosition.y);
	packet.AddToPacket(m_block->m_currRotation);
	packet.SendPacket(MyClient::GetClient()->GetPeerInterface());
}

