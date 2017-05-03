#include "MyClient.h"

MyClient* MyClient::m_instance = nullptr;

MyClient::MyClient() {
	m_client = new Client();
}

void MyClient::HandleMessages() {
	RakNet::Packet* packet;

	for (packet = m_client->GetPacket(); packet; m_client->DeallocatePacket(packet), packet = m_client->GetPacket()) {
		switch (packet->data[0]) {
		case ID_SERVER_SET_CLIENT_ID:
			m_client->SetClientID(packet);
			printf("This Client ID: %i\n", m_client->GetClientID());
			break;
		case ID_SERVER_SET_PLAYER_START_POS:
			SetPlayerPos(packet);
			break;
		case ID_SERVER_SEND_NEW_OTHERS_PIECE:
			SetOthersBlock(packet);
			break;
		case ID_CLIENT_REQUEST_OTHERS_START_POS:
			SetOthersStartPos(packet);
			break;
		case ID_SERVER_SEND_NEW_OTHERS_POSITION:
			SetOthersPosition(packet);
			break;
		case ID_SERVER_GAME_READY:
			m_instance->m_gameReady = true;
			break;

		// TEST
		case ID_SERVER_SEND_PLAYER_UPDATE:
			UpdateOtherPlayer(packet);
			break;

		default:
			break;
		}
	}
}

void MyClient::SetPlayerPos(RakNet::Packet* packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));
	bs.Read(m_instance->m_startPos.m_x);
	bs.Read(m_instance->m_startPos.m_y);
}

void MyClient::SetOthersBlock(RakNet::Packet * packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int packetClientID;
	bs.Read(packetClientID);
	if (packetClientID == m_client->GetClientID()) {
		bs.Read(m_instance->m_p2Shape);
	}
}

void MyClient::SetOthersStartPos(RakNet::Packet * packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int packetClientID;
	bs.Read(packetClientID);
	if (packetClientID == m_client->GetClientID()) {
		bs.Read(m_instance->m_p2StartPos.m_x);
		bs.Read(m_instance->m_p2StartPos.m_y);
	}
}

void MyClient::SetOthersPosition(RakNet::Packet * packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int packetClientID;
	bs.Read(packetClientID);
	if(packetClientID != m_client->GetClientID()) {
		bs.Read(m_instance->m_p2Pos.m_x);
		bs.Read(m_instance->m_p2Pos.m_y);
		printf("Client %i: Updated client %i's Position (X: %i, Y: %i)\n", m_client->GetClientID(), packetClientID, (int)m_instance->m_p2Pos.m_x, (int)m_instance->m_p2Pos.m_y);
	}
}

void MyClient::UpdateOtherPlayer(RakNet::Packet * packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int id = 0;
	bs.Read(id);


	if(id != m_instance->m_client->GetClientID()) {
		(Shapes::BlockType)bs.Read(m_instance->m_p2Shape);
		bs.Read(m_instance->m_p2Pos.m_x);
		bs.Read(m_instance->m_p2Pos.m_y);
		bs.Read(m_instance->m_p2Rotation);
	}
}

void MyClient::Init() {
	if (m_instance == nullptr)
		m_instance = new MyClient();
}

MyClient * MyClient::GetIntance() {
	if (m_instance == nullptr)
		m_instance = new MyClient();

	return m_instance;
}

Client * MyClient::GetClient() {
	if (m_instance != nullptr) 
		return m_instance->m_client;
	return nullptr;
}

void MyClient::Destroy() {
	// might break
	delete m_instance->m_client;
	m_instance->m_client = nullptr;
	delete m_instance;
	m_instance = nullptr;
}

void MyClient::Update() {
	m_instance->HandleMessages();
}

Position MyClient::GetStartPos() {
	return m_instance->m_startPos;
}

void MyClient::ClearOthersPiece() {
	m_instance->m_p2Shape = Shapes::BlockType::NONE;
}

Shapes::BlockType MyClient::GetOthersBlock() {
	if(m_instance->m_p2Shape == Shapes::BlockType::NONE) {
		UserPacket packet(m_instance->GetClient()->GetClientID(), GameMessages::ID_CLIENT_REQUEST_OTHERS_PIECE);
		packet.SendPacket(m_instance->GetClient()->GetPeerInterface());
	}

	return m_instance->m_p2Shape;
}

Position MyClient::GetOthersStartPos() {
	if (m_instance->m_p2StartPos.m_x == 0 && m_instance->m_p2StartPos.m_y == 0) {
		//UserPacket packet(m_instance->GetClient()->GetClientID(), ID_CLIENT_REQUEST_OTHERS_START_POS);
		//packet.SendPacket(m_instance->GetClient()->GetPeerInterface());
		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_REQUEST_OTHERS_START_POS);
		bs.Write(m_instance->GetClient()->GetClientID());
		
		m_instance->GetClient()->GetPeerInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	return m_instance->m_p2StartPos;
}

Position MyClient::GetOthersPosition() {
	return m_instance->m_p2Pos;
}

unsigned int MyClient::GetOthersRotation() {
	return m_instance->m_p2Rotation;
}

bool MyClient::IsGameReady() {
	return m_instance->m_gameReady;
}

MyClient::~MyClient() {
}