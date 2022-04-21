#include <cassert>
#include <iostream>

#include "camera.h"
#include "RenderWidget.h"
#include "widget_attribute.h"

RenderWidget::RenderWidget(const int width, const int height, const std::string& title)
    : WidgetAttribute()
{
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    // 绑定上下文到当前线程 
    glfwMakeContextCurrent(m_window);
    //  加载OpenGL函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initzlized GLAD" << std::endl;
        return;
    }
    // 初始化视口大小 
    glViewport(0, 0, 800, 600);
    //  记录当前指针到窗口 -- 在回调中能拿到当前实例 
    glfwSetWindowUserPointer(m_window, this);

    m_width = width;
    m_height = height;
    m_title = title;
}

RenderWidget::~RenderWidget()
{
    glfwTerminate();
}

RenderWidget* RenderWidget::getRenderWidget(GLFWwindow* window)
{
    const auto userData = glfwGetWindowUserPointer(window);
    return static_cast<RenderWidget*>(userData);
}

void RenderWidget::frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height)
{
    // 重新设置视口的大小
    glViewport(0, 0, width, height);

    // 获取当前窗口 
    const auto renderWidget = getRenderWidget(window);
    renderWidget->m_width = width;
    renderWidget->m_height = height;

    // 进入事件 
    renderWidget->resizeEvent(width, height);

    // 更新窗口
    glfwMakeContextCurrent(window);
    renderWidget->render();
    glfwSwapBuffers(window);

}     

// 鼠标的位置在窗口发生变化
void RenderWidget::mouseCursorPosCallback(GLFWwindow* window, const double x, const double y)
{
    const auto renderWidget = getRenderWidget(window);

    // 第一次被捕捉
    if (renderWidget->m_mouse.firstInput())
    {
        renderWidget->m_mouse.setPos(static_cast<int>(x), static_cast<int>(y));
        renderWidget->m_mouse.m_first = false;
    }

    auto& mouse = renderWidget->m_mouse;
    const auto xOffset = static_cast<float>(x) - static_cast<float>(mouse.m_pos.m_x);
    const auto yOffset = static_cast<float>(mouse.m_pos.m_y) - static_cast<float>(y);

    const auto& camera = renderWidget->m_camera;
    camera.camera()->processMouseMovement(xOffset, yOffset);
    // 记录此次鼠标所在的位置
    mouse.setPos(static_cast<int>(x), static_cast<int>(y));
    renderWidget->mouseCursorMoveEvent(x, y);
}

void RenderWidget::scrollCallback(GLFWwindow* window, double offset_x, const double offset_y)
{
    const auto renderWidget = getRenderWidget(window);
    const auto& camera = renderWidget->m_camera;
    camera.camera()->processMouseScroll(static_cast<float>(offset_y));
    renderWidget->wheelEvent(static_cast<float>(offset_y));

}

void RenderWidget::setFrameBufferSizeCallback() const
{
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallBack);
}

void RenderWidget::setMousePosCallback() const
{
    glfwSetCursorPosCallback(m_window, mouseCursorPosCallback);
}

void RenderWidget::setScrollCallback() const
{
    glfwSetScrollCallback(m_window, scrollCallback);
}

void RenderWidget::initialize(gl_version_major major, gl_version_minor minor)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void RenderWidget::run()
{
    // 初始化资源             
    init();

    while (!glfwWindowShouldClose(m_window))
    {
        glfwWaitEvents();
        frameTime();
        // 获取输入	 s
        processInput();
        // 渲染指令 
        render();
        // 检查并调用事件 交换缓冲 
        glfwSwapBuffers(m_window);
        // glfwPollEvents();
    }

    // 释放资源
    destroy();

    glfwTerminate();
}

Camera* RenderWidget::currentCamera() const
{
    assert(m_camera.camera());
    return m_camera.camera();
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
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        currentCamera()->processKeyBoard(camera_movement::forward, m_camera.m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        currentCamera()->processKeyBoard(camera_movement::backward, m_camera.m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        currentCamera()->processKeyBoard(camera_movement::left, m_camera.m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        currentCamera()->processKeyBoard(camera_movement::right, m_camera.m_deltaTime);

}

void RenderWidget::frameTime()
{	// 计算每一帧的时间
    const auto currentFrame = static_cast<float>(glfwGetTime());
    m_camera.m_deltaTime = currentFrame - m_camera.m_lastTime;
    m_camera.m_lastTime = currentFrame;
}

void RenderWidget::mouseCursorMoveEvent(const double x, const double y)
{
    // std::cout<<"mouseCursorMoveEvent"<<std::endl;
}

void RenderWidget::wheelEvent(const float delta) // 滚动的角度 
{
    // std::cout << "WheelEvent" << std::endl;
}

void RenderWidget::resizeEvent(const int w, const int h)
{
    // std::cout<<"resizeEvent"<<std::endl;
}



