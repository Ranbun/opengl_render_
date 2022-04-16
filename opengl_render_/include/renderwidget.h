#ifndef RENDER_WIDGET_H_
#define RENDER_WIDGET_H_

#include <string>

#include "camera.h"
#include "enum.h"
#include "widget_attribute.h"

//初始化上下文的版本 

struct MousePos
{
    int m_x;
    int m_y;
};
struct MouseCallBackData
{
    MouseCallBackData()
    {
        m_pos.m_x = 0;
        m_pos.m_y = 0;
        m_first = true;
    }

    bool firstInput() const
    {
        return m_first;
    }

    void setPos(const int x, const int y)
    {
        m_pos.m_x = x;
        m_pos.m_y = y;
    }

    MousePos m_pos;
    bool m_first;    // 第一次响应鼠标回调
};
struct CameraData
{
    CameraData()
    {
        m_camera = nullptr;
        m_deltaTime = 0;
        m_lastTime = 0;
    }
    ~CameraData()
    {
        if (m_camera)
        {
            delete m_camera;
            m_camera = nullptr;
        }
    }

    Camera* camera() const
    {
        return m_camera;
    }

    Camera* m_camera;
    float m_deltaTime;
    float m_lastTime;
};

using namespace oglRender::renderWidget;

class RenderWidget :public WidgetAttribute
{
public:
    explicit RenderWidget(const int width = 800, const int height = 600, const std::string& title = "OpenGL Widgets");
    ~RenderWidget() override;

    // 初始化系统资源
    static void initialize(gl_version_major major, gl_version_minor minor);
    virtual void run();

    Camera* currentCamera()  const;

protected:
    // 窗口指针
    static RenderWidget* getRenderWidget(GLFWwindow* window);

    // 窗口的回调函数 
    static void frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height);
    static void mouseCursorPosCallback(GLFWwindow* window, double x, double y);
    static void scrollCallback(GLFWwindow* window, double offset_x, double offset_y);

    // 设置更新回调函数
    void setFrameBufferSizeCallback() const;
    void setMousePosCallback() const;
    void setScrollCallback() const;

protected:
    // 事件函数
    void resizeEvent(const int w, const int h) override;
    void mouseCursorMoveEvent(const double x, const double y) override;
    void wheelEvent(const float delta) override;

protected:
    // draw function
    virtual void render();;
    // 初始化资源
    virtual void init();
    // 释放资源
    virtual void destroy();

protected:
    // 鼠标回调产生的信息  
    MouseCallBackData m_mouse;
    CameraData m_camera;

protected:
    void processInput() const;
    void frameTime();
};






#endif 