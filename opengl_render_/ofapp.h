#ifndef _OF_APP_H_
#define _OF_APP_H_

#include "renderwidget.h"
#include <glm/vec3.hpp>
#include "camera.h"
class RenderApp :public RenderWidget
{
public:
	RenderApp(const int width = 800, const int height = 600, const std::string& title ="Render Widget")
		:RenderWidget(width, height, title)
	{
		setFrameBufferSizeCallback();
		setMousePosCallback();
		setScrollCallback();

		m_camera_.camera_ = new Camera(glm::vec3(0.0, 0.0, 3.0));
	}
	~RenderApp() override {}

};

#endif 
