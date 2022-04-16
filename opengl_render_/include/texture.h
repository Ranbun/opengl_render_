#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

#include "enum.h"

using namespace oglRender;  // NOLINT(clang-diagnostic-header-hygiene)

class Texture
{
public:
    explicit Texture(const std::string& path); // ��ȡ�ļ� 
    ~Texture();
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(const Texture&&) = delete;
    Texture& operator=(const Texture&&) = delete;

    // bind 
    void bindTexture() const;
    //create 
    void createTexture();
    // set wrap & filter
    void setWarpParameteri(texture::wrap wrap, unsigned int type);
    void setFilterParameteri(texture::fileter filter, unsigned int type);
    // release
    void releaseTexture();

    // ��תͼ��
    static void flipVerticallyImage();

    unsigned int textureId() const;

private:
    // �y��ĵ�ͼƬ��Ϣ
    int m_width;
    int m_height;
    // ͨ����Ϣ 
    int m_nrChannels;
    // ����create֮ǰ�ݴ�� ͼ������ 
    unsigned char* m_data; //�ݴ� ���ص�����

    // wrap 
    GLuint m_wrapS;
    GLuint m_wrapT;

    // filter 
    GLuint m_filterMin;
    GLuint m_filterMag;

    // texture ID  
    unsigned int m_textureId{0};

public:
    // ʹ�� stb_image ��ȡһ������ ���������ID 
    static unsigned int textureFromFile(const std::string& path, const std::string& directory, bool gamma = false);
    // load picture as texture 
    static unsigned int loadTexture(const char* path);
};


#endif // !1
