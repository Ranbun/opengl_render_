#include <iostream>
#include "renderwidget.h"
#include "callbackfunction.h"

int main()
{
	// ��ʼ��ϵͳ��Դ
	RenderWidget::initialize();
	
	auto render_widget = RenderWidget::getSingleObject();
	// ���ø��»ص� 
	render_widget->setFrameBufferSizeCallback();

	const auto render_base = new RenderBase;
	render_widget->setRenderObject(render_base);
	
	render_widget->run();

	delete render_base;
	
	return 0;
}
