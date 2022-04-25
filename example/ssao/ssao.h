#ifndef SSAO_H_
#define SSAO_H_

/*
*
*  ��Ƭ��λ������
*  ��Ƭ�εķ�������
*  ��Ƭ�εķ�����ɫ
*  ��������
*  ������ת�������ĵ������תʸ��
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
static SSAOBuffer ssaoFrameBuffer("ssaoframeBuffer"); // ��������SSAO��Ƭ��
static SSAOBuffer ssaoColorTexture("ssaoColorTexture");

// ���δ���׶� ���м��δ���׶�ʹ�õĻ��� 
using gBuffer = std::string;
static gBuffer gBufferFrameBuffer("gBufferFrameBuffer");
static gBuffer gPositionTexture("gPositionTexture");
static gBuffer gNormalTexture("gNormalTexture");
static gBuffer gColorTexture("gColorTexture");
static gBuffer gRenderBuffer("gRenderBuffer");


// �洢�������ת������ - ÿ��Ƭ��������ת����
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
        // �������ĵ���ת�������� -- ���ת������ 
        use4X4VectorCreateTexture();
        createGBuffer();
        createSSAOFrameBuffer();
        createSSAOBulerFramebuffer();
        createShader();
        createQuad();

    }



    // ��Ⱦ���� 
    void render() override;

protected:
    void createQuad();

    // ����64����������
    void generateRandomDataX64()
    {
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        for (GLuint i = 0; i < 64; ++i)
        {
            // ����һ���������� x[-1 1] y[-1 1] z[0 1]
            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0, // ���ŵ� -1 -- 1
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
            );

            // �����λ��Լ��������ڲ�
            sample = normalize(sample);
            sample *= randomFloats(generator);

            // �ò����� ���ܵĿ���Ƭ��λ�� 
            GLfloat scale = static_cast<GLfloat>(i) / 64.0;
            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            m_ssaoKernel.push_back(sample);
        }
    }

    // ���������������ת 
    void use4X4VectorCreateTexture();
    // ���ν׶ε���Ϣ -- λ�� ���� ��ɫ 
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
    // ��¼���е�buffer
    /*
     * framebuffer
     * color
     *
     */
    std::map<std::string, unsigned int> m_buffers;
    // ��������� 
    std::vector<glm::vec3> m_ssaoKernel;
    // �����ת���� 
    std::vector<glm::vec3> m_ssaoNoise;

    Shader* m_gemotryPassShader{ nullptr };  // ���δ���׶ε���ɫ�� 
    Shader* m_calsSSAOPassShader{ nullptr }; // ����SSAO ��shader                      
    Shader* m_lightPassShader{ nullptr };    // ���ս׶ε�shader                 
    Shader* m_bluerPassShader{ nullptr };    // ģ���߽�shader                 


private:
    // �������� 
    void generateRotateVecrtor4X4();
    static GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
    void createShader();
};

#endif // !SSAO
