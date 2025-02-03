#pragma once
#include "Useful.h"
#include <type_traits>

template <typename T>
class Buffer {
public:
	GLuint bufferId; 
	GLenum bufferType; 

	std::vector<T> data; // raw data

	Buffer(GLenum _bufferType = GL_ARRAY_BUFFER);

	// Make destructor

	void bind() const;
	void reserveAdditional(unsigned int items); // Reserves space for additional items; IE, if current size = 10 and reserveAdditional(5) is called, size will be 15. CHANGES BUFFER ID
	void reserveReplace(unsigned int items); // Reserves space for items (can reduce space); IE if current size = 4 and reserveReplace(2) is called, size will be 2. CHANGES BUFFER ID
	
	void bufferData(size_t size, const std::vector<T>* data, GLenum usage);

	void bufferSubData(size_t offset, size_t size, const T* data);
	void bufferSubDataIndex(unsigned int index, size_t size, const T* data); // Buffer data at a specific index (for example, if you want to update a single vertex in a buffer of vertices

	const std::vector<T>& getData() const;

	void resize(unsigned int items);
};

#include "Buffer.inl" // Needed for type stuff