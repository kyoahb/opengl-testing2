#ifndef BUFFER_INL
#define BUFFER_INL

#include "Buffer.h"

template <typename T>
Buffer<T>::Buffer(GLenum _bufferType, std::string _name) : bufferType(_bufferType), name(_name) {
	// Create buffer
	glGenBuffers(1, &bufferId);
	size.push_back((unsigned int)sizeof(T)); // The buffer size is the size of the type, not separated by anything
}

template <typename T>
void Buffer<T>::bind() const {
	glBindBuffer(bufferType, bufferId);
}

template <typename T>
void Buffer<T>::reserveAdditional(unsigned int items) {
	resize(data.size() + items);
	data.reserve(items);
}

template <typename T>
void Buffer<T>::reserveReplace(unsigned int items) {
	resize(items);
	std::vector<T> copyData = std::copy(data.begin(), data.begin()+items, copyData.begin());
	data = copyData;
}

template <typename T>
void Buffer<T>::bufferData(const std::vector<T>& _data, GLenum usage) {
	spdlog::info("Buffering data of size {}, name {}", _data.size(), name);

	bind();
	this->data = _data;
	if (_data.size() == 0) {
		glBufferData(bufferType, 0, nullptr, usage); // Clears buffer
	}
	else {
		glBufferData(bufferType, data.size() * sizeof(T), data.data(), usage);
	}
}

template <typename T>
void Buffer<T>::bufferSubData(size_t offset, const T* _data) {
	spdlog::info("Sub-buffering data of size {}, name {}", sizeof(T), name);
	bind();
	
	// Insert data into the buffer at appropriate index
	unsigned int index = offset / sizeof(T);
	data.insert(data.begin() + index, _data, _data + sizeof(T));
	glBufferSubData(bufferType, offset, sizeof(T), _data);
}

template <typename T>
void Buffer<T>::bufferSubDataIndex(unsigned int index, const T* _data) {
	size_t offset = index * sizeof(T);
	bufferSubData(offset, _data);
}

template <typename T>
void Buffer<T>::resize(unsigned int items) {
	// Generate new buffer
	GLuint newBufferId;
	glGenBuffers(1, &newBufferId);
	glBindBuffer(bufferType, newBufferId);
	glBufferData(bufferType, items * sizeof(T), nullptr, GL_STATIC_DRAW); // Reserve new data space

	// Copy data from the old buffer to the new buffer
	glBindBuffer(GL_COPY_READ_BUFFER, bufferId);
	glBindBuffer(GL_COPY_WRITE_BUFFER, newBufferId);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, data.size()+1 * sizeof(T));

	// Delete the old buffer and update the modelBuffer to the new buffer
	glDeleteBuffers(1, bufferId);
	bufferId = newBufferId;

	// Unbind the buffers
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(bufferType, 0);
}

template <typename T>
GLint Buffer<T>::getBufferSize() const {
	bind();
	GLint bufferSize;
	glGetBufferParameteriv(bufferType, GL_BUFFER_SIZE, &bufferSize);
	return bufferSize;
}

template <typename T>
std::vector<T> Buffer<T>::getBufferData() const {
	bind();

	// Check that the buffer sizes match expected buffer size
	GLint bufferSize = getBufferSize();
	unsigned int expectedBufferSize = data.size() * sizeof(T);
	ASSERT_LOG(bufferSize == expectedBufferSize, "Buffer<T> exception when getting buffer data: buffer data does not match expected size"); // Ensure sizes match

	glFinish();

	T* bufferDataPtr = static_cast<T*>(glMapBuffer(bufferType, GL_READ_ONLY));
	std::vector<T> bufferData;
	if (bufferDataPtr) {
		// Initialise vector from data. If data.size() is inaccurate , this will cause a crash
		bufferData = std::vector<T>(bufferDataPtr, bufferDataPtr + (bufferSize / sizeof(T)));
		glUnmapBuffer(bufferType);
	}
	return bufferData;

}

template <typename T>
const std::vector<T>& Buffer<T>::getData() const {
	return data;
}

template <typename T>
const std::string& Buffer<T>::getName() const {
	return name;
}



#endif