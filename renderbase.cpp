#include "renderbase.h"
#include <glad/glad.h>

RenderBase::RenderBase()
{
}

RenderBase::~RenderBase()
{
	
}

void RenderBase::render()
{
	// 设置清空的颜色 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// 清空颜色缓冲 
	glClear(GL_COLOR_BUFFER_BIT);
	
}
