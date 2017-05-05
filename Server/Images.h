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

class Images {
public:
	~Images();

	static Images* Init();

	static void LoadInTextures();
	static sf::Texture* GetTexture(Colour colour);

	static void Destroy();

private:
	Images();

	static Images* m_instance;

	sf::Texture* m_textures[8] = { nullptr };
};

