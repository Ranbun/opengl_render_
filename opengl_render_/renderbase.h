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

	static RenderBase* get()
	{
		static RenderBase* ob = new RenderBase;
		return ob;
	}

protected:
	Camera* default_camera_;

	
};


#endif 
