#pragma once
#include "SFML\Graphics.hpp"
#include "Piece.h"
#include "ShapesArray.h"
#include "DeltaTime.h"
#include "SFML\Graphics\Font.hpp"

#include "MyClient.h"
#include "UserPacket.h"

#include <string>

enum class Direction {
	Left, Right, Down
};

struct Size {
	int x;
	int y;
};

struct Player {
	sf::Vector2f m_startPos = sf::Vector2f(0, 0);
	sf::Vector2f m_blockPosition;
	Shapes::ShapeBase* m_block = nullptr;
};

class Board {
public:
	Board(int size_x, int size_y);
	~Board();

	void Update();
	void Draw(sf::RenderWindow &render);

	bool MovePiece(Direction dire);
	bool RotatePiece(Shapes::Rotation rotate);

	void DropPiece();


	// DEBUG
	void ShowStatus(sf::RenderWindow &render);

private:
	// DEBUG
	sf::Font m_font;
	// DEBUG


	Piece* PositionInGrid(sf::Vector2f position);
	Piece* PositionInGrid(int x, int y);

	void ClearLastSpot();
	void ClearLastSpotPlayer2();
	void PlacePlayer2Piece();

	void CreateNewPiece();

	/* COLLISION STUFF */
	// NEED
	// Check to make sure player isnt moving off grid
	bool SideCollisionCheck(sf::Vector2f position); // CHECK FOR SIDE COLLISONS ONLY
	bool BlockCollisionCheck(Shapes::ShapeBase block, sf::Vector2f offset = sf::Vector2f(0,0)); // CHECKS FOR COLLISONS WITH OTHER BLOCKS

	// NOT SURE IF NEEDED
	// Check to make sure player isnt rotating off grid & other pieces
	bool RotationCollisionCheck(Shapes::shape block);
	// Check player 'Landing' piece onto groud or nearby pieces
	bool GridCollisionCheck();
	/* END COLLISION STUFF */


	// SERVER
	void GetStartingPosition();
	void SendServerNewBlock();
	void SendServerNewPosition();

	
	bool CheckPlayer2Stats();
	void GetPlayer2Block();
	void GetPlayer2StartPos();
	void GetPlayer2Position();
	void GetPlayer2Rotation();


	// Variables
	std::vector<Piece*> m_grid;
	Size m_size;

	// Player 1
	Shapes::ShapeBase* m_block = nullptr;
	sf::Vector2f m_blockPosition;
	sf::Vector2f m_startPos = sf::Vector2f(0, 0);

	bool m_readyPacketSent = false;

	// Player 2
	Player m_player2 = Player();

	// Time
	float m_currTime = 0;
	const float DROPSPEED = 500.0f; // 0.7f
};

