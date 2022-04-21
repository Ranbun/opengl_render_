#ifndef SSAO_H_
#define SSAO_H_

/*
*
*  逐片段位置向量
*  逐片段的法线向量
*  逐片段的反射颜色
*  采样核心
*  用来旋转采样核心的随机旋转矢量
*
*
 */


#include <map>
#include <random>
#include "glad/glad.h"

#include <cmath>
#include <istream>
#include <numbers>
#include "model.h"
#include "ofapp.h"





class SSAO final :public RenderApp
{
public:
    // ReSharper disable once IdentifierTypo
    SSAO();
    SSAO(const SSAO&) = delete;
    SSAO(const SSAO&&) = delete;
    SSAO& operator=(const SSAO&) = delete;
    SSAO& operator=(const SSAO&&) = delete;

    ~SSAO() override;

    void init()  override
    {
        createGBuffer();
    }

    void createGBuffer()
    {
        // gPositionDepth - 记录点的世界坐标 & 线性深度 
        unsigned int buffers[3]{ 0 };
        glGenBuffers(3, buffers);

        {
            m_buffers["position"] = buffers[0];
            m_buffers["normal"] = buffers[1];
            m_buffers["albed"] = buffers[2];

            // init sposition
            glBindBuffer(GL_TEXTURE_2D, buffers[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width(), height(), 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


        }






    }


    void render()  override
    {
        RenderApp::render();
    }

protected:

    static GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
    {
        return a + f * (b - a);
    }

    void generateRandomDataX64()
    {
        std::uniform_real_distribution<GLfloat>  randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        for(GLuint i = 0; i < 64;++i)
        {
            // 生成一个球形坐标 x[-1 1] y[-1 1] z[0 1]
            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,  // 缩放到 -1 -- 1
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
            );

            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            GLfloat scale = GLfloat(i) / 64.0;
            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            m_ssaoKernel.push_back(sample);

        }
    }

private:

    // 记录所有的buffer
    /*
     * framebuffer
     * color
     *
     */
    std::map<std::string, unsigned int> m_buffers;
    std::vector<glm::vec3>  m_ssaoKernel;

};



#endif // !SSAO_H_

