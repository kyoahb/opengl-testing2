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
#include "Object.h"
#include <glm/gtx/quaternion.hpp>
class Instance : public GameObject {
public:
	bool requestingUpdate = false; // Whether any transformations have been applied to the instance this frame, which should be reflected in the model matrix.
	unsigned int instanceID;
	

	glm::mat4 model = glm::mat4(1.0f); 
	Instance(const std::string& _name = "Unnamed Instance",
		const glm::vec3& _position = glm::vec3(0.0f),
		const glm::quat& _rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
		const glm::vec3& _scale = glm::vec3(1.0f));

	// Overrriden GameObject transforms, with added flags.
	void rotateEuler(const glm::vec3& _rotation);
	void rotateQuat(const glm::quat& _rotation);
	void setRotation(const glm::quat& _rotation);
	void move(const glm::vec3& change);
	void addScale(const glm::vec3& _scale);

	glm::mat4 getModelMatrix();
	void formModelMatrix();

private:
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	// pre set to true to ensure initial matrix calculation is done
	bool positionChanged = true;
	bool rotationChanged = true;
	bool scaleChanged = true;
};