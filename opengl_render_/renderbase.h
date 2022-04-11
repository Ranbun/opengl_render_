#ifndef RENDERBASE_H
#define RENDERBASE_H

#include "camera.h"
#include "shader.h"
#include "vertexarraybufferobject.h"
#include "bufferobject.h"

class RenderBase
{
	friend class RenderWidget;
public:
	RenderBase();
	virtual ~RenderBase();

	virtual void init();

protected:
	virtual void render();
	virtual Camera* currentCamera();
	virtual void destroy();

protected:
	Camera* default_camera_;
	Shader* default_shader_;   // 默认的着色器
	bool initialized_;

	VertexArrayObject* d_vao_;
	BufferObject* d_ebo_;
	BufferObject* d_vbo_;


private:
	int width_;
	int height_;

protected:
	//  窗口事件函数  
	virtual void resizeEvent(const int w, const int h);

	// 获取当前窗口的宽高 
	int width() const;
	int height() const;
};


#endif 
