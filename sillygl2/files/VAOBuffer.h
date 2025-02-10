#pragma once
#include "Buffer.h"
#include "Useful.h"
#include <variant>
#include <algorithm>

class VAOBuffer {
private:
	std::vector<unsigned int> getComponentSizes(std::vector<unsigned int> offsets, unsigned int totalSize) const; // (Roughly) get sizes from component offsets

public:
	std::vector<std::shared_ptr<BaseBuffer>> buffers;
	std::vector<unsigned int> attributeSizeSplit; // Attribute locations in terms of sizes (bytes).
	GLuint vaoId;

	VAOBuffer();

	std::vector<std::shared_ptr<BaseBuffer>>* getBuffers();
	BaseBuffer* getBuffer(const std::string& name) const;

	template <typename T>
	void addBuffer(Buffer<T>& buffer); // Add buffer to VAO

	void addAttribPointers(std::vector<unsigned int> offsets, unsigned int totalSize, GLenum type, bool normalized); // Add Attrib Pointers to VAO

	void bind() const;
	void bindBuffers() const;
};

#include "VAOBuffer.inl"