#include <iostream>
#include "renderwidget.h"
#include "callbackfunction.h"

// ���ο���̨ 
//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main()
{
	// ��ʼ��ϵͳ��Դ
	RenderWidget::initialize();
	
	auto render_widget = RenderWidget::get();
	glfwSetFramebufferSizeCallback(render_widget->getWindow(), frameBufferSizeCallBack);
	glfwSetCursorPosCallback(render_widget->getWindow(), mouseCallback);
	glfwSetScrollCallback(render_widget->getWindow(), scrollCallback);


	const auto render_base = new RenderBase;
	render_widget->setRenderObject(render_base);
	
	render_widget->run();

	delete render_base;
	
	return 0;
}
