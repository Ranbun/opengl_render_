#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"

//初始化上下文的版本 
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

		void setPos(const int x,const int y)
		{
			pos_.x_ = x;
			pos_.y_ = y;
		}

		MousePos pos_;
		bool first_;    // 第一次响应鼠标回调
	};

	struct  CameraData
	{
		CameraData()
		{
			camera_ = nullptr;
			delta_time_ = 0;
			last_time_ = 0;
		}
		~CameraData()
		{
			if(camera_)
			{
				delete camera_;
				camera_ = nullptr;
			}
		}

		Camera * camera() const
		{
			return camera_;
		}

		Camera* camera_;
		float delta_time_;
		float last_time_;
	};


};


class RenderWidget
{
public:
	explicit RenderWidget(const int width = 800, const int height = 600, std::string title = "OpenGL Widgets");
	~RenderWidget();

	// 初始化系统资源
	static void initialize(render_widget::gl_version_major major, render_widget::gl_version_minor minor);
	virtual void run();

	Camera * currentCamera()  const;

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

public:
	// 窗口的宽度
	int width()const;
	// 窗口的高度
	int height() const;

protected:
	// 设置宽高
	virtual void resizeEvent(const int w, const int h);
	virtual void mouseCursorMoveEvent(const double x,const double y);

	// draw function
	virtual void render();;
	// 初始化资源
	virtual void init();
	// 释放资源
	virtual void destroy();

private:
	//渲染窗口
	GLFWwindow* window_;
	// 窗口的宽高 
	int width_;
	int height_;

protected:
	// 鼠标回调产生的信息  
	render_widget::MouseCallBackData mouse_;
	render_widget::CameraData m_camera_;

protected:
	void processInput() const;
	void frameTime();
};



#endif 