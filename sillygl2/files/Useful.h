#pragma once
#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#define GLM_FORCE_ALIGNED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Returns rotation matrix for a vec3 rotation
glm::mat4& mat4Rotate(glm::vec3 rotation);

//Rotates a vector by a given rotation about the origin
glm::vec3& vec3Rotate(glm::vec3 rotation, glm::vec3 original);

//Rotates a vector by a given rotation about a given point
glm::vec3& vec3RotateAroundPoint(glm::vec3 rotation, glm::vec3 point, glm::vec3 original);

// Force a vector to be within a range / return min if below min, max if above max
glm::vec3& vec3Clamp(glm::vec3 vector, float min, float max);

// Force a float to be within range, but overfill excess. EG: if value = 11, min is 0, max is 5 -> returns 1
float fOverfill(float value, float min, float max);

// Force a vector of floats to be within range, but overfill excess. EG: if value = 11, min is 0, max is 5 -> returns 1
glm::vec3& vec3Overfill(glm::vec3 vector, float min, float max);

void vec3Print(glm::vec3 vector, std::string name);

// Returns a random float value between a and b
float rand_float(float a, float b);

// Returns a vector of random float values between a and b
glm::vec3 rand_vec3(float a, float b);