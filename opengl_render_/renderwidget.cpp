#include <cassert>
#include "RenderWidget.h"

RenderWidget::RenderWidget(const int width, const int height, std::string title)
    :render_(nullptr),
    first_mouse_(true),
    last_x_(0.0),
    last_y_(0.0),
    width_(width),height_(height)
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

    // 设置回调函数 
    setFrameBufferSizeCallback();

    //  记录当前指针到窗口
    glfwSetWindowUserPointer(window_,this);

    camera_ = new Camera(glm::vec3(0.0, 0.0, 3.0));
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
    // 重新设置视口的大小
    glViewport(0, 0, width, height);

    // 获取当前窗口 
    const auto render_widget = getRenderWidget(window);
    render_widget->renderBase()->resizeEvent(width, height);

    // 进入事件 
    render_widget->resizeEvent(width, height);

}

void RenderWidget::mouseCallback(GLFWwindow* window, double pos_x, double pos_y)
{
    const auto render_widget = getRenderWidget(window);

    // 第一次被捕捉
    if (render_widget->first_mouse_)
    {
        render_widget->last_x_ = pos_x;
        render_widget->last_y_ = pos_y;
        render_widget->first_mouse_ = false;
    }

    const float x_offset = pos_x - render_widget->last_x_;
    const float y_offset = render_widget->last_y_ - pos_y;

    render_widget->last_x_ = pos_x;
    render_widget->last_y_ = pos_y;

    // render_widget->render_->currentCamera()->processMouseMovement(x_offset, y_offset);
    render_widget->camera_->processMouseMovement(x_offset, y_offset);

}

void RenderWidget::scrollCallback(GLFWwindow* window, double offset_x, const double offset_y)
{
    const auto render_widget = getRenderWidget(window);
    // render_widget->render_->currentCamera()->processMouseScroll(offset_y);
    render_widget->camera_->processMouseScroll(offset_y);
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
    // assert(render_);

    // 初始化资源 
    init();

    while (!glfwWindowShouldClose(window_))
    {
        const auto current_frame = static_cast<float>(glfwGetTime());
        delta_time_ = current_frame - last_frame_;
        last_frame_ = current_frame;

        // 获取输入
        processInput();

        // 渲染指令 
        // render_->render();
        render();

        // 检查并调用事件 交换缓冲 
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    // 释放资源
    // render_->destroy();
    destroy();
    glfwTerminate();
}

int RenderWidget::width() const
{
    return width_;
}

int RenderWidget::height() const
{
    return height_;
}

void RenderWidget::resizeEvent(const int w, const int h)
{
    width_ = w;
    height_ = h;
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

RenderBase* RenderWidget::renderBase() const
{
    return render_;
}


