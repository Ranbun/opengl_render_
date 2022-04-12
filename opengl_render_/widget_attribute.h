#ifndef _WIDGET_ATTRIBUTE_H_
#define _WIDGET_ATTRIBUTE_H_

/*
 * ��¼���ڵ����� ���������ӿ�
 *
 */

#if 0
namespace render_widget
{
	namespace widget_attribute
	{
		struct MousePos
		{
			int x_;
			int y_;
		};

		struct  MouseCallBackData
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
			bool first_;    // ��һ����Ӧ���ص�
		};
	};
};
#endif 
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