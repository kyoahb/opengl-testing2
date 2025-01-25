#pragma once
#include "Texture.h"
#include <vector>


class TextureManager {
public:
	static std::vector<Texture*> textures; // List of textures loaded

	static Texture* getTextureByPath(std::string _path); // Returns texture with given path

	static Texture* createTexture(TextureType _type, std::string _path); // Adds texture to list of textures

	static Texture* defaultTexture(TextureType _type); // Returns default texture (diffuse, specular, etc.)
};
