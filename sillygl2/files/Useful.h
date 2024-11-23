#pragma once

#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#define GLM_FORCE_ALIGNED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//Rotates a vector by a given rotation about the origin
glm::vec3 vec3Rotate(glm::vec3 rotation, glm::vec3 original);

//Rotates a vector by a given rotation about a given point
glm::vec3 vec3RotateAroundPoint(glm::vec3 rotation, glm::vec3 point, glm::vec3 original);

glm::vec3 vec3Clamp(glm::vec3 vector, float clamp);

void vec3Print(glm::vec3 vector, std::string name);

float rand_float(float a, float b);