#pragma once
#include <iostream>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "GameMessages.h"

class Client {
public:
	Client();
	~Client();

	void Update();
	bool ConnectToServer(const char* ip, unsigned short port, const char* password = nullptr, unsigned int passwordLength = 0);

	unsigned int GetClientID();

	// Handle Packets
	RakNet::Packet* GetPacket();
	void DeallocatePacket(RakNet::Packet* packet);

	void SetClientID(RakNet::Packet* packet);

	RakNet::RakPeerInterface* GetPeerInterface();

private:
	void HandleMessages();

	RakNet::RakPeerInterface* m_pPeerInterface = nullptr;
	
	unsigned int m_clientID = 0;
};

/*
- What will the server and clients need to know about the game

Server:
- Both Players Positions
- Both Players Current Shape

Client:
- Board Pieces
- Players Position
- Other Players Position
- Other Players Shape

Maybe:
- Check for Server Board Collisions as well as client collisions
- (Server) Holds Data of the Board Pieces (MAYBE)


If a player moves a piece send a packet to the server telling them the moved direction
Server then sends a packet to the other client informing them of the position change
(As well as Rotations)
This should also include the Automatic movement too (might need to slow down the movement speed to not flood server with packets)


If a piece hits the bottom of the board or another piece (not 2nd players piece)
send the server a packet informing them of the piece ending


When the player generates a new piece
Send a packet with the new piece and new position to the server


If either players get an error spawning a new piece (Because not enough space due to blocks)
Send an End packet to the server informing it that we failed to place a new block
Server should then send packet to the other player telling them the game has ended

*/
