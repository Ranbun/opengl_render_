#include <iostream>
#include "ofapp.h"

int main()
{
	// 初始化系统资源
	RenderApp::initialize(render_widget::gl_version_major::major_3,
		render_widget::gl_version_minor::minor_3);

	RenderApp render_widget;
	render_widget.setTitle(std::string("OpenGL"));

	render_widget.run();
	return 0;
}
