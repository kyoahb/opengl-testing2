#pragma once
#define GLM_FORCE_SSE2         // Enable SSE2
#define GLM_FORCE_SSE3         // Enable SSE3
#define GLM_FORCE_SSSE3        // Enable SSSE3
#define GLM_FORCE_SSE4_1       // Enable SSE4.1
#define GLM_FORCE_SSE4_2       // Enable SSE4.2
#define GLM_FORCE_AVX          // Enable AVX
#define GLM_FORCE_AVX2         // Enable AVX2
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES  // Align types to 16 bytes
#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <string>
#include "Vertex.h"
#include "Texture.h"
#include "Shader_l.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>

class InstanceGroup;

class Instance {
public:
	bool requestingUpdate = false; // Whether any transformations have been applied to the instance this frame, which should be reflected in the model matrix.

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	unsigned int instanceID;
	unsigned int id;
	std::string name;
	

	glm::mat4 model = glm::mat4(1.0f); 
	Instance(glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _rotation = glm::vec3(0.0f), glm::vec3 _scale = glm::vec3(1.0f), std::string _name = "");

	void move(glm::vec3 change);
	void rotateEuler(glm::vec3 _rotation);
	void rotateQuat(glm::quat _rotation);
	void addScale(glm::vec3 _scale);

	glm::mat4 getModelMatrix();
	void formModelMatrix();

private:
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	glm::vec3 lastPosition = glm::vec3(0.0f);
	glm::quat lastRotation;
	glm::vec3 lastScale = glm::vec3(1.0f);
};