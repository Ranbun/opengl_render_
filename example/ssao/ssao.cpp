#include "ssao.h"

SSAO::SSAO()
    :RenderApp()
{
    m_camera.m_camera = new Camera(glm::vec3(0.0, 0.0, 3.0));
}

SSAO::~SSAO()
{
}
