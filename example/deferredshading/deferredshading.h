#ifndef   DEFERREDSHADING_H_
#define   DEFERREDSHADING_H_

#include "glad/glad.h"
#include <map>
#include "model.h"
#include "ofapp.h"

class DeferredShading : public RenderApp
{

public:
    DeferredShading();

    ~DeferredShading() override;

    void render() override
    {
        // RenderBase::render();
        // glClearColor(0.1,0.2,0.5,1.0);
        // glClear(GL_COLOR_BUFFER_BIT);
        // 延迟渲染 两个阶段
        // 1.1 几何阶段 渲染为多个缓冲 记录各个最终显示的片元的信息到纹理中
        // 每个片段需要记录的是 世界空间的位置 法向量 颜色 镜面强度值
        // 正常的渲染场景 不计算光照的信息
        // bind frame buffer

        glBindFramebuffer(GL_FRAMEBUFFER, m_buffers["framebuffer"]);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_BLEND);
        auto radio = static_cast<float>(width()) / height();
        glm::mat4 projection = glm::perspective(m_camera.camera()->fov(), radio, 0.1f, 100.0f);
        glm::mat4 view = m_camera.camera()->getViewMatrix();
        glm::mat4 modelmat(1.0);
        gbuffer_shader_->use();
        gbuffer_shader_->setMat4("view", view);
        gbuffer_shader_->setMat4("projection", projection);
        for (GLuint i = 0; i < objectPositions.size(); i++)
        {
            modelmat = glm::mat4(1.0);
            modelmat = glm::translate(modelmat, objectPositions[i]);
            modelmat = glm::scale(modelmat, glm::vec3(0.25f));
            gbuffer_shader_->setMat4("model", modelmat);
            model->draw(gbuffer_shader_);
        }

        // use default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 1.2 光照阶段 使用第一阶段的片元的信息 渲染结果
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
        glBlendEquation(GL_MAX);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        light_pass_shader_->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_buffers["position"]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_buffers["normal"]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_buffers["diffuse&specular"]);

        for (GLuint i = 0; i < lightPositions.size(); i++)
        {
            light_pass_shader_->setVec3(std::string("lights[" + std::to_string(i) + "].Position"), lightPositions[i]);
            light_pass_shader_->setVec3(std::string("lights[" + std::to_string(i) + "].Color"), lightColors[i]);
            // Update attenuation parameters and calculate radius
            const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
            const GLfloat linear = 0.7;
            const GLfloat quadratic = 1.8;
            light_pass_shader_->setFloat(("lights[" + std::to_string(i) + "].Linear").c_str(), linear);
            light_pass_shader_->setFloat(("lights[" + std::to_string(i) + "].Quadratic").c_str(), quadratic);

            const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
            GLfloat radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * maxBrightness))) / (2 * quadratic);

            light_pass_shader_->setFloat(("lights[" + std::to_string(i) + "].Radius").c_str(), radius);

        }
        light_pass_shader_->setVec3("viewPos", m_camera.camera()->position());
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // 获取GBuffer中的深度信息
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_buffers["framebuffer"]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        // use default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 渲染光源
        draw_lightcube_shader_->use();
        glBindVertexArray(cubeVAO);
        view = m_camera.camera()->getViewMatrix();
        draw_lightcube_shader_->setMat4("view", view);
        draw_lightcube_shader_->setMat4("projection", projection);
        for (GLuint i = 0; i < lightPositions.size(); i++)
        {
            modelmat = glm::mat4(1.0);
            modelmat = glm::translate(modelmat, lightPositions[i]);
            modelmat = glm::scale(modelmat, glm::vec3(0.25f));

            draw_lightcube_shader_->setMat4("model", modelmat);
            draw_lightcube_shader_->setVec3("lightColor", lightColors[i]);

            // RenderCube();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
    }

    void init() override;

    void initGeomPass();
    void initLightPass();

    // 创建绘制的场景
    void createCube();
    void createQuad();

private:
    // 分别记录位置 颜色 镜面(颜色和值)
    std::map<std::string, unsigned int> m_buffers;
    // 渲染缓冲作为深度
    unsigned  int depth;

    Shader* gbuffer_shader_;
    Shader* light_pass_shader_;
    Shader* draw_lightcube_shader_;

    GLuint cubeVAO{ 0 };
    GLuint cubeVBO{ 0 };
    GLuint quadVAO{ 0 };
    GLuint quadVBO{ 0 };

    Model* model;
    const GLuint NR_LIGHTS = 32;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    // model position
    std::vector<glm::vec3> objectPositions;


};

#endif // _DEFERREDSHADING_H_