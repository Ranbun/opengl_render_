#ifndef BUFFEROBJECT_H
#define BUFFEROBJECT_H

#include <glad/glad.h>

#include "render_en.h"

class BufferObject
{
public:
	BufferObject();   // 默认创建的是一个 VBO vertex buffer object 
	explicit BufferObject(object_buffer::OBJECT_TYPE type);
	~BufferObject();

	void allocate(const void* data, unsigned int count) const;
	void allocate(unsigned int count);

	bool bind() const;
	unsigned int bufferId() const;
	 
	bool create();
	void destroy();

	bool isCreate() const;

	static void* map(object_buffer::OBJECT_ACCESS access);
	static void* mapRange(unsigned int offset, unsigned int count, object_buffer::OBJECT_ACCESS_FLAGS flags);

	static bool read(int offset, void* data, unsigned int count);

	void release() const;
	

	// void setUsagePattern(QOpenGLBuffer::UsagePattern value)


	size_t size() const;

	object_buffer::OBJECT_TYPE type();
	bool unMap() const;

	static void write(unsigned int offset, const void* data,unsigned int count);

	// 当前的buffer 类型 
	void bufferType();

private:
	unsigned int object_;
	object_buffer::OBJECT_TYPE buffer_type_;
	bool is_create_;  // 是否创建完成
	unsigned int current_buffer_;
	size_t count_;
};

#endif 
