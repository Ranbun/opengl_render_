#include "bufferobject.h"

#include <cassert>
#include <glad/glad.h>
#include "enum.h"

BufferObject::BufferObject()
    : m_bufferType(object_type::vertex_buffer)
      , m_isCreate(false)
      , m_count(0)
{
    bufferType();
}

BufferObject::BufferObject(const object_type type)
    : m_bufferType(type)
      , m_isCreate(false)
      , m_count(0)
{
    bufferType();
}

BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &m_object);
}

void BufferObject::allocate(const void* data, const unsigned count) const
{
    glBufferData(m_currentBuffer, count, data, GL_STATIC_DRAW);
}

void BufferObject::allocate(const unsigned count)
{
    allocate(nullptr, count);
    m_count = count;
}

bool BufferObject::bind() const
{
    if (m_isCreate)
    {
        glBindBuffer(m_currentBuffer, m_object);
    }
    return m_isCreate;
}

unsigned int BufferObject::bufferId() const
{
    return m_object;
}

bool BufferObject::create()
{
    glGenBuffers(1, &m_object);
    m_isCreate = true;
    return m_isCreate;
}

void BufferObject::destroy()
{
    glDeleteBuffers(1, &m_object);
    m_isCreate = false;
}

bool BufferObject::isCreate() const
{
    return m_isCreate;
}

void* BufferObject::map(const object_access access) const
{
    GLenum mapAccess = GL_WRITE_ONLY;
    switch (access)
    {
    case object_access::read_only:
        mapAccess = GL_READ_ONLY;
        break;
    case object_access::write_only:
        mapAccess = GL_WRITE_ONLY;
        break;
    case object_access::read_write:
        mapAccess = GL_READ_WRITE;
        break;
    }

    const auto dataPtr = glMapBuffer(m_currentBuffer, mapAccess);

    return dataPtr;
}

void* BufferObject::mapRange(unsigned offset, unsigned count, object_access_flags flags)
{
    assert(this);
    return nullptr;
}

bool BufferObject::read(int offset, void* data, unsigned count)
{
    assert(this);
    return false;
}

void BufferObject::release() const
{
    glBindBuffer(m_currentBuffer, 0);
}

size_t BufferObject::size() const
{
    return m_count;
}

object_type BufferObject::type() const
{
    return m_bufferType;
}

bool BufferObject::unMap() const
{
    assert(this);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return true;
}

void BufferObject::write(unsigned int offset, const void* data, unsigned count)
{
    assert(this);
}

void BufferObject::bufferType()
{
    switch (m_bufferType)
    {
    case object_type::index_buffer:
        m_currentBuffer = GL_ELEMENT_ARRAY_BUFFER;
        break;
    case object_type::vertex_buffer:
        m_currentBuffer = GL_ARRAY_BUFFER;
        break;
    }
}
