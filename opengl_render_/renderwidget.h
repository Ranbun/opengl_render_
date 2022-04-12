#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"

//��ʼ�������ĵİ汾 
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
		bool first_;    // ��һ����Ӧ���ص�
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

	// ��ʼ��ϵͳ��Դ
	static void initialize(render_widget::gl_version_major major, render_widget::gl_version_minor minor);
	virtual void run();

	Camera * currentCamera()  const;

protected:
	// ����ָ��
	static RenderWidget* getRenderWidget(GLFWwindow* window);

	// ���ڵĻص����� 
	static void frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height);
	static void mouseCursorPosCallback(GLFWwindow* window, double x, double y);
	static void scrollCallback(GLFWwindow* window, double offset_x, double offset_y);

	// ���ø��»ص�����
	void setFrameBufferSizeCallback() const;
	void setMousePosCallback() const;
	void setScrollCallback() const;

public:
	// ���ڵĿ��
	int width()const;
	// ���ڵĸ߶�
	int height() const;

protected:
	// ���ÿ��
	virtual void resizeEvent(const int w, const int h);
	virtual void mouseCursorMoveEvent(const double x,const double y);

	// draw function
	virtual void render();;
	// ��ʼ����Դ
	virtual void init();
	// �ͷ���Դ
	virtual void destroy();

private:
	//��Ⱦ����
	GLFWwindow* window_;
	// ���ڵĿ�� 
	int width_;
	int height_;

protected:
	// ���ص���������Ϣ  
	render_widget::MouseCallBackData mouse_;
	render_widget::CameraData m_camera_;

protected:
	void processInput() const;
	void frameTime();
};



#endif 