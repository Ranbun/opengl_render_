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
	// 设置更新回调函数
	void setFrameBufferSizeCallback() const;
	void setMousePosCallback() const;
	void setScrollCallback() const;
	void setRenderObject(RenderBase* object);

	// 初始化系统资源
	static void initialize();
	GLFWwindow* getWindow() const;
	virtual void run();

	static RenderWidget* getSingleObject();

	// 设置宽高
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
	//渲染窗口 
	GLFWwindow* window_;
	RenderBase* render_;

	// 计算delta Time 
	float delta_time_ = 0.0f;  // 当前帧与上一帧的时间差
	float last_frame_ = 0.0f;  // 上一帧的时间 

	bool first_mouse_;  // default true
	float last_x_;
	float last_y_;

	int width_;
	int height_;

protected:
	void processInput() const;

};



#endif 