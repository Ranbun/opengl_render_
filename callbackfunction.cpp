#include "callbackfunction.h"

void frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height)
{
	// 重新设置视口的大小 
	glViewport(0, 0, width, height);
}
