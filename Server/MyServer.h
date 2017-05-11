#pragma once
#include "Server.h"
#include "ShapesArray.h"

#define PLAYER_1_POS_X 3.0f
#define PLAYER_1_POS_Y 2.0f

#define	PLAYER_2_POS_X 6.0f
#define PLAYER_2_POS_Y 3.0f

struct Position {
	Position(float a_x, float a_y) { x = a_x; y = a_y; }
	float x = -1; float y = -1;
};

struct Player {
	Shapes::BlockType piece = Shapes::BlockType::NONE;
	Position startPos = Position(0,0);
	Position currPos = Position(0,0);
	unsigned int currRotate = 1;
};

class MyServer {
public:
	MyServer();
	~MyServer();

	void CreateServer();
	void Update();

private:

	void HandleMessages();

	void SetStartPosition(RakNet::Packet* packet);	/* Set players start position */
	void SetNewBlock(RakNet::Packet* packet);		/* Sets players new block */
	void SetNewPosition(RakNet::Packet* packet);	/* Sets players new position */
	void SetPlayerReady(RakNet::Packet* packet);	/* Set player ready for game */

	void SendStartPos(RakNet::Packet* packet);
	void SendOthersCurrentBlock(RakNet::Packet* packet);


	Server* m_server = nullptr;

	/* Player 1*/
	bool m_player1PosSet = false; /* Has players starting pos been set */
	bool m_player1Ready = false; /* Is player ready to begin */
	Shapes::BlockType m_player1BlockType = Shapes::BlockType::NONE; /* Current Shape */
	Shapes::ShapeBase m_player1Shape; /* Current block; also rotation PROBABLY DONT NEED! */
	float m_player1PosX = 0; /* Current block position */
	float m_player1PosY = 0;

	/* Player 2*/
	bool m_player2PosSet = false;
	bool m_player2Ready = false;
	Shapes::BlockType m_player2BlockType = Shapes::BlockType::NONE;
	Shapes::ShapeBase m_player2Shape;
	float m_player2PosX = 0;
	float m_player2PosY = 0;



	// TEST
	void UpdatePlayerStats(RakNet::Packet* packet);

	Player m_playerOne;
	Player m_playerTwo;
};



