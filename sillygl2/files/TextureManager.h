#pragma once
#include "Texture.h"
#include <vector>


class TextureManager {
public:
	std::vector<Texture*> textures; // List of textures loaded

	TextureManager();

	Texture* createTexture(TextureType _type, std::string _path); // Adds texture to list of textures
};
