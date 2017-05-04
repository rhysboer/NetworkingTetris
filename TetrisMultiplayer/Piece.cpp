#include "Piece.h"

Piece::Piece(sf::Vector2f position, Colour texture, CurrentStatus status) {
	m_currentColour = Colour::NONE;
	m_status = status;
	LoadNewTexture(texture);

	m_sprite.setPosition(position);
}

Piece::~Piece() {
}

/*

b = 1
z = 2
[ ][ ][ ][z] 
[ ][ ][z][z]
[ ][ ][b][z] << = CurrColour : B
[b][b][b][ ]	= PrevColour : None
		

[ ][ ][ ][ ] 
[ ][ ][ ][z]
[ ][ ][z][z] << = CurrColour : Z
[b][b][b][z]	= PrevColour : B


[ ][ ][ ][ ]
[ ][ ][ ][z]
[ ][ ][ ][z] << = CurrColour : None
[b][b][b][z]	= PrevColour : B

[ ][ ][ ][ ]
[ ][ ][ ][z]
[ ][ ][b][z] << = CurrColour : B
[b][b][b][z]	= PrevColour : None
*/

void Piece::ChangeColour(Colour colour) {
	//if(m_status == CurrentStatus::OTHER_P)
		if(colour != m_currentColour &&	colour != Colour::EMPTY)
			m_prevColour = m_currentColour;

	//if(m_status == CurrentStatus::OTHER_P) {
		if(colour == Colour::EMPTY) {
			if(m_prevColour != Colour::EMPTY) {
				m_currentColour = m_prevColour;
				m_prevColour = Colour::EMPTY;
			}
		}
	//}

	LoadNewTexture(colour);
}

void Piece::Draw(sf::RenderWindow &render) {
	render.draw(m_sprite);
}

void Piece::SetStatus(CurrentStatus status) {
	m_status = status;
}

CurrentStatus Piece::GetStatus() {
	return m_status;
}

sf::Vector2f Piece::GetPosition() {
	return m_sprite.getPosition();
}

void Piece::LoadNewTexture(Colour colour) {
	if (colour == m_currentColour)
		return;

	if (m_sprite.getTexture() != nullptr)
		delete m_sprite.getTexture();

	m_currentColour = colour;

	sf::Texture* newSprite = new sf::Texture;
	std::string fileName = "bin/textures/";

	switch (colour) {
	case Colour::EMPTY:		fileName += "empty.png"; break;
	case Colour::GREEN:		fileName += "green.png"; break;
	case Colour::RED:		fileName += "red.png"; break;
	case Colour::BLUE:		fileName += "blue.png"; break;
	case Colour::CYAN:		fileName += "cyan.png"; break;
	case Colour::PURPLE:	fileName += "purple.png"; break;
	case Colour::YELLOW:	fileName += "yellow.png"; break;
	case Colour::ORANGE:	fileName += "orange.png"; break;
	default:				fileName += "error.png"; break; 
	}

	newSprite->loadFromFile(fileName);
	m_sprite.setTexture(*newSprite);
}
