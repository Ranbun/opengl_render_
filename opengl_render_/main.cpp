#include <iostream>
#include "renderwidget.h"

// ���ο���̨ 
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main()
{
	// ��ʼ��ϵͳ��Դ
	RenderWidget::initialize();
	auto render_widget = new RenderWidget(800,600,"OpenGL Widget");
	render_widget->setFrameBufferSizeCallback();

	const auto render_base = new RenderBase;
	render_widget->setRenderObject(render_base);
	
	render_widget->run();

	delete render_widget;
	delete render_base;
	
	return 0;
}
