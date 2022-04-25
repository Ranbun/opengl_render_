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
*/

#include <map>
#include <random>
#include "glad/glad.h"

#include <cmath>
#include <iostream>
#include <istream>
#include <numbers>
#include "model.h"
#include "ofapp.h"
#include <string>

// gBuffer pass 
using SSAOBuffer = std::string;
static SSAOBuffer ssaoFrameBuffer("ssaoframeBuffer"); // 用来计算SSAO的片段
static SSAOBuffer ssaoColorTexture("ssaoColorTexture");

// 几何处理阶段 所有几何处理阶段使用的缓冲 
using gBuffer = std::string;
static gBuffer gBufferFrameBuffer("gBufferFrameBuffer");
static gBuffer gPositionTexture("gPositionTexture");
static gBuffer gNormalTexture("gNormalTexture");
static gBuffer gColorTexture("gColorTexture");
static gBuffer gRenderBuffer("gRenderBuffer");


// 存储了随机旋转的向量 - 每个片段用来旋转核心
using RotateVector = std::string;
static RotateVector noiseTexture("noiseTexture4X4");

using BulerPass = std::string;
static BulerPass bluerFramebuffer("BulerFramebuffer");
static BulerPass bulerColorTexture("bulerPassColorBuffer");


class SSAO final : public RenderApp
{
public:
    // ReSharper disable once IdentifierTypo
    SSAO();
    SSAO(const SSAO&) = delete;
    SSAO(const SSAO&&) = delete;
    SSAO& operator=(const SSAO&) = delete;
    SSAO& operator=(const SSAO&&) = delete;

    ~SSAO() override;

    void init() override
    {
        // 创建核心的旋转向量纹理 -- 随机转动核心 
        use4X4VectorCreateTexture();
        createGBuffer();
        createSSAOFrameBuffer();
        createSSAOBulerFramebuffer();
        createShader();
        createQuad();

    }



    // 渲染函数 
    void render() override;

protected:
    void createQuad();

    // 生成64个采样样本
    void generateRandomDataX64()
    {
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        for (GLuint i = 0; i < 64; ++i)
        {
            // 生成一个球形坐标 x[-1 1] y[-1 1] z[0 1]
            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0, // 缩放到 -1 -- 1
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
            );

            // 将这个位置约束到球的内部
            sample = normalize(sample);
            sample *= randomFloats(generator);

            // 让采样点 可能的靠近片段位置 
            GLfloat scale = static_cast<GLfloat>(i) / 64.0;
            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            m_ssaoKernel.push_back(sample);
        }
    }

    // 生成随机样本的旋转 
    void use4X4VectorCreateTexture();
    // 几何阶段的信息 -- 位置 法向 颜色 
    void createGBuffer();
    void createSSAOFrameBuffer()
    {
        assert(this);
    }

    void createSSAOBulerFramebuffer()
    {
        assert(this);
    }


private:
    // 记录所有的buffer
    /*
     * framebuffer
     * color
     *
     */
    std::map<std::string, unsigned int> m_buffers;
    // 随机采样点 
    std::vector<glm::vec3> m_ssaoKernel;
    // 随机旋转向量 
    std::vector<glm::vec3> m_ssaoNoise;

    Shader* m_gemotryPassShader{ nullptr };  // 几何处理阶段的着色起 
    Shader* m_calsSSAOPassShader{ nullptr }; // 计算SSAO 的shader                      
    Shader* m_lightPassShader{ nullptr };    // 光照阶段的shader                 
    Shader* m_bluerPassShader{ nullptr };    // 模糊边界shader                 


private:
    // 生成纹理 
    void generateRotateVecrtor4X4();
    static GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
    void createShader();
};

#endif // !SSAO
