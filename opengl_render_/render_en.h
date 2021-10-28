#ifndef RENDER_EN_H_
#define RENDER_EN_H_

/*
 * ��Ⱦʹ�õĺ����صĶ���
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


#endif 