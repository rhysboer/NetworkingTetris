#include "Piece.h"

Piece::Piece(sf::Vector2f position, Colour texture, CurrentStatus status) {
	m_currentColour = Colour::NONE;
	m_status = status;
	LoadNewTexture(texture);

	m_sprite.setPosition(position);
}

Piece::~Piece() {
}

void Piece::ChangeColour(Colour texture) {
	LoadNewTexture(texture);
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

void Piece::LoadNewTexture(Colour texture) {
	if (texture == m_currentColour)
		return;

	if (m_sprite.getTexture() != nullptr)
		delete m_sprite.getTexture();

	m_currentColour = texture;

	sf::Texture* newSprite = new sf::Texture;
	std::string fileName = "bin/textures/";

	switch (texture) {
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
