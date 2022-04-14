#ifndef _RENDERWIDGET_H_
#define _RENDERWIDGET_H_

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "enum.h"
#include "widget_attribute.h"

//初始化上下文的版本 
namespace oglRender
{
    namespace  renderWidget
    {

        struct MousePos
        {
            int x_;
            int y_;
        };
        struct MouseCallBackData
        {
            MouseCallBackData()
            {
                pos_.x_ = 0;
                pos_.y_ = 0;
                first_ = true;
            }

            bool firstInput() const
            {
                return first_;
            }

            void setPos(const int x, const int y)
            {
                pos_.x_ = x;
                pos_.y_ = y;
            }

            MousePos pos_;
            bool first_;    // 第一次响应鼠标回调
        };
        struct CameraData
        {
            CameraData()
            {
                camera_ = nullptr;
                delta_time_ = 0;
                last_time_ = 0;
            }
            ~CameraData()
            {
                if (camera_)
                {
                    delete camera_;
                    camera_ = nullptr;
                }
            }

            Camera* camera() const
            {
                return camera_;
            }

            Camera* camera_;
            float delta_time_;
            float last_time_;
        };
    }
};

using namespace oglRender;

class RenderWidget :public WidgetAttribute
{
public:
    explicit RenderWidget(const int width = 800, const int height = 600, std::string title = "OpenGL Widgets");
    ~RenderWidget() override;

    // 初始化系统资源
    static void initialize(renderWidget::gl_version_major major, renderWidget::gl_version_minor minor);
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
    void resizeEvent(const int w,const int h) override;
    void mouseCursorMoveEvent(const double x, const double y) override;

protected:
    // draw function
    virtual void render();;
    // 初始化资源
    virtual void init();
    // 释放资源
    virtual void destroy();

protected:
    // 鼠标回调产生的信息  
    renderWidget::MouseCallBackData m_mouse;
    renderWidget::CameraData m_camera;

protected:
    void processInput() const;
    void frameTime();
};






#endif 