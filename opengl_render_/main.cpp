#include <iostream>
#include "renderwidget.h"

int main()
{
	// ��ʼ��ϵͳ��Դ
	RenderWidget::initialize(render_widget::gl_version_major::major_3,render_widget::gl_version_minor::minor_3);
	RenderWidget render_widget;
	render_widget.run();
	return 0;
}
