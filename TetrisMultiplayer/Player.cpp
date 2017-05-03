#include "Player.h"

//Test
#include "DeltaTime.h"

Player::Player() {
}

Player::~Player() {
}

void Player::Update(Board &board) {
	if (InputManager::KeyPressed(sf::Keyboard::A)) {
		board.MovePiece(Direction::Left);
	}else if (InputManager::KeyPressed(sf::Keyboard::D)) {
		board.MovePiece(Direction::Right);
	} else if (InputManager::KeyDown(sf::Keyboard::S)) {
		board.MovePiece(Direction::Down);
	}

	if (InputManager::KeyPressed(sf::Keyboard::Q)) {
		board.RotatePiece(Shapes::Rotation::Left);
	} else if (InputManager::KeyPressed(sf::Keyboard::E)) {
		board.RotatePiece(Shapes::Rotation::Right);
	}	
}
