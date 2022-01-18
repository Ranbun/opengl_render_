#ifndef RENDERBASE_H
#define RENDERBASE_H
#include "camera.h"
#include "shader.h"

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
	
};


#endif 
