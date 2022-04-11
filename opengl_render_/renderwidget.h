#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderbase.h"

namespace render_widget
{
	enum class gl_version_major
	{
		major_3 = 3,
		major_4 = 4,
	};
	enum class gl_version_minor
	{
		minor_0 = 0,
		minor_1 = 1,
		minor_2 = 2,
		minor_3 = 3,
		minor_4 = 4,
		minor_5 = 5,
		minor_6 = 6,
		minor_7 = 7,
		minor_8 = 8,
		minor_9 = 9
	};


};

class RenderWidget
{
public:
	explicit RenderWidget(const int width = 800, const int height = 600, std::string title = "OpenGL Widgets");
	~RenderWidget();

	// 设置渲染对象 
	void setRenderObject(RenderBase* object);

	// 初始化系统资源
	static void initialize(render_widget::gl_version_major major, render_widget::gl_version_minor minor);
	virtual void run();

protected:
	// 窗口指针
	static RenderWidget* getRenderWidget(GLFWwindow* window);

	// 窗口的回调函数 
	static void frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height);
	static void mouseCallback(GLFWwindow* window, double pos_x, double pos_y);
	static void scrollCallback(GLFWwindow* window, double offset_x, double offset_y);

	// 设置更新回调函数
	void setFrameBufferSizeCallback() const;
	void setMousePosCallback() const;
	void setScrollCallback() const;

public:
	// 窗口的宽度
	int width()const;
	// 窗口的高度
	int height() const;

protected:
	// 设置宽高
	virtual void resizeEvent(const int w, const int h);

	// draw function
	virtual void render();;
	// 初始化资源
	virtual void init();
	// 释放资源
	virtual void destroy();

protected:
	//渲染窗口
	GLFWwindow* window_;
	//负责渲染部分 -- 只需要编写渲染部分的逻辑
	RenderBase* render_;

	//camera
	Camera* camera_;

	// 计算delta Time
	float delta_time_{ 0.0f };  // 当前帧与上一帧的时间差
	float last_frame_{ 0.0f };  // 上一帧的时间 

	// 是否是第一次鼠标进入
	bool first_mouse_{true};  // default true
	float last_x_{0};
	float last_y_{0};

private:
	// 窗口的宽高 
	int width_;
	int height_;


protected:
	void processInput() const;
	RenderBase* renderBase() const;
};



#endif 