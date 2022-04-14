#include "texture.h"

#include <cassert>

Texture::Texture(const std::string& path)
    : m_width(0)
    , m_height(0)
    , m_nrChannels(0)
    , m_data(stbi_load(path.c_str(), &m_width, &m_height,
        &m_nrChannels, 0)), m_wrapS(GL_REPEAT)
    , m_wrapT(GL_REPEAT)
    , m_filterMin(GL_LINEAR_MIPMAP_LINEAR)
    , m_filterMag(GL_LINEAR)
{
    glGenTextures(1, &m_textureId);
}

Texture::~Texture() = default;

void Texture::bindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::createTexture()
{
    if (m_data)
    {
        // 绑定到当前创建的纹理 
        bindTexture();

        if (m_nrChannels < 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMag);
    }
    else
    {
        std::cout << "Failed to create texture" << std::endl;
    }

    stbi_image_free(m_data);
    m_data = nullptr;
}

void Texture::setWarpParameteri(const texture::WRAP wrap, unsigned int type)
{
    if (wrap == texture::WRAP::S)
    {
        m_wrapS = type;
    }
    else if (wrap == texture::WRAP::T)
    {
        m_wrapT = type;
    }
}

void Texture::setFilterParameteri(texture::FILETER filter, unsigned int type)
{
    if (filter == texture::FILETER::MIN)
    {
        m_filterMin = type;
    }
    else
    {
        m_filterMag = type;
    }
}

void Texture::releaseTexture()
{
    assert(this);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::flipVerticallyImage()
{
    stbi_set_flip_vertically_on_load(true);
}

unsigned Texture::textureId() const
{
    return m_textureId;
}

unsigned Texture::textureFromFile(const std::string& path, const std::string & directory, bool gamma)
{
    auto fileName = std::string(path);
    fileName = directory + '/' + fileName;

    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
        {
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureId;
}

unsigned Texture::loadTexture(char const* path)
{
    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureId;
}

