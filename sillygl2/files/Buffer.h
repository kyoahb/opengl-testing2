#pragma once
#include "Useful.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <type_traits>
#include <glm/gtx/string_cast.hpp> // For debugging
#include <vector>
#include <numeric>

// Base class, typeless
class BaseBuffer {
public:
	virtual ~BaseBuffer() = default;
	virtual void bind() const = 0;
	virtual const std::string& getName() const = 0;
};


template <typename T>
class Buffer : public BaseBuffer{
private:
	void resize(unsigned int items);

public:
	GLuint bufferId; 
	GLenum bufferType; 
	std::string name; // Name of buffer
	std::vector<unsigned int> size; // Size of single element in the buffer in bytes, separated by attribute pointers
	std::vector<T> data; // raw data

	Buffer(GLenum _bufferType = GL_ARRAY_BUFFER, std::string _name = "Unnamed Buffer");

	// Make destructor

	std::vector<T> getBufferData() const; // Get data stored inside buffer in GPU
	GLint getBufferSize() const; // Get size of buffer in bytes
	const std::vector<T>& getData() const; // Get data stored inside buffer class
	const std::string& getName() const; // Get name of buffer

	void bind() const;
	void reserveAdditional(unsigned int items); // Reserves space for additional items; IE, if current size = 10 and reserveAdditional(5) is called, size will be 15. CHANGES BUFFER ID
	void reserveReplace(unsigned int items); // Reserves space for items (can reduce space); IE if current size = 4 and reserveReplace(2) is called, size will be 2. CHANGES BUFFER ID
	
	void bufferData(const std::vector<T>& _data, GLenum usage); // Buffer data into buffer, replacing all current data
	void bufferSubData(size_t offset, const T* _data); // Buffer data at a specific size offset
	void bufferSubDataIndex(unsigned int index, const T* _data); // Buffer data at a specific index (for example, if you want to update a single vertex in a buffer of vertices


};

#include "Buffer.inl" // Needed for type stuff