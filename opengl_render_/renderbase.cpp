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
	// ������յ���ɫ 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// �����ɫ���� 
	glClear(GL_COLOR_BUFFER_BIT);
	
}
