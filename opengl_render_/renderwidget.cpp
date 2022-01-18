#include <cassert>
#include "callbackfunction.h"
#include "RenderWidget.h"

RenderWidget::RenderWidget(const int width, const int height, std::string title)
	:render_(nullptr),
	first_mouse_(true),
	last_x_(width / static_cast<float>(2.0)),
	last_y_(height / static_cast<float>(2.0))
{
	window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window_ == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window_);

	// 加载函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initzlized GLAD" << std::endl;
		return;
	}
	// 初始化视口大小 
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

void RenderWidget::setMousePosCallback() const
{
	glfwSetCursorPosCallback(window_, mouseCallback);
}

void RenderWidget::setScrollCallback() const
{
	glfwSetScrollCallback(window_, scrollCallback);
}

void RenderWidget::setRenderObject(RenderBase* object)
{
	if(render_)
	{
		delete render_;
		render_ = nullptr;
	}
	
	render_ = object;
	// 初始化渲染的资源
	object->init();
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
		float current_frame = static_cast<float>(glfwGetTime());
		delta_time_ = current_frame - last_frame_;
		last_frame_ = current_frame;

		// 获取输入
		processInput();

		// 渲染指令 
		render_->render();

		// 检查并调用事件 交换缓冲 
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	glfwTerminate();
}

RenderWidget* RenderWidget::getSingleObject()
{
	static RenderWidget object(800, 600, "OPenGL Widget");
	return  &object;
}

void RenderWidget::processInput() const
{
	if (glfwGetKey(window_,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window_, true);
	}

	if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
		render_->currentCamera()->processKeyBoard(camera::Camera_Movement::FORWARD, delta_time_);
	if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
		render_->currentCamera()->processKeyBoard(camera::Camera_Movement::BACKWARD, delta_time_);
	if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
		render_->currentCamera()->processKeyBoard(camera::Camera_Movement::LEFT, delta_time_);
	if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
		render_->currentCamera()->processKeyBoard(camera::Camera_Movement::RIGHT, delta_time_);

}


