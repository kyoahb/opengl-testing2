#pragma once
#include <string>



#include "Useful.h"
#include <iostream>
#include <stb_image.h>
#include "Shader_l.h"
// ID (post-generation), type (diffuse, specular, etc.), and path to image file

enum TextureType {
	None = 0,
	Diffuse = 1,
	Specular = 2,
	Normal = 3,
};

class Texture {
public:
	unsigned int id;
	TextureType type;
	std::string path;
	int width;
	int height;

	Texture(TextureType _type, std::string _path); // Constructor that reads image file and sets up texture
	Texture(); // Default constructor that sets up id
	void use(unsigned int offset); // Activates and then binds texture to unit offset. Does not pass itself into shader.


};