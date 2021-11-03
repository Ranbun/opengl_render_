#include "callbackfunction.h"
#include "camera.h"

void frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height)
{
	// 重新设置视口的大小 
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	auto render_widget = RenderWidget::getSingleObject();
	if (render_widget->first_mouse_)
	{
		render_widget->last_x_ = xpos;
		render_widget->last_y_ = ypos;
		render_widget->first_mouse_ = false;
	}

	float xoffset = xpos - render_widget->last_x_;
	float yoffset = render_widget->last_y_ - ypos;

	render_widget->last_x_ = xpos;
	render_widget->last_y_ = ypos;

	render_widget->render_->currentCamera()->processMouseMovement(xoffset, yoffset);

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto render_widget = RenderWidget::getSingleObject();
	render_widget->render_->currentCamera()->processMouseScroll(yoffset);
}