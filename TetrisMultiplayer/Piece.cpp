#include "Piece.h"\

Piece::Piece(sf::Vector2f position, Colour texture, CurrentStatus status) {
	m_currentColour = Colour::NONE;
	m_prevColour = Colour::EMPTY;

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

////if(m_status == CurrentStatus::OTHER_P)
//	if(colour != m_currentColour &&	colour != Colour::EMPTY)
//		m_prevColour = m_currentColour;
//
////if(m_status == CurrentStatus::OTHER_P) {
//	if(colour == Colour::EMPTY) {
//		if(m_prevColour != Colour::EMPTY) {
//			m_currentColour = m_prevColour;
//			m_prevColour = Colour::EMPTY;
//		}
//	}
////}


void Piece::FillPiece(Colour colour) {
	m_currentColour = colour;
	m_status = CurrentStatus::FILLED;
}

void Piece::ChangeColour(Colour colour) {
	if(colour == m_currentColour) return;

	if(m_status == CurrentStatus::OTHER_P)
		if(colour != m_currentColour &&	colour != Colour::EMPTY)
			m_prevColour = m_currentColour;
		else if(colour == Colour::EMPTY) {
			if(m_prevColour != Colour::EMPTY) {
				//m_currentColour = m_prevColour;
				LoadNewTexture(m_prevColour);
				m_prevColour = Colour::EMPTY;
				return;
			}
		}

	LoadNewTexture(colour);
}

void Piece::Draw(sf::RenderWindow &render) {
	render.draw(m_sprite);
}

// Draw for 1 frame only with any color
void Piece::TempDraw(sf::RenderWindow & render, Colour colour) {
	sf::Sprite temp;
	temp.setPosition(m_sprite.getPosition());
	temp.setTexture(*ImageFactory::GetTexture(colour));

	//m_sprite.setTexture(*ImageFactory::GetTexture(colour));
	render.draw(temp);
	//m_sprite.setTexture(*ImageFactory::GetTexture(m_currentColour));
}

void Piece::SetStatus(CurrentStatus status) {
	m_status = status;
}

Colour Piece::GetPrevColour() {
	return m_prevColour;
}

CurrentStatus Piece::GetStatus() {
	return m_status;
}

sf::Vector2f Piece::GetPosition() {
	return m_sprite.getPosition();
}

void Piece::LoadNewTexture(Colour colour) {
	//if (colour == m_currentColour)
	//	return;

	m_sprite.setTexture(*ImageFactory::GetTexture(colour));
	m_currentColour = colour;
}
