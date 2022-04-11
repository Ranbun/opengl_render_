#ifndef RENDERWIDGET_h
#define RENDERWIDGET_h

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderbase.h"

class RenderWidget
{
	friend void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	friend void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

protected:
	RenderWidget(const int width, const int height, std::string title);
	~RenderWidget();
public:
	// ���ø��»ص�����
	void setFrameBufferSizeCallback() const;
	void setMousePosCallback() const;
	void setScrollCallback() const;
	void setRenderObject(RenderBase* object);

	// ��ʼ��ϵͳ��Դ
	static void initialize();
	GLFWwindow* getWindow() const;
	virtual void run();

	static RenderWidget* getSingleObject();

	// ���ÿ��
	void setSize(int w,int h)
    {
        width_ = w;
        height_ = h;
    }

    int getWidth()
    {
	    return width_;
    }

    int getHeight()
    {
	    return height_;
    }

protected:
	//��Ⱦ���� 
	GLFWwindow* window_;
	RenderBase* render_;

	// ����delta Time 
	float delta_time_ = 0.0f;  // ��ǰ֡����һ֡��ʱ���
	float last_frame_ = 0.0f;  // ��һ֡��ʱ�� 

	bool first_mouse_;  // default true
	float last_x_;
	float last_y_;

	int width_;
	int height_;

protected:
	void processInput() const;

};



#endif 