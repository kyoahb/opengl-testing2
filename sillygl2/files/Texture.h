#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stbimage/stb_image.h"
#include <glm/glm.hpp>
// ID (post-generation), type (diffuse, specular, etc.), and path to image file

enum TextureType {
	Diffuse = 0,
	Specular = 1
};

class Texture {
public:
	unsigned int id;
	TextureType type;
	std::string path;

	Texture(TextureType _type, std::string _path);
	Texture() = default; // Default constructor because it wanted me to make one


};