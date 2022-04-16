#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include <glad/glad.h>
#include "enum.h"

using namespace oglRender::objectBuffer;  // NOLINT(clang-diagnostic-header-hygiene)

class BufferObject
{
public:
	explicit BufferObject();   // 默认创建的是一个 VBO vertex buffer object 
	explicit BufferObject(object_type type);

	BufferObject(const BufferObject& another) = delete;
	BufferObject(BufferObject&& another) = delete;
	BufferObject& operator=(const BufferObject&) = delete;
	BufferObject& operator=(BufferObject&&) = delete;

	~BufferObject();

	void allocate(const void* data, unsigned int count) const;
	void allocate(unsigned int count);

	bool bind() const;
	unsigned int bufferId() const;
	 
	bool create();
	void destroy();

	bool isCreate() const;

	void* map(object_access access) const;
	void* mapRange(unsigned int offset, unsigned int count, object_access_flags flags);
	bool read(int offset, void* data, unsigned int count);

	void release() const;
	// void setUsagePattern(QOpenGLBuffer::UsagePattern value)
	size_t size() const;

	object_type type() const;
	bool unMap() const;

	void write(unsigned int offset, const void* data,unsigned int count);

	// 当前的buffer 类型 
	void bufferType();

private:
	unsigned int m_object{};
	object_type m_bufferType;
	bool m_isCreate;
	GLenum m_currentBuffer{GL_ARRAY_BUFFER};
	size_t m_count;
};

#endif 
