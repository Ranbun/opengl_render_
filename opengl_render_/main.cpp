#include <iostream>
#include "renderwidget.h"

// 屏蔽控制台 
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main()
{
	// 初始化系统资源
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
