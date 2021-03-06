#pragma once
#include "ImageFactory.h"
#include "SFML\Graphics.hpp"

enum class CurrentStatus {
	EMPTY, /* - Completely Empty Piece*/
	FILLED, /* - Random Block Piece */
	PLAYER, /* - This Player */
	OTHER_P, /* - Another Player */
};

/*
namespace ShapeColour {
	enum class Colour {
		NONE,	// No texture at all!
		EMPTY,	// Empty Square (gray box/outline)
		GREEN,
		RED,
		BLUE,
		CYAN,
		PURPLE,
		YELLOW,
		ORANGE
	};
}
*/

//using namespace ShapeColour;

class Piece {
public:
	Piece(sf::Vector2f position, Colour texture, CurrentStatus status);
	~Piece();

	//test
	void FillPiece(Colour colour);
	void EmptyPiece();

	void ChangeColour(Colour texture);
	void Draw(sf::RenderWindow &render);
	void TempDraw(sf::RenderWindow &render, Colour colour);

	void SetStatus(CurrentStatus status);

	Colour GetPrevColour();

	CurrentStatus GetStatus();
	sf::Vector2f GetPosition();
private:

	void LoadNewTexture(Colour texture);

	Colour m_currentColour;
	Colour m_prevColour;
	
	sf::Sprite m_sprite;

	CurrentStatus m_status = CurrentStatus::EMPTY;
	CurrentStatus m_prevStatus = CurrentStatus::EMPTY;
};

