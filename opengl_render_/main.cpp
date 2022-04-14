#include <iostream>
#include "ofapp.h"

int main()
{
	// ��ʼ��ϵͳ��Դ
	RenderApp::initialize(renderWidget::gl_version_major::major_3,
		renderWidget::gl_version_minor::minor_3);

	RenderApp render_widget;
	render_widget.setTitle(std::string("OpenGL"));

	render_widget.run();
	return 0;
}
