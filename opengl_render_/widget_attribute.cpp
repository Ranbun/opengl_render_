#include "widget_attribute.h"
#include <GLFW/glfw3.h>

WidgetAttribute::WidgetAttribute()
	: m_window(nullptr)
	, m_width(0)
	, m_height(0)
	, m_title("Render Widget")
{

}

int WidgetAttribute::width() const
{
	return m_width;
}

int WidgetAttribute::height() const
{
	return m_height;
}

const std::string& WidgetAttribute::title()
{
	return m_title;
}

void WidgetAttribute::setTitle(const std::string& title)
{
	glfwSetWindowTitle(m_window, title.c_str());
	m_title = title;
}
