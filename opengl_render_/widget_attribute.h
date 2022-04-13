#ifndef _WIDGET_ATTRIBUTE_H_
#define _WIDGET_ATTRIBUTE_H_

/*
 * ��¼���ڵ����� ���������ӿ�
 *
 */

#include <string>

class GLFWwindow;

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

	// TODO �˴���Ҫ����
	virtual void init() = 0;
	virtual void destroy() = 0;

protected:
	GLFWwindow* window_;
	int width_;
	int height_;
	std::string title_;
};

#endif //_WIDGET_ATTRIBUTE_H_