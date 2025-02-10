#ifndef VAO_INL
#define VAO_INL

#include "VAOBuffer.h"

template <typename T>
void VAOBuffer::addBuffer(Buffer<T>& buffer) {
    bind();
    buffer.bind();
    buffers.push_back(std::make_shared<Buffer<T>>(buffer));
}

#endif
