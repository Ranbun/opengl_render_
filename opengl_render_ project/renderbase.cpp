#include "renderbase.h"
#include <glad/glad.h>
#include <Glfw/glfw3.h>
#include "renderwidget.h"

RenderBase::RenderBase()
{
	default_camera_ = nullptr;
	default_shader_ = nullptr;
}

RenderBase::~RenderBase()
{
	if(default_camera_)
    {
        delete default_camera_;
    }
    if(default_shader_)
    {
        delete default_shader_;
    }

}

void RenderBase::render()
{
	// 设置清空的颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// 清空颜色缓冲 
	glClear(GL_COLOR_BUFFER_BIT);
    assert(default_shader_);

    default_shader_->use();
    glm::mat4 model(1);
    default_shader_->setMat4("model", model);
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
    default_shader_->setMat4("view", view);
    glm::mat4 projection = glm::ortho(-0.5, 0.5, -0.5, 0.5,1.0,100.0);
    default_shader_->setMat4("projection", projection);

    //获取当前时间
    auto time = glfwGetTime();
    default_shader_->setFloat("u_time",time);
    auto w = RenderWidget::getSingleObject()->getWidth();
    auto h = RenderWidget::getSingleObject()->getHeight();
    default_shader_->setVec2("u_resolution",w,h);
    default_vao_->bind();

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);

}

Camera* RenderBase::currentCamera()
{
	return default_camera_;
}

void RenderBase::init()
{
    default_camera_ = new Camera(glm::vec3(0.0,0.0,3.0),glm::vec3(0,1,0),0,0);
    default_shader_ = new Shader("./../shader/triangle.vert","./../shader/yuv_matrix.frag");

    // draw use ebo
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f  // top left
    };

    unsigned int indices[] =
            {
                    0,1,3,
                    1,2,3
            };

    default_vao_ = new VertexArrayObject;
    default_vao_->create();

    default_vbo_ = new BufferObject(object_buffer::OBJECT_TYPE::VERTEX_BUFFER);
    default_vbo_->create();

    default_ebo_ = new BufferObject(object_buffer::OBJECT_TYPE::INDEX_BUFFER);
    default_ebo_->create();

    default_vao_->bind();
    auto res = default_vbo_->bind();
    if (res)
    {
        default_vbo_->allocate(vertices, sizeof(vertices));
        res = false;
    }

    res = default_ebo_->bind();
    if (res)
    {
        default_ebo_->allocate(indices, sizeof(indices));
    }

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    default_vbo_->release();
    VertexArrayObject::release();

}
