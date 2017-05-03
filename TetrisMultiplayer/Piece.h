#pragma once
#include "SFML\Graphics.hpp"

enum class CurrentStatus {
	EMPTY, /* - Completely Empty Piece*/
	FILLED, /* - Random Block Piece */
	PLAYER, /* - This Player */
	OTHER_P, /* - Another Player */
};

namespace ShapeColour {
	enum class Colour {
		NONE,
		EMPTY,
		GREEN,
		RED,
		BLUE,
		CYAN,
		PURPLE,
		YELLOW,
		ORANGE
	};
}

using namespace ShapeColour;

class Piece {
public:
	Piece(sf::Vector2f position, Colour texture, CurrentStatus status);
	~Piece();

	void ChangeColour(Colour texture);
	void Draw(sf::RenderWindow &render);
	void SetStatus(CurrentStatus status);

	CurrentStatus GetStatus();
	sf::Vector2f GetPosition();
private:

	void LoadNewTexture(Colour texture);

	sf::Sprite m_sprite;

	Colour m_currentColour;
	CurrentStatus m_status = CurrentStatus::EMPTY;
};

