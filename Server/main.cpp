#include <iostream>
#include "MyServer.h"

#define PORT 1452
#define MAX_PLAYERS 2

void main() {
	MyServer* server = new MyServer();
	server->CreateServer();
	
	while (1) {
		server->Update();
	}

	delete server;
}