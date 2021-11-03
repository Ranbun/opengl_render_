#ifndef RENDERBASE_H
#define RENDERBASE_H
#include "camera.h"

class RenderBase
{
public:
	RenderBase();
	virtual ~RenderBase();
	virtual void render();
	virtual Camera* currentCamera();

protected:
	Camera* default_camera_;

	
};


#endif 
