#include "bufferobject.h"

#include <glad/glad.h>

BufferObject::BufferObject()
{
	buffer_type_ = object_buffer::OBJECT_TYPE::VERTEX_BUFFER;
	object_ = 0;
	count_ = 0;
	is_create_ = false;
	bufferType();
}

BufferObject::BufferObject(const object_buffer::OBJECT_TYPE type)
{
	buffer_type_ = type;
	is_create_ = false;
	count_ = 0;
	bufferType();
}

BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &object_);
}

void BufferObject::allocate(const void* data, const unsigned count) const
{
	// bind();
	glBufferData(current_buffer_, count, data, GL_STATIC_DRAW);
	// release();
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

void* BufferObject::map(object_buffer::OBJECT_ACCESS access)
{
	return nullptr;
}

void* BufferObject::mapRange(unsigned offset, unsigned count, object_buffer::OBJECT_ACCESS_FLAGS flags)
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

object_buffer::OBJECT_TYPE BufferObject::type()
{
	return buffer_type_;
}

bool BufferObject::unMap() const 
{
	return true;
}

void BufferObject::write(unsigned offset, const void* data, unsigned count)
{
	
}

void BufferObject::bufferType()
{
	switch (buffer_type_)
	{
	case object_buffer::OBJECT_TYPE::INDEX_BUFFER:
		current_buffer_ = GL_ELEMENT_ARRAY_BUFFER;
		break;
	case object_buffer::OBJECT_TYPE::VERTEX_BUFFER:
		current_buffer_ = GL_ARRAY_BUFFER;
		break;
	}
}
