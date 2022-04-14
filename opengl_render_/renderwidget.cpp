#include <cassert>
#include "RenderWidget.h"
#include "camera.h"
#include "widget_attribute.h"

RenderWidget::RenderWidget(const int width, const int height, const std::string title)
    : WidgetAttribute()
{
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    // glfwHideWindow(window_);
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
    const auto user_data = glfwGetWindowUserPointer(window);
    return static_cast<RenderWidget*>(user_data);
}

void RenderWidget::frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height)
{
    // ���������ӿڵĴ�С
    glViewport(0, 0, width, height);

    // ��ȡ��ǰ���� 
    const auto render_widget = getRenderWidget(window);
    render_widget->m_width = width;
    render_widget->m_height = height;

    // �����¼� 
    render_widget->resizeEvent(width, height);
}

// ����λ���ڴ��ڷ����仯
void RenderWidget::mouseCursorPosCallback(GLFWwindow* window, const double x, const double y)
{
    const auto render_widget = getRenderWidget(window);

    // ��һ�α���׽
    if (render_widget->m_mouse.firstInput())
    {
        render_widget->m_mouse.setPos(static_cast<int>(x), static_cast<int>(y));
        render_widget->m_mouse.first_ = false;
    }

    auto& mouse = render_widget->m_mouse;
    const float x_offset = static_cast<float>(x) - static_cast<float>(mouse.pos_.x_);
    const float y_offset = static_cast<float>(mouse.pos_.y_) - static_cast<float>(y);

    const auto& camera = render_widget->m_camera;
    camera.camera()->processMouseMovement(x_offset, y_offset);
    // ��¼�˴�������ڵ�λ��
    mouse.setPos(static_cast<int>(x), static_cast<int>(y));
    render_widget->mouseCursorMoveEvent(x, y);
}

void RenderWidget::scrollCallback(GLFWwindow* window, double offset_x, const double offset_y)
{
    const auto render_widget = getRenderWidget(window);
    const auto& camera = render_widget->m_camera;
    camera.camera()->processMouseScroll(static_cast<float>(offset_y));
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

void RenderWidget::initialize(renderWidget::gl_version_major major, renderWidget::gl_version_minor minor)
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

    while (!glfwWindowShouldClose(m_window))
    {
        frameTime();
        // ��ȡ����	 s
        processInput();
        // ��Ⱦָ�� 
        render();
        // ��鲢�����¼� �������� 
        glfwSwapBuffers(m_window);
        glfwPollEvents();
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
        currentCamera()->processKeyBoard(camera::Camera_Movement::FORWARD, m_camera.delta_time_);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        currentCamera()->processKeyBoard(camera::Camera_Movement::BACKWARD, m_camera.delta_time_);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        currentCamera()->processKeyBoard(camera::Camera_Movement::LEFT, m_camera.delta_time_);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        currentCamera()->processKeyBoard(camera::Camera_Movement::RIGHT, m_camera.delta_time_);

}

void RenderWidget::frameTime()
{	// ����ÿһ֡��ʱ��
    const auto current_frame = static_cast<float>(glfwGetTime());
    m_camera.delta_time_ = current_frame - m_camera.last_time_;
    m_camera.last_time_ = current_frame;
}

void RenderWidget::mouseCursorMoveEvent(const double x, const double y)
{
    std::cout<<"mouseCursorMoveEvent"<<std::endl;
}

void RenderWidget::resizeEvent(const int w, const int h)
{
    std::cout<<"resizeEvent"<<std::endl;
}



