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

	// ������Ⱦ���� 
	void setRenderObject(RenderBase* object);

	// ��ʼ��ϵͳ��Դ
	static void initialize(render_widget::gl_version_major major, render_widget::gl_version_minor minor);
	virtual void run();

protected:
	// ����ָ��
	static RenderWidget* getRenderWidget(GLFWwindow* window);

	// ���ڵĻص����� 
	static void frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height);
	static void mouseCallback(GLFWwindow* window, double pos_x, double pos_y);
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

	// draw function
	virtual void render();;
	// ��ʼ����Դ
	virtual void init();
	// �ͷ���Դ
	virtual void destroy();

protected:
	//��Ⱦ����
	GLFWwindow* window_;
	//������Ⱦ���� -- ֻ��Ҫ��д��Ⱦ���ֵ��߼�
	RenderBase* render_;

	//camera
	Camera* camera_;

	// ����delta Time
	float delta_time_{ 0.0f };  // ��ǰ֡����һ֡��ʱ���
	float last_frame_{ 0.0f };  // ��һ֡��ʱ�� 

	// �Ƿ��ǵ�һ��������
	bool first_mouse_{true};  // default true
	float last_x_{0};
	float last_y_{0};

private:
	// ���ڵĿ�� 
	int width_;
	int height_;


protected:
	void processInput() const;
	RenderBase* renderBase() const;
};



#endif 