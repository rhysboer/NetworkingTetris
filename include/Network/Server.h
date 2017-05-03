#pragma once
#include <iostream>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

#include "UserPacket.h"

class Server {
public:
	Server();
	Server(unsigned short port, unsigned int maxConnections);
	~Server();
	 
	void CreateServer(unsigned short port, unsigned int maxConnections);
	void Update();

	RakNet::Packet* GetPacket();
	void DeallocatePacket(RakNet::Packet* packet);
	void SendNewClientID(RakNet::SystemAddress address);

	RakNet::RakPeerInterface* GetPeerInterface();
private:

	void HandleMessages();

	// Peer Interface
	RakNet::RakPeerInterface* m_pPeerInterface = nullptr;

	// Server Port
	unsigned short m_port = 0;
	// Next client ID
	unsigned int m_nextClientID = 1;
};

