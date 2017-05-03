#include <iostream>
#include <time.h>
#include <thread>
#include "Application.h"
#include "DeltaTime.h"
#include "SFML\Main.hpp"

#include "MyClient.h"

std::thread* thread;

void ConnectToServer();

void main() {
	srand(time(NULL));
	sf::Clock clock;

	// Connect to Server
	ConnectToServer();

	BaseApplication* app = new Application();
	bool run = false;
	app->OnStartUp();



	bool deletethis = false;

	while (run == false) {
		// Update Delta Time
		sf::Time time = clock.restart();
		DeltaTime::UpdateTime(time.asSeconds());


		run = app->Update();
		app->Draw();
	}

	app->OnShutDown();

	thread->join();
	MyClient::Destroy();
	delete thread;
	delete app;
	return;
}

void ConnectToServer() {
	MyClient::Init();

	if (MyClient::GetClient()->ConnectToServer("127.0.0.1", 1452)) {
		thread = new std::thread([&]() {
			while (1) {
				MyClient::GetClient()->Update();
				MyClient::Update();
			}
		});
	}
}

/*
- What will the server and clients need to know about the game

	Server:
		- Send Players Starting Pos
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