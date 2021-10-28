#include "vertexarraybufferobject.h"
#include <glad/glad.h>

VertexArrayObject::VertexArrayObject()
{
	vao_ = 0;
	is_create_ = false;
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &vao_);
}

void VertexArrayObject::bind() const
{	if (isCreated())
	{
		glBindVertexArray(vao_);
	}

}

bool VertexArrayObject::create()
{
	if (!is_create_)
	{
		glGenVertexArrays(1, &vao_);
		is_create_ = true;
	}
	
	return is_create_;
}

void VertexArrayObject::destroy()
{
	glDeleteVertexArrays(1, &vao_);
	vao_ = 0;
	is_create_ = false;
}

bool VertexArrayObject::isCreated() const
{
	return is_create_;
}

unsigned int VertexArrayObject::objectId() const
{
	return vao_;
}

void VertexArrayObject::release()
{
	glBindVertexArray(0);
}
