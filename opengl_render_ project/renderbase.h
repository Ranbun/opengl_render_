#ifndef RENDERBASE_H
#define RENDERBASE_H
#include "camera.h"
#include "shader.h"
#include "vertexarraybufferobject.h"
#include "bufferobject.h"

class RenderBase
{
public:
	RenderBase();
	virtual ~RenderBase();
	virtual void render();
	virtual void init();
	virtual Camera* currentCamera();

protected:
	Camera* default_camera_;
	Shader * default_shader_;   // 默认的着色器
    bool is_init_;

    VertexArrayObject * default_vao_;
    BufferObject *      default_ebo_;
    BufferObject *      default_vbo_;

};


#endif 
