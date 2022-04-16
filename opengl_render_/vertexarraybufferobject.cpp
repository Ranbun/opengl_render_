#include "vertexarraybufferobject.h"

#include <cassert>
#include <glad/glad.h>

VertexArrayObject::VertexArrayObject()
    : m_arrayBuffer(0)
    , m_isCreate(false)
{
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &m_arrayBuffer);
}

void VertexArrayObject::bind() const
{
    if (isCreated())
    {
        glBindVertexArray(m_arrayBuffer);
    }

}

bool VertexArrayObject::create()
{
    if (!m_isCreate)
    {
        glGenVertexArrays(1, &m_arrayBuffer);
        m_isCreate = true;
    }

    return m_isCreate;
}

void VertexArrayObject::destroy()
{
    glDeleteVertexArrays(1, &m_arrayBuffer);
    m_arrayBuffer = 0;
    m_isCreate = false;
}

bool VertexArrayObject::isCreated() const
{
    return m_isCreate;
}

unsigned int VertexArrayObject::objectId() const
{
    return m_arrayBuffer;
}

void VertexArrayObject::release()
{
    assert(this);
    glBindVertexArray(0);
}
