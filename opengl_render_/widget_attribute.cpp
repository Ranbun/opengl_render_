#include "widget_attribute.h"
#include <GLFW/glfw3.h>

WidgetAttribute::WidgetAttribute()
	: window_(nullptr)
	, width_(0)
	, height_(0)
	, title_("Render Widget")
{

}

int WidgetAttribute::width() const
{
	return width_;
}

int WidgetAttribute::height() const
{
	return height_;
}

const std::string& WidgetAttribute::title()
{
	return title_;
}

void WidgetAttribute::setTitle(const std::string& title)
{
	glfwSetWindowTitle(window_, title.c_str());
	title_ = title;
}
