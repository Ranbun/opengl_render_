#include "bufferobject.h"

#include <cassert>
#include <glad/glad.h>

#include "enum.h"

BufferObject::BufferObject()
    : buffer_type_(objectBuffer::OBJECT_TYPE::VERTEX_BUFFER)
    , is_create_(false)
    , count_(0)
{
    bufferType();
}

BufferObject::BufferObject(const objectBuffer::OBJECT_TYPE type)
    : buffer_type_(type)
    , is_create_(false)
    , count_(0)
{
    bufferType();
}

BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &object_);
}

void BufferObject::allocate(const void* data, const unsigned count) const
{
    glBufferData(current_buffer_, count, data, GL_STATIC_DRAW);
}

void BufferObject::allocate(const unsigned count)
{
    allocate(nullptr, count);
    count_ = count;
}

bool BufferObject::bind() const
{
    if (is_create_)
    {
        glBindBuffer(current_buffer_, object_);
    }
    return is_create_;
}

unsigned int BufferObject::bufferId() const
{
    return object_;
}

bool BufferObject::create()
{
    glGenBuffers(1, &object_);
    is_create_ = true;
    return is_create_;
}

void BufferObject::destroy()
{
    glDeleteBuffers(1, &object_);
    is_create_ = false;
}

bool BufferObject::isCreate() const
{
    return is_create_;
}

void* BufferObject::map(const objectBuffer::OBJECT_ACCESS access)
{
    GLenum map_access = GL_WRITE_ONLY;
    switch (access)
    {
    case objectBuffer::OBJECT_ACCESS::READ_ONLY:
        map_access = GL_READ_ONLY;
        break;
    case objectBuffer::OBJECT_ACCESS::WRITE_ONLY:
        map_access = GL_WRITE_ONLY;
        break;
    case objectBuffer::OBJECT_ACCESS::READ_WRITE:
        map_access = GL_READ_WRITE;
        break;
    }

    auto data_ptr = glMapBuffer(current_buffer_, map_access);

    return data_ptr;
}

void* BufferObject::mapRange(unsigned offset, unsigned count, objectBuffer::OBJECT_ACCESS_FLAGS flags)
{
    return nullptr;
}

bool BufferObject::read(int offset, void* data, unsigned count)
{
    return false;
}

void BufferObject::release() const
{
    glBindBuffer(current_buffer_, 0);
}

size_t BufferObject::size() const
{
    return count_;
}

objectBuffer::OBJECT_TYPE BufferObject::type() const
{
    return buffer_type_;
}

bool BufferObject::unMap() const
{
    assert(this);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return true;
}

void BufferObject::write(unsigned int offset, const void* data, unsigned count)
{

}

void BufferObject::bufferType()
{
    switch (buffer_type_)
    {
    case objectBuffer::OBJECT_TYPE::INDEX_BUFFER:
        current_buffer_ = GL_ELEMENT_ARRAY_BUFFER;
        break;
    case objectBuffer::OBJECT_TYPE::VERTEX_BUFFER:
        current_buffer_ = GL_ARRAY_BUFFER;
        break;
    }
}
