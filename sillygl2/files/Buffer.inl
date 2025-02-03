#ifndef BUFFER_INL
#define BUFFER_INL

#include "Buffer.h"

template <typename T>
Buffer<T>::Buffer(GLenum _bufferType, std::string _name) : bufferType(_bufferType), name(_name) {
	// Create buffer
	glGenBuffers(1, &bufferId);
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

/*
template <typename T>
void Buffer<T>::bufferData(size_t size, const std::vector<T>* data, GLenum usage) {
	if (bufferType == GL_ELEMENT_ARRAY_BUFFER)
		spdlog::info("Buffering data of size {}, GL_ELEMENT_ARRAY_BUFFER", size);
	else if (bufferType == GL_ARRAY_BUFFER) {
		spdlog::info("Buffering data of size {}, GL_ARRAY_BUFFER", size);
	}
	else {
		spdlog::info("Buffering data of size {}, unknown buffer type", size);
	}

	bind();
	this->data = *data;
	glBufferData(bufferType, size, data->data(), usage);
}*/

template <typename T>
void Buffer<T>::bufferData(size_t size, const std::vector<T>& _data, GLenum usage) {
	if (bufferType == GL_ELEMENT_ARRAY_BUFFER)
		spdlog::info("Buffering data of size {}, GL_ELEMENT_ARRAY_BUFFER, name {}", size, name);
	else if (bufferType == GL_ARRAY_BUFFER) {
		spdlog::info("Buffering data of size {}, GL_ARRAY_BUFFER, name {}", size, name);
	}
	else {
		spdlog::info("Buffering data of size {}, unknown buffer type, name {}", size, name);
	}

	bind();
	this->data = _data;
	if (_data.size() == 0) {
		glBufferData(bufferType, size, nullptr, usage);
	}
	else {
		glBufferData(bufferType, size, this->data.data(), usage);
	}
}

template <typename T>
void Buffer<T>::bufferSubData(size_t offset, size_t size, const T* _data) {
	if (bufferType == GL_ELEMENT_ARRAY_BUFFER)
		spdlog::info("Single Buffering data of size {}, GL_ELEMENT_ARRAY_BUFFER", size);
	else if (bufferType == GL_ARRAY_BUFFER) {
		spdlog::info("Single Buffering data of size {}, GL_ARRAY_BUFFER", size);
	}
	else {
		spdlog::info("Single Buffering data of size {}, unknown buffer type", size);
	}
	bind();
	this->data.insert(this->data.begin() + offset, _data, _data + size);
	glBufferSubData(bufferType, offset, size, _data->data());
}

template <typename T>
void Buffer<T>::bufferSubDataIndex(unsigned int index, size_t size, const T* _data) {
	size_t offset = index * sizeof(T);
	bufferSubData(offset, size, _data);
}

template <typename T>
void Buffer<T>::resize(unsigned int items) {
	// Generate new buffer
	GLuint newBufferId;
	glGenBuffers(1, &newBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, newBufferId);
	glBufferData(GL_ARRAY_BUFFER, items * sizeof(T), nullptr, GL_STATIC_DRAW); // Reserve new data space

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <typename T>
const std::vector<T>& Buffer<T>::getData() const {
	return data;
}


#endif