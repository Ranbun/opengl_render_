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
	// 设置更新回调函数
	void setFrameBufferSizeCallback() const;

	void setRenderObject(RenderBase* object);

	// 初始化系统资源
	static void initialize();
	GLFWwindow* getWindow() const;
	virtual void run();

private:
	//渲染窗口 
	GLFWwindow* window_;
	RenderBase* render_;

protected:
	void processInput() const;
};



#endif 