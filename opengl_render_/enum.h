#ifndef RENDER_EN_H_
#define RENDER_EN_H_

/*
 * 渲染使用的宏的相关的定义
 * 
 */

namespace oglRender
{
	namespace objectBuffer
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
	namespace camera
	{
		enum class Camera_Movement
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};
	};
	namespace renderWidget
	{
		enum class gl_version_major
		{
			major_3 = 3,
			major_4 = 4,
		};
		enum class gl_version_minor
		{
			minor_0 = 0,
			minor_1 = 1,
			minor_2 = 2,
			minor_3 = 3,
			minor_4 = 4,
			minor_5 = 5,
			minor_6 = 6,
			minor_7 = 7,
			minor_8 = 8,
			minor_9 = 9
		};
	}

}

#endif 