#include <cassert>
#include "RenderWidget.h"
#include "camera.h"
#include "widget_attribute.h"

RenderWidget::RenderWidget(const int width, const int height, const std::string title)
	: WidgetAttribute()
{
	window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window_ == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	// glfwHideWindow(window_);
	// �������ĵ���ǰ�߳� 
	glfwMakeContextCurrent(window_);
	//  ����OpenGL����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initzlized GLAD" << std::endl;
		return;
	}
	// ��ʼ���ӿڴ�С 
	glViewport(0, 0, 800, 600);
	//  ��¼��ǰָ�뵽���� -- �ڻص������õ���ǰʵ�� 
	glfwSetWindowUserPointer(window_, this);

	width_ = width;
	height_ = height;
	title_ = title;
}

RenderWidget::~RenderWidget()
{
	glfwTerminate();
}

RenderWidget* RenderWidget::getRenderWidget(GLFWwindow* window)
{
	const auto user_data = glfwGetWindowUserPointer(window);
	return static_cast<RenderWidget*>(user_data);
}

void RenderWidget::frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height)
{
	// ���������ӿڵĴ�С
	glViewport(0, 0, width, height);

	// ��ȡ��ǰ���� 
	const auto render_widget = getRenderWidget(window);
	render_widget->width_ = width;
	render_widget->height_ = height;

	// �����¼� 
	render_widget->resizeEvent(width, height);
}

// ����λ���ڴ��ڷ����仯
void RenderWidget::mouseCursorPosCallback(GLFWwindow* window, const double x, const double y)
{
	const auto render_widget = getRenderWidget(window);

	// ��һ�α���׽
	if (render_widget->mouse_.firstInput())
	{
		render_widget->mouse_.setPos(x, y);
		render_widget->mouse_.first_ = false;
	}

	auto& mouse = render_widget->mouse_;
	const float x_offset = x - mouse.pos_.x_;
	const float y_offset = mouse.pos_.y_ - y;

	const auto& camera = render_widget->m_camera_;
	camera.camera()->processMouseMovement(x_offset, y_offset);
	// ��¼�˴�������ڵ�λ��
	mouse.setPos(x, y);
	render_widget->mouseCursorMoveEvent(x, y);
}

void RenderWidget::scrollCallback(GLFWwindow* window, double offset_x, const double offset_y)
{
	const auto render_widget = getRenderWidget(window);
	const auto& camera = render_widget->m_camera_;
	camera.camera()->processMouseScroll(offset_y);
}

void RenderWidget::setFrameBufferSizeCallback() const
{
	glfwSetFramebufferSizeCallback(window_, frameBufferSizeCallBack);
}

void RenderWidget::setMousePosCallback() const
{
	glfwSetCursorPosCallback(window_, mouseCursorPosCallback);
}

void RenderWidget::setScrollCallback() const
{
	glfwSetScrollCallback(window_, scrollCallback);
}

void RenderWidget::initialize(render_widget::gl_version_major major, render_widget::gl_version_minor minor)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(major));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(minor));
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void RenderWidget::run()
{
	// glfwShowWindow(window_);
	// ��ʼ����Դ             
	init();

	while (!glfwWindowShouldClose(window_))
	{
		frameTime();
		// ��ȡ����	 s
		processInput();
		// ��Ⱦָ�� 
		render();
		// ��鲢�����¼� �������� 
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}

	// �ͷ���Դ
	destroy();

	glfwTerminate();
}

Camera* RenderWidget::currentCamera() const
{
	assert(m_camera_.camera());
	return m_camera_.camera();
}

void RenderWidget::resizeEvent(const int w, const int h)
{

}

void RenderWidget::mouseCursorMoveEvent(const double x, const double y)
{

}

void RenderWidget::render()
{
	glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderWidget::init()
{

}

void RenderWidget::destroy()
{

}

void RenderWidget::processInput() const
{
	if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window_, true);
	}

	if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
		currentCamera()->processKeyBoard(camera::Camera_Movement::FORWARD, m_camera_.delta_time_);
	if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
		currentCamera()->processKeyBoard(camera::Camera_Movement::BACKWARD, m_camera_.delta_time_);
	if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
		currentCamera()->processKeyBoard(camera::Camera_Movement::LEFT, m_camera_.delta_time_);
	if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
		currentCamera()->processKeyBoard(camera::Camera_Movement::RIGHT, m_camera_.delta_time_);

}

void RenderWidget::frameTime()
{	// ����ÿһ֡��ʱ��
	const auto current_frame = static_cast<float>(glfwGetTime());
	m_camera_.delta_time_ = current_frame - m_camera_.last_time_;
	m_camera_.last_time_ = current_frame;
}



