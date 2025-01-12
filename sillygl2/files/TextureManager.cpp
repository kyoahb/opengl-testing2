#include "TextureManager.h"

TextureManager::TextureManager() : textures({}) {

}

Texture* TextureManager::createTexture(TextureType _type, std::string _path) {

	// Check if texture already exists. If it does, no need to remake it.
	for (Texture* texture : textures) {
		if (texture->path == _path) {
			return texture;
		}
	}

	// If texture does not exist, it must be created.
	Texture* texture = new Texture(_type, _path);
	textures.push_back(texture);
	return texture;
}