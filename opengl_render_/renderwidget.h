#ifndef RENDERWIDGET_h
#define RENDERWIDGET_h

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "callbackfunction.h"
#include "renderbase.h"

class RenderWidget
{
public:
	RenderWidget(const int width, const int height, std::string title);
	~RenderWidget();
	// ���ø��»ص�����
	void setFrameBufferSizeCallback() const;

	void setRenderObject(RenderBase* object);

	// ��ʼ��ϵͳ��Դ
	static void initialize();
	GLFWwindow* getWindow() const;
	virtual void run();

private:
	//��Ⱦ���� 
	GLFWwindow* window_;
	RenderBase* render_;

protected:
	void processInput() const;
};



#endif 