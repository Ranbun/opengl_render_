#ifndef WIDGET_ATTRIBUTE_H_
#define WIDGET_ATTRIBUTE_H_

/*
 * 记录窗口的属性 单纯用作接口
 *
 */

#include <string>
#include <GLFW/glfw3.h>

class WidgetAttribute
{
public:
    WidgetAttribute();
    virtual ~WidgetAttribute() = default;
    WidgetAttribute(const WidgetAttribute&) = delete;
    WidgetAttribute(const WidgetAttribute&&) = delete;
    WidgetAttribute& operator=(const WidgetAttribute&) = delete;
    WidgetAttribute& operator=(const WidgetAttribute&&) = delete;

    int width() const;
    int height() const;
    const std::string& title();
    void setTitle(const std::string& title);

protected:
    virtual void resizeEvent(int w, int h) = 0;
    virtual void mouseCursorMoveEvent(double x, double y) = 0;
    virtual void wheelEvent(float delta) = 0;

protected:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;
};

#endif //_WIDGET_ATTRIBUTE_H_
