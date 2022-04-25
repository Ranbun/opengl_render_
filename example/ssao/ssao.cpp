#include "ssao.h"

SSAO::SSAO()
    :RenderApp()
{
    m_camera.m_camera = new Camera(glm::vec3(0.0, 0.0, 5.0));
}

SSAO::~SSAO()
{
}

void SSAO::createGBuffer()
{
    // gPositionDepth - 记录点的世界坐标 & 线性深度 
    unsigned int buffers[3]{ 0 };
    glGenBuffers(3, buffers);

    {
        unsigned int gframebuffer;
        glGenFramebuffers(1, &gframebuffer);
        m_buffers[gBufferFrameBuffer] = gframebuffer;
        glBindFramebuffer(GL_FRAMEBUFFER,gframebuffer);
    }

    {
        m_buffers[gPositionTexture] = buffers[0];
        m_buffers[gNormalTexture  ] = buffers[1];
        m_buffers[gColorTexture   ] = buffers[2];

        // init position
        glBindBuffer(GL_TEXTURE_2D, buffers[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width(), height(), 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_buffers[gPositionTexture], 0);

        // normal 
        glBindBuffer(GL_TEXTURE_2D, m_buffers[gNormalTexture]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width(), height(), 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_buffers[gNormalTexture], 0);

        // albedo color buffer 
        glBindBuffer(GL_TEXTURE_2D, m_buffers[gColorTexture]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_buffers[gColorTexture], 0);

        // render buffer
        unsigned int renderbuffer;
        glGenRenderbuffers(1, &renderbuffer);
        m_buffers[gRenderBuffer] = renderbuffer;
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width(), height());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "GBuffer Framebuffer not complete!" << std::endl;

    }

    {
        // 计算环境光遮蔽的帧缓冲 
        GLuint ssaoFBO;
        glGenFramebuffers(1, &ssaoFBO);
        m_buffers[ssaoFrameBuffer] = ssaoFBO;
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

        GLuint ssaoColorBuffer;
        glGenTextures(1, &ssaoColorBuffer);
        m_buffers[ssaoColorTexture] = ssaoColorBuffer;
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        //环境光遮蔽 是一个灰度值 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width(), height(), 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // 绑定颜色纹理到帧缓冲  
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    }
}

void SSAO::render()
{
    RenderApp::render();

    // Gbuffer 阶段
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[gBufferFrameBuffer]);


    // 渲染SSAO结果
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[ssaoFrameBuffer]);

    // light pass
    // 开始光照阶段 
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 使用默认的帧缓冲
    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SSAO::createQuad()
{
    assert(this);
}

void SSAO::use4X4VectorCreateTexture()
{
    // 生成创建纹理的数据 
    generateRotateVecrtor4X4();

    GLuint noiseTexture_;
    glGenTextures(1, &noiseTexture_);

    m_buffers[noiseTexture] = noiseTexture_;

    glBindTexture(GL_TEXTURE_2D, noiseTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, m_ssaoNoise.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAO::generateRotateVecrtor4X4()
{
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
    std::default_random_engine generator;

    for (GLuint i = 0; i < 16; ++i)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f
        );
        m_ssaoNoise.push_back(noise);
    }
}

GLfloat SSAO::lerp(GLfloat a, GLfloat b, GLfloat f)
{
    return a + f * (b - a);
}

void SSAO::createShader()
{
    m_gemotryPassShader = new Shader("./../resources/shader/ssao/ssao_geomtry_pass.vert",
                                     "./../resources/shader/ssao/ssao_geomtry_pass.frag");
    m_calsSSAOPassShader = new Shader("./../resources/shader/ssao/cal_ssao.vert",
                                      "./../resources/shader/ssao/cal_ssao.frag");

    m_bluerPassShader = new Shader("./../resources/shader/ssao/ssao_buler.vert",
                                   "./../resources/shader/ssao/ssao_buler.frag");

    m_lightPassShader = new Shader("./../resources/shader/ssao/ssao_light_pass.vert",
                                   "./../resources/shader/ssao/ssao_light_pass.frag");

}
