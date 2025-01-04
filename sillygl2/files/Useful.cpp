#include "Useful.h"

// Returns rotation matrix for a vec3 rotation
glm::mat4& mat4Rotate(glm::vec3 rotation) {
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    return rotationMatrix;
}

//Rotates a vector by a given rotation about the origin
glm::vec3& vec3Rotate(glm::vec3 rotation, glm::vec3 original) {
    // Combine rotations
    glm::mat4 rotationMatrix = mat4Rotate(rotation);

    original = rotationMatrix * glm::vec4(original, 0.0f);
    original = glm::vec3(original);
    return original;
}

//Rotates a vector by a given rotation about a given point
glm::vec3& vec3RotateAroundPoint(glm::vec3 rotation, glm::vec3 point, glm::vec3 original) {
    // Translate the point to the origin
    glm::vec3 translated = original - point;

    // Combine rotations
    glm::mat4 rotationMatrix = mat4Rotate(rotation);

    translated = rotationMatrix * glm::vec4(translated, 0.0f);
    translated = glm::vec3(translated);

    // Translate the point back
    glm::vec3 result = translated + point;

    return result;
}

// Force a vector to be within a range / return min if below min, max if above max
glm::vec3& vec3Clamp(glm::vec3 vector, float min, float max) {
    glm::vec3 v = glm::vec3(glm::clamp(vector.x, min, max), glm::clamp(vector.y, min, max), glm::clamp(vector.z, min, max));
	return v;
}

// Force a float to be within range, but overfill excess. EG: if value = 11, min is 0, max is 5 -> returns 1
float fOverfill(float value, float min, float max) { 
	while (value < min) value += (max - min);
	while (value > max) value -= (max - min);
	return value;
}

// Force a vector of floats to be within range, but overfill excess. EG: if value = 11, min is 0, max is 5 -> returns 1
glm::vec3& vec3Overfill(glm::vec3 vector, float min, float max) {
    glm::vec3 v = glm::vec3(fOverfill(vector.x, min, max), fOverfill(vector.y, min, max), fOverfill(vector.z, min, max));
    return v;
}

void vec3Print(glm::vec3 vector, std::string name) {
    std::cout << name << " X IS" << vector.x << " AND Y IS " << vector.y << " AND Z IS " << vector.z << std::endl;
}

// Returns a random float value between a and b
float rand_float(float a, float b)
{
    if (a > b) std::swap(a, b);
    return a + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (b - a)));
}

// Returns a vector of random float values between a and b
glm::vec3 rand_vec3(float a, float b) {
	return glm::vec3(rand_float(a, b), rand_float(a, b), rand_float(a, b));
}