#pragma once
#include "SFML\Graphics.hpp"

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

class ImageFactory {
public:
	~ImageFactory();

	static ImageFactory* Init();

	static void LoadInTextures();
	static sf::Texture* GetTexture(Colour colour);

	// Delete all
	static void Destroy();

private:
	ImageFactory();

	static ImageFactory* m_instance;

	sf::Texture* m_textures[8] = { nullptr };

	bool m_loaded = false;
};

