#ifndef RENDER_EN_H_
#define RENDER_EN_H_

/*
 * 渲染使用的宏的相关的定义
 * 
 */

namespace object_buffer
{
	enum class OBJECT_TYPE
	{
		VERTEX_BUFFER,
		INDEX_BUFFER
	};

	enum class OBJECT_ACCESS
	{
		READ_ONLY,
		WRITE_ONLY,
		READ_WRITE
	};
	
	enum class OBJECT_ACCESS_FLAGS
	{
		READ,
		WRITE,
		INVALIDATE
	};

};


namespace texture
{
	enum class WRAP
	{
		S,
		T,
		R
	};

	enum class FILETER
	{
		MIN,
		MAG
	};
}


#endif 