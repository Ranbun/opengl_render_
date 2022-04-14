#ifndef BUFFEROBJECT_H
#define BUFFEROBJECT_H

#include <glad/glad.h>
#include "enum.h"

using namespace oglRender;

class BufferObject
{
public:
	explicit BufferObject();   // 默认创建的是一个 VBO vertex buffer object 
	explicit BufferObject(objectBuffer::OBJECT_TYPE type);
	~BufferObject();

	void allocate(const void* data, unsigned int count) const;
	void allocate(unsigned int count);

	bool bind() const;
	unsigned int bufferId() const;
	 
	bool create();
	void destroy();

	bool isCreate() const;

	void* map(objectBuffer::OBJECT_ACCESS access);
	static void* mapRange(unsigned int offset, unsigned int count, objectBuffer::OBJECT_ACCESS_FLAGS flags);
	static bool read(int offset, void* data, unsigned int count);

	void release() const;
	// void setUsagePattern(QOpenGLBuffer::UsagePattern value)
	size_t size() const;

	objectBuffer::OBJECT_TYPE type() const;
	bool unMap() const;

	static void write(unsigned int offset, const void* data,unsigned int count);

	// 当前的buffer 类型 
	void bufferType();

private:
	unsigned int object_{};
	objectBuffer::OBJECT_TYPE buffer_type_;
	bool is_create_;
	GLenum current_buffer_{GL_ARRAY_BUFFER};
	size_t count_;
};

#endif 
