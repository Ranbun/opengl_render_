#ifndef OF_APP_H_
#define OF_APP_H_

#include "renderwidget.h"
#include <glm/vec3.hpp>
#include "camera.h"

class RenderApp : public RenderWidget
{
public:
    explicit RenderApp(const int width = 800, const int height = 600, const std::string& title = "Render Widget")
        : RenderWidget(width, height, title)
    {
        setFrameBufferSizeCallback();
        setMousePosCallback();
        setScrollCallback();

        //  m_camera.m_camera = new Camera(glm::vec3(0.0, 0.0, 3.0));
    }

    RenderApp(const RenderApp&) = delete;
    RenderApp(const RenderApp&&) = delete;
    RenderApp& operator=(const RenderApp&) = delete;
    RenderApp& operator=(const RenderApp&&) = delete;

    ~RenderApp() override = default;
};

#endif
