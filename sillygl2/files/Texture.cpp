#include "Texture.h"

Texture::Texture() : id(), type(), path(), height(0), width(0) {
	glGenTextures(1, &id);
}

Texture::Texture(TextureType _type, std::string _path)
	: id(), type(_type), path(_path), height(0), width(0)
{
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// Texture parameters
	// Texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Mipmapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int _width, _height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &nrChannels, 4);
	if (!data) {
		ASSERT_LOG(false, "Failed to load texture: " + path);
		return;
	}

	// Now that texture has loaded, so width and height are known
	width = _width;
	height = _height;

	spdlog::info("Loaded texture: " + path);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::use(unsigned int offset) {
	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D, id);
}