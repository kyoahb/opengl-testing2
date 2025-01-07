#pragma once
#include <string>

// ID (post-generation), type (diffuse, specular, etc.), and path to image file
struct Texture {
	unsigned int id;
	std::string type;
	std::string imagePath;
};