#include "MyServer.h"

MyServer::MyServer() {
}

MyServer::~MyServer() {
}

void MyServer::CreateServer() {
	m_server = new Server(1452, 2);
}

void MyServer::Update() {
	m_server->Update();
	HandleMessages();
}

void MyServer::HandleMessages() {
	RakNet::Packet* packet;

	for (packet = m_server->GetPacket(); packet; m_server->DeallocatePacket(packet), packet = m_server->GetPacket()) {
		switch (packet->data[0]) {
		case ID_NEW_INCOMING_CONNECTION:
			m_server->SendNewClientID(packet->systemAddress);
			SetStartPosition(packet);
			std::cout << "New Connection: Setting ID and Start Position" << std::endl;
			break;
		case ID_CLIENT_SEND_NEW_PIECE:
			//SetNewBlock(packet);
			break;
		case ID_CLIENT_SEND_NEW_POSITION:
			//SetNewPosition(packet);
			break;
		case ID_CLIENT_REQUEST_OTHERS_START_POS:
			SendStartPos(packet);
			break;
		case ID_CLIENT_REQUEST_OTHERS_PIECE:
			//SendOthersCurrentBlock(packet);
			break;
		case ID_CLIENT_READY: /* Tell server this client is ready to begin */
			SetPlayerReady(packet);
			break;

		case ID_CLIENT_SEND_PLAYER_UPDATE:
			UpdatePlayerStats(packet);
			break;

		default:
			break;
		}
	}
}

// Set Clients Start Position
void MyServer::SetStartPosition(RakNet::Packet* packet) {
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_SERVER_SET_PLAYER_START_POS);
	if (m_player1PosSet == false) {
		bs.Write(PLAYER_1_POS_X);
		bs.Write(PLAYER_1_POS_Y);
		m_player1PosX = PLAYER_1_POS_X;
		m_player1PosY = PLAYER_1_POS_Y;


		/*TEST*/
		m_playerOne.startPos.x = PLAYER_1_POS_X;
		m_playerOne.startPos.y = PLAYER_1_POS_Y;
		/*ENDTEST*/

		m_player1PosSet = true;
	} else if (m_player2PosSet == false) {
		bs.Write(PLAYER_2_POS_X);
		bs.Write(PLAYER_2_POS_Y);
		m_player2PosX = PLAYER_2_POS_X;
		m_player2PosY = PLAYER_2_POS_Y;


		/*TEST*/
		m_playerTwo.startPos.x = PLAYER_2_POS_X;
		m_playerTwo.startPos.y = PLAYER_2_POS_Y;
		/*ENDTEST*/

		m_player2PosSet = true;
	}

	m_server->GetPeerInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void MyServer::SetNewBlock(RakNet::Packet* packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientID = 0;
	bs.Read(clientID);

	switch (clientID) {
	case 1: bs.Read((Shapes::BlockType)m_player1BlockType); break;
	case 2: bs.Read((Shapes::BlockType)m_player2BlockType); break;
	default: break;
	}

	printf("Client ID %i: Set piece\n", clientID);
  
	RakNet::BitStream bss;
	bss.Write((RakNet::MessageID)GameMessages::ID_SERVER_SEND_NEW_OTHERS_PIECE);
	bss.Write(clientID);
	switch (clientID) {
	case 1: bss.Write(m_player2BlockType); break;
	case 2: bss.Write(m_player1BlockType); break;
	default: bss.Write(Shapes::BlockType::NONE); break;
	}
  
	m_server->GetPeerInterface()->Send(&bss, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void MyServer::SetNewPosition(RakNet::Packet * packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));


	int clientID = 0;
	bs.Read(clientID);

	switch(clientID) {
	case 1: 
		bs.Read(m_player1PosX);
		bs.Read(m_player1PosY);
		
		break;
	case 2:
		bs.Read(m_player2PosX);
		bs.Read(m_player2PosY);

		break;
	default: return; 
	}

	RakNet::BitStream bss;
	bss.Write((RakNet::MessageID)GameMessages::ID_SERVER_SEND_NEW_OTHERS_POSITION);
	bss.Write(clientID);

	if(clientID == 1) {
		bss.Write(m_player1PosX);
		bss.Write(m_player1PosY);
	} else if(clientID == 2) {
		bss.Write(m_player2PosX);
		bss.Write(m_player2PosY);
	}

	m_server->GetPeerInterface()->Send(&bss, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

/* Tell server this client is ready to begin */
void MyServer::SetPlayerReady(RakNet::Packet * packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientID = 0;
	bs.Read(clientID);
	switch(clientID) {
	case 1: m_player1Ready = true; break;
	case 2: m_player2Ready = true; break;
	default: break;
	}

	printf("Client % i is now ready to play!\n", clientID);

	/* Send packet to clients telling them they are both ready to begin */
	if(m_player1Ready == true && m_player2Ready == true) {
		RakNet::BitStream bss;
		bss.Write((RakNet::MessageID)GameMessages::ID_SERVER_GAME_READY);
		bss.Write(true);
		m_server->GetPeerInterface()->Send(&bss, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		printf("Both Clients are ready to play!\n");
	}
}


void MyServer::SendStartPos(RakNet::Packet* packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientID = 0;
	bs.Read(clientID);

	RakNet::BitStream bss;
	bss.Write((RakNet::MessageID)GameMessages::ID_CLIENT_REQUEST_OTHERS_START_POS);
	bss.Write(clientID);

	switch (clientID) {
	case 1: 
		bss.Write(PLAYER_2_POS_X);
		bss.Write(PLAYER_2_POS_Y);
		break;
	case 2: 
		bss.Write(PLAYER_1_POS_X); 
		bss.Write(PLAYER_1_POS_Y); 
		break;
	default: bss.Write(0); bss.Write(0); break;
	}

	m_server->GetPeerInterface()->Send(&bss, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void MyServer::SendOthersCurrentBlock(RakNet::Packet * packet) {
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));
	int clientID = 0;
	bs.Read(clientID);

	RakNet::BitStream bss;
	bss.Write((RakNet::MessageID)GameMessages::ID_SERVER_SEND_NEW_OTHERS_PIECE);
	bss.Write(clientID);

	switch(clientID) {
	case 1: bss.Write((int)m_player2BlockType); break;
	case 2: bss.Write((int)m_player1BlockType); break;
	default: break;
	}

	m_server->GetPeerInterface()->Send(&bss, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}


// TESTING
void MyServer::UpdatePlayerStats(RakNet::Packet * packet) {
	RakNet::BitStream out;
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientID = 0;
	bs.Read(clientID);

	out.Write((RakNet::MessageID)GameMessages::ID_SERVER_SEND_PLAYER_UPDATE);
	out.Write(clientID);

	switch(clientID) {
	case 1: 
	{
		(Shapes::BlockType)bs.Read(m_playerOne.piece);
		bs.Read(m_playerOne.currPos.x);
		bs.Read(m_playerOne.currPos.y);
		bs.Read(m_playerOne.currRotate);

		out.Write((int)m_playerOne.piece);
		out.Write(m_playerOne.currPos.x);
		out.Write(m_playerOne.currPos.y);
		out.Write(m_playerOne.currRotate);

		break;
	}
	case 2: 
	{
		(Shapes::BlockType)bs.Read(m_playerTwo.piece);
		bs.Read(m_playerTwo.currPos.x);
		bs.Read(m_playerTwo.currPos.y);
		bs.Read(m_playerTwo.currRotate);

		out.Write((int)m_playerTwo.piece);
		out.Write(m_playerTwo.currPos.x);
		out.Write(m_playerTwo.currPos.y);
		out.Write(m_playerTwo.currRotate);

		break;
	}
	default: return;
	}

	m_server->GetPeerInterface()->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
