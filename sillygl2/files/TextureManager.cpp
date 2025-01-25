#include "TextureManager.h"

std::vector<Texture*> TextureManager::textures = {};

Texture* TextureManager::getTextureByPath(std::string _path) {
	for (Texture* texture : textures) {
		if (texture->path == _path) {
			return texture;
		}
	}
	return nullptr;
}

Texture* TextureManager::createTexture(TextureType _type, std::string _path) {

	// Check if texture already exists. If it does, no need to remake it.
	Texture* existingTexture = getTextureByPath(_path);
	if (existingTexture != nullptr) {
		return existingTexture;
	}

	// If texture does not exist, it must be created.
	Texture* texture = new Texture(_type, _path);
	textures.push_back(texture);
	return texture;
}

Texture* TextureManager::defaultTexture(TextureType _type) {
	std::string fake_path = "";
	unsigned char pixels[3];

	switch (_type) {
	case TextureType::Diffuse:
		fake_path = "DEFAULT_DIFFUSE";
		pixels[0] = 255;
		pixels[1] = 255;
		pixels[2] = 255;
		break;
	case TextureType::Specular:
		fake_path = "DEFAULT_SPECULAR";
		pixels[0] = 0;
		pixels[1] = 0;
		pixels[2] = 0;
		break;
	case TextureType::Normal:
		fake_path = "DEFAULT_NORMAL";
		pixels[0] = 128;
		pixels[1] = 128;
		pixels[2] = 255;
		break;
	default:
		ASSERT_LOG(false, "Invalid texture type");
		return nullptr;
	}

	// Check if default texture already exists. If it does, no need to remake it.
	Texture* existingTexture = getTextureByPath(fake_path);
	if (existingTexture != nullptr) { // If it DOES already exist, return it.
		return existingTexture;
	}

	// texture setup
	Texture* tex = new Texture();
	glBindTexture(GL_TEXTURE_2D, tex->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// attributes
	tex->path = fake_path;
	tex->type = _type;
	tex->width = 1;
	tex->height = 1;

	textures.push_back(tex);
	return tex;
}