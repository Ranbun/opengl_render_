#include <iostream>
#include "renderwidget.h"
#include "callbackfunction.h"

int main()
{
	// 初始化系统资源
	RenderWidget::initialize();
	
	auto render_widget = RenderWidget::getSingleObject();
	// 设置跟新回调 
	render_widget->setFrameBufferSizeCallback();

	const auto render_base = new RenderBase;
	render_widget->setRenderObject(render_base);
	
	render_widget->run();

	delete render_base;
	
	return 0;
}
