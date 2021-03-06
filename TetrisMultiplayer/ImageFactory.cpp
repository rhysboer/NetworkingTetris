#include "ImageFactory.h"

ImageFactory* ImageFactory::m_instance = nullptr;

ImageFactory::ImageFactory() {
}

ImageFactory::~ImageFactory() {
}

ImageFactory* ImageFactory::Init() {
	if(m_instance == nullptr)
		m_instance = new ImageFactory();
	return m_instance;
}

// Load in all the textures
void ImageFactory::LoadInTextures() {
	if(m_instance->m_loaded == true) return;

	sf::String path = "bin/textures/";

	for(int i = 0; i < 8; i++)
		m_instance->m_textures[i] = new sf::Texture;

	m_instance->m_textures[0]->loadFromFile(path + "empty.png");
	m_instance->m_textures[1]->loadFromFile(path + "green.png");
	m_instance->m_textures[2]->loadFromFile(path + "red.png");
	m_instance->m_textures[3]->loadFromFile(path + "blue.png");
	m_instance->m_textures[4]->loadFromFile(path + "cyan.png");
	m_instance->m_textures[5]->loadFromFile(path + "purple.png");
	m_instance->m_textures[6]->loadFromFile(path + "yellow.png");
	m_instance->m_textures[7]->loadFromFile(path + "orange.png");

	m_instance->m_loaded = true;
}

sf::Texture* ImageFactory::GetTexture(Colour colour) {
	if(m_instance->m_loaded == false)
		m_instance->LoadInTextures();

	switch(colour) {
	case Colour::EMPTY:	 return m_instance->m_textures[0];
	case Colour::GREEN:	 return m_instance->m_textures[1];
	case Colour::RED:	 return m_instance->m_textures[2];
	case Colour::BLUE:	 return m_instance->m_textures[3];
	case Colour::CYAN:	 return m_instance->m_textures[4];
	case Colour::PURPLE: return m_instance->m_textures[5];
	case Colour::YELLOW: return m_instance->m_textures[6];
	case Colour::ORANGE: return m_instance->m_textures[7];
	default: return nullptr;
	}
}

void ImageFactory::Destroy() {
	for(int i = 0; i < 8; i++) {
		if(m_instance->m_textures[i] != nullptr) {
			delete m_instance->m_textures[i];
			m_instance->m_textures[i] = nullptr;
		}
	}
}
