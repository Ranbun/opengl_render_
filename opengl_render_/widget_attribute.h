#ifndef WIDGET_ATTRIBUTE_H_
#define WIDGET_ATTRIBUTE_H_

/*
 * 记录窗口的属性 单纯用作接口
 *
 */

#include <string>
#include "glfw/glfw3.h"

class WidgetAttribute
{
public:
	WidgetAttribute();
	virtual ~WidgetAttribute() = default;

	int width() const;
	int height() const;
	const std::string& title();
	void setTitle(const std::string& title);
	
protected:
	virtual void resizeEvent(const int w, const int h) = 0;
	virtual void mouseCursorMoveEvent(const double x, const double y) = 0;

protected:
	GLFWwindow* m_window;
	int m_width;
	int m_height;
	std::string m_title;
};

#endif //_WIDGET_ATTRIBUTE_H_