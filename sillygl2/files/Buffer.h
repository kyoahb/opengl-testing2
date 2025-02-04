#pragma once
//#include "Useful.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <type_traits>
#include <glm/gtx/string_cast.hpp> // For debugging

template <typename T>
class Buffer {
public:
	GLuint bufferId; 
	GLenum bufferType; 
	std::string name; // Name of buffer

	std::vector<T> data; // raw data

	Buffer(GLenum _bufferType = GL_ARRAY_BUFFER, std::string _name = "Unnamed Buffer");

	// Make destructor

	void bind() const;
	void reserveAdditional(unsigned int items); // Reserves space for additional items; IE, if current size = 10 and reserveAdditional(5) is called, size will be 15. CHANGES BUFFER ID
	void reserveReplace(unsigned int items); // Reserves space for items (can reduce space); IE if current size = 4 and reserveReplace(2) is called, size will be 2. CHANGES BUFFER ID
	
	void bufferData(const std::vector<T>& _data, GLenum usage);
	void bufferSubData(size_t offset, const T* _data);
	void bufferSubDataIndex(unsigned int index, const T* _data); // Buffer data at a specific index (for example, if you want to update a single vertex in a buffer of vertices


	std::vector<T> getBufferData() const; // Get data stored inside buffer

	const std::vector<T>& getData() const;

	void resize(unsigned int items);
};

#include "Buffer.inl" // Needed for type stuff