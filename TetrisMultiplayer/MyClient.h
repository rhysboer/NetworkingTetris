#pragma once
#include "Client.h"
#include "ShapesArray.h"
#include "UserPacket.h"

struct Position {
	Position(float x, float y) { m_x = x; m_y = y; }
	float m_x;
	float m_y;
};

class MyClient {
public:
	~MyClient();

	static void Init();
	static MyClient* GetIntance();
	static Client* GetClient();

	static void Destroy();

	static void Update();

	static Position GetStartPos();

	static void ClearOthersPiece();

	static Shapes::BlockType GetOthersBlock();
	static Position GetOthersStartPos();
	static Position GetOthersPosition();
	static unsigned int GetOthersRotation();
	
	// Check if players are ready
	static bool IsGameReady();

private:
	MyClient();

	void HandleMessages();
	void SetPlayerPos(RakNet::Packet* packet);

	void SetOthersBlock(RakNet::Packet* packet);
	void SetOthersStartPos(RakNet::Packet* packet);
	void SetOthersPosition(RakNet::Packet* packet);

	// TEST
	void UpdateOtherPlayer(RakNet::Packet* packet);

	static MyClient* m_instance;
	Client* m_client;

	bool m_gameReady = false;

	Position m_startPos = Position(0, 0);

	// Player 2's block
	Shapes::BlockType m_p2Shape = Shapes::BlockType::NONE;
	Position m_p2StartPos = Position(0, 0);
	Position m_p2Pos = Position(0, 0);
	unsigned int m_p2Rotation = 1;
};

