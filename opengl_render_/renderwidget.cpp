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
    // �������ĵ���ǰ�߳� 
    glfwMakeContextCurrent(m_window);
    //  ����OpenGL����
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initzlized GLAD" << std::endl;
        return;
    }
    // ��ʼ���ӿڴ�С 
    glViewport(0, 0, 800, 600);
    //  ��¼��ǰָ�뵽���� -- �ڻص������õ���ǰʵ�� 
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
    // ���������ӿڵĴ�С
    glViewport(0, 0, width, height);

    // ��ȡ��ǰ���� 
    const auto renderWidget = getRenderWidget(window);
    renderWidget->m_width = width;
    renderWidget->m_height = height;

    // �����¼� 
    renderWidget->resizeEvent(width, height);

    // ���´���
    glfwMakeContextCurrent(window);
    renderWidget->render();
    glfwSwapBuffers(window);

}     

// ����λ���ڴ��ڷ����仯
void RenderWidget::mouseCursorPosCallback(GLFWwindow* window, const double x, const double y)
{
    const auto renderWidget = getRenderWidget(window);

    // ��һ�α���׽
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
    // ��¼�˴�������ڵ�λ��
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
    // ��ʼ����Դ             
    init();

    while (!glfwWindowShouldClose(m_window))
    {
        glfwWaitEvents();
        frameTime();
        // ��ȡ����	 s
        processInput();
        // ��Ⱦָ�� 
        render();
        // ��鲢�����¼� �������� 
        glfwSwapBuffers(m_window);
        // glfwPollEvents();
    }

    // �ͷ���Դ
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
{	// ����ÿһ֡��ʱ��
    const auto currentFrame = static_cast<float>(glfwGetTime());
    m_camera.m_deltaTime = currentFrame - m_camera.m_lastTime;
    m_camera.m_lastTime = currentFrame;
}

void RenderWidget::mouseCursorMoveEvent(const double x, const double y)
{
    // std::cout<<"mouseCursorMoveEvent"<<std::endl;
}

void RenderWidget::wheelEvent(const float delta) // �����ĽǶ� 
{
    // std::cout << "WheelEvent" << std::endl;
}

void RenderWidget::resizeEvent(const int w, const int h)
{
    // std::cout<<"resizeEvent"<<std::endl;
}



