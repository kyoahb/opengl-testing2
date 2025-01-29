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
#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "Log.h"



void assert_log(bool condition, const std::string& message, const char* file, int line);

// Only use assert log if in debug mode
#ifndef NDEBUG
#define ASSERT_LOG(condition, message) assert_log(condition, message, __FILE__, __LINE__)
#else
#define ASSERT_LOG(condition, message) ((void)0)
#endif

template <typename T>
void batchBuffer(GLenum bufferType, unsigned int buffer, const std::vector<T>& dataList, GLenum usage) {
    ASSERT_LOG(bufferType == GL_ARRAY_BUFFER || bufferType == GL_ELEMENT_ARRAY_BUFFER, "Attempted to batch invalid buffer type");
    ASSERT_LOG(usage == GL_STATIC_DRAW || usage == GL_DYNAMIC_DRAW || usage == GL_STREAM_DRAW, "Attempted to batch buffer with invalid usage");

    glBindBuffer(bufferType, buffer);
	std::string invalidBuffer = "Attempted to batch invalid buffer " + std::to_string(buffer);
    ASSERT_LOG(glIsBuffer(buffer), invalidBuffer);

    glBufferData(bufferType, dataList.size() * sizeof(T), dataList.data(), usage);

    glBindVertexArray(0);
}

template <typename T>
void singleBuffer(GLenum bufferType, unsigned int buffer, std::size_t start, const T& data, GLenum usage) {
	ASSERT_LOG(bufferType == GL_ARRAY_BUFFER || bufferType == GL_ELEMENT_ARRAY_BUFFER, "Attempted to batch invalid buffer type");
	ASSERT_LOG(usage == GL_STATIC_DRAW || usage == GL_DYNAMIC_DRAW || usage == GL_STREAM_DRAW, "Attempted to batch buffer with invalid usage");

    glBindBuffer(bufferType, buffer);

    std::string invalidBuffer = "Attempted to send over single data to invalid buffer " + std::to_string(buffer);

    ASSERT_LOG(glIsBuffer(buffer), invalidBuffer);

	glBufferSubData(bufferType, start, sizeof(T), &data);
	glBindBuffer(bufferType, 0);
}
void mat4Print(const glm::mat4& matrix);

void vec3Print(const glm::vec3& vector);

// Returns rotation matrix for a vec3 rotation
const glm::mat4& mat4Rotate(const glm::vec3& rotation);

//Rotates a vector by a given rotation about the origin
const glm::vec3& vec3Rotate(const glm::vec3& rotation, const glm::vec3& original);

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