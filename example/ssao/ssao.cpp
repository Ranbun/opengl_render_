#include "ssao.h"

SSAO::SSAO()
    :RenderApp()
{
    m_camera.m_camera = new Camera(glm::vec3(0.0, 0.0, 5.0));
}

SSAO::~SSAO()
{
    if (m_cubeVao)
    {
        m_cubeVao->destroy();
        delete m_cubeVao;
        m_cubeVao = nullptr;
    }

    if (m_quadVao)
    {
        m_quadVao->destroy();
        delete m_quadVao;
        m_quadVao = nullptr;
    }

    m_buffers.clear();
    m_ssaoKernel.clear();
    m_ssaoNoise.clear();

}

void SSAO::createGBuffer()
{
    // gPositionDepth - 记录点的世界坐标 & 线性深度 
    unsigned int buffers[3]{ 0 };
    glGenBuffers(3, buffers);

    {  // Gbuffer Framebuffer
        unsigned int gframebuffer;
        glGenFramebuffers(1, &gframebuffer);
        m_buffers[gBufferFrameBuffer] = gframebuffer;
        glBindFramebuffer(GL_FRAMEBUFFER, gframebuffer);
    }

    {  // Gbuffer Framebuffer pass
        m_buffers[gPositionTexture] = buffers[0];
        m_buffers[gNormalTexture] = buffers[1];
        m_buffers[gColorTexture] = buffers[2];

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

}

void SSAO::createSSAOFrameBuffer()
{ // 计算环境光遮蔽的帧缓冲
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
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::createSSAOBulerFramebuffer()
{
    // 环境光遮蔽的模糊
    unsigned int ssaoBlurFBO;
    glGenFramebuffers(1, &ssaoBlurFBO);
    m_buffers[bluerFramebuffer] = ssaoBlurFBO;

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

    unsigned  int ssaoBlurTexture;
    glGenTextures(1, &ssaoBlurTexture);
    m_buffers[bulerColorTexture] = ssaoBlurTexture;

    glBindTexture(GL_TEXTURE_2D, ssaoBlurTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width(), height(), 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::render()
{
    RenderApp::render();
    // Gbuffer 阶段
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[gBufferFrameBuffer]);


    // 渲染SSAO结果
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[ssaoFrameBuffer]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
    model = glm::rotate(model, -90.0f, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    glm::mat4 projection = glm::perspective(m_camera.m_camera->fov(), static_cast<float>(width()) / height(), 0.1f, 100.0f);
    glm::mat4 view = m_camera.m_camera->getViewMatrix();
    m_gemotryPassShader->use();
    m_gemotryPassShader->setMat4("projection", projection);
    m_gemotryPassShader->setMat4("view", view);
    m_gemotryPassShader->setMat4("model", model);
    nanosuit->draw(m_gemotryPassShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // light pass
    // 开始光照阶段 
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 使用默认的帧缓冲
    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SSAO::createCube()
{
    assert(this);
    if (!m_cubeVao)
    {
        m_cubeVao = new VertexArrayObject();
        m_cubeVao->create();
    }

    m_cubeVao->bind();
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat), reinterpret_cast<const void*>(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<const void*>(6 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    m_cubeVao->release();
}

void SSAO::createQuad()
{
    assert(this);
    if (!m_quadVao)
    {
        m_quadVao = new VertexArrayObject();
        m_quadVao->create();
    }
    m_quadVao->bind();
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat quadVertices[] = {
        // Positions        // Texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<const void*>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_quadVao->release();
}

void SSAO::loadModel()
{
    nanosuit = new Model("./../resources/model/nanosuit/nanosuit.obj");
}

void SSAO::generateRandomDataX64()
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
