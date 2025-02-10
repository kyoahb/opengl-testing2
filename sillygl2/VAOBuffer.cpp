#include "VAOBuffer.h"
#include <algorithm>
#include <cassert>

VAOBuffer::VAOBuffer() {
    glGenVertexArrays(1, &vaoId);
}

std::vector<std::shared_ptr<BaseBuffer>>* VAOBuffer::getBuffers() {
    return &buffers;
}

BaseBuffer* VAOBuffer::getBuffer(const std::string& name) const {
    for (auto& buffer : buffers) {
        if (buffer->getName() == name) {
            return buffer.get();
        }
    }
    return nullptr;
}

std::vector<unsigned int> VAOBuffer::getComponentSizes(std::vector<unsigned int> offsets, unsigned int totalSize) const {
    std::vector<unsigned int> sizes;
    if (offsets.size() == 1) {
        sizes.push_back(totalSize);
        return sizes;
    }
    for (size_t i = 1; i < offsets.size(); i++) {
        sizes.push_back(offsets[i] - offsets[i - 1]);
    }
    sizes.push_back(totalSize - offsets.back());

    for (unsigned int size : sizes) {
        assert(size > 0 && "Buffer <T> exception when setting up attrib pointers: component size must be greater than 0 : likely inputted incorrect offsets and incorrect totalSize to match");
    }

    return sizes;
}

void VAOBuffer::addAttribPointers(std::vector<unsigned int> offsets, unsigned int totalSize, GLenum type, bool normalized) {
    bind();
    assert(!offsets.empty() && "Buffer <T> exception when setting up attrib pointers: number of components must be greater than 0");
    assert(offsets[0] == 0 && "Buffer <T> exception when setting up attrib pointers: first offset must be 0");

    // Get sizes, given offsets
    std::vector<unsigned int> componentSizes = getComponentSizes(offsets, totalSize);

    unsigned int totalComponents = static_cast<unsigned int>(offsets.size());
	unsigned int indexOffset = static_cast<unsigned int>(attributeSizeSplit.size()); // Offset from already preset attrib pointers, preventing overwriting

    for (unsigned int i = 0; i < totalComponents; i++) {
        glVertexAttribPointer(indexOffset + i, totalComponents, type, normalized, totalSize, reinterpret_cast<void*>(static_cast<uintptr_t>(offsets[i])));
        glEnableVertexAttribArray(indexOffset + i);
    }
    attributeSizeSplit.insert(attributeSizeSplit.end(), componentSizes.begin(), componentSizes.end()); // Keep note in attributeSizeSplit
}

void VAOBuffer::bind() const {
    glBindVertexArray(vaoId);
}

void VAOBuffer::bindBuffers() const {
    for (auto& buffer : buffers) {
        buffer->bind();
    }
}
