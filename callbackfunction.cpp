#include "callbackfunction.h"

void frameBufferSizeCallBack(GLFWwindow* window, const int width, const int height)
{
	// ���������ӿڵĴ�С 
	glViewport(0, 0, width, height);
}
