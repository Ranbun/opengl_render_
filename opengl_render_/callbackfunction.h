#ifndef CALLBACKFUNCTION_H
#define CALLBACKFUNCTION_H

#include "renderwidget.h"
#include <GLFW/glfw3.h>

void frameBufferSizeCallBack(GLFWwindow* window, const int width,const int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif 