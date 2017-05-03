#pragma once
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "GameMessages.h"

class UserPacket {
public:
	UserPacket(unsigned int clientID, GameMessages id = GameMessages::ID_SERVER_SET_CLIENT_ID);
	~UserPacket();

	void AddToPacket(const int message);
	void AddToPacket(const float message);
	void AddToPacket(const char* message);

	void SendPacket(RakNet::RakPeerInterface* server, char orderChannel = 0, RakNet::AddressOrGUID address = RakNet::UNASSIGNED_SYSTEM_ADDRESS, bool broadCast = true);
private:
	RakNet::BitStream m_bs;
};

