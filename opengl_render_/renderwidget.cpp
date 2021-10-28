#include <cassert>

#include "RenderWidget.h"

RenderWidget::RenderWidget(const int width, const int height, std::string title)
	:render_(nullptr)
{
	window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window_ == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window_);

	// ���غ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initzlized GLAD" << std::endl;
		return;
	}
	// ��ʼ���ӿڴ�С 
	glViewport(0, 0, 800, 600);
	
}

RenderWidget::~RenderWidget()
{
	glfwTerminate();
}

void RenderWidget::setFrameBufferSizeCallback() const
{
	glfwSetFramebufferSizeCallback(window_, frameBufferSizeCallBack);
}

void RenderWidget::setRenderObject(RenderBase* object)
{
	if(render_)
	{
		delete render_;
		render_ = nullptr;
	}
	
	render_ = object;
}

void RenderWidget::initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* RenderWidget::getWindow() const
{
	return window_;
}

void RenderWidget::run()
{
	assert(render_);
	while (!glfwWindowShouldClose(window_))
	{
		// ��ȡ����
		processInput();

		// ��Ⱦָ�� 
		render_->render();

		// ��鲢�����¼� �������� 
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	glfwTerminate();
}

void RenderWidget::processInput() const
{
	if (glfwGetKey(window_,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window_, true);
	}
}
