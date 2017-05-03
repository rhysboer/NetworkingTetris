#pragma once
#include "MessageIdentifiers.h"

enum GameMessages {
	ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1,
	ID_SERVER_SET_CLIENT_ID,
	ID_CLIENT_CLIENT_DATA,
	/* Add Here */

	ID_SERVER_SET_PLAYER_START_POS,
	ID_CLIENT_SEND_NEW_PIECE,		/* Client Send Server New Block */
	ID_CLIENT_SEND_NEW_POSITION,	/* Client Send Server New Block Position */
	ID_CLIENT_SEND_NEW_ROTATION,	/* Client Send Server New Block Rotation */
	ID_CLIENT_REQUEST_OTHERS_START_POS, /* Client Request Other Players Starting Position */
	ID_CLIENT_REQUEST_OTHERS_POSITION,	/* Client Request Other Players Current Position */
	ID_CLIENT_REQUEST_OTHERS_PIECE,	/* Client Request Other Players Current Piece */
	ID_CLIENT_READY,				/* Tell Server Client Is Ready To Play */

	ID_SERVER_SEND_NEW_OTHERS_PIECE,	/* Server Send Client Other Players New Block */
	ID_SERVER_SEND_NEW_OTHERS_POSITION,	/* Server Send Client Other Players New Position */
	ID_SERVER_SEND_OTHERS_START_POS,	/* Other Players Start Pos */
	ID_SERVER_SEND_OTHERS_PIECE,		/* Other Players Current Piece (Block)*/
	ID_SERVER_GAME_READY,				/* Game Is Ready To Start*/

	// TEST
	ID_CLIENT_SEND_PLAYER_UPDATE,
	ID_SERVER_SEND_PLAYER_UPDATE,
};

/*
- What will the server and clients need to know about the game

Server:
- Both Players Positions
- Both Players Current Shape
- Start Position for both players

Client:
- Board Pieces
- Players Position
- Other Players Position
- Other Players Shape
- Other Players Start Pos

Maybe:
- Check for Server Board Collisions as well as client collisions
- (Server) Holds Data of the Board Pieces (MAYBE)


Wait till both clients are connected to the server before starting
both clients should have each others new piece & starting Position


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