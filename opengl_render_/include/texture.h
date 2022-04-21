#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

#include "enum.h"

using namespace oglRender;  // NOLINT(clang-diagnostic-header-hygiene)

class Texture
{
public:
    explicit Texture(const std::string& path); // 读取文件 
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

    // 翻转图像
    static void flipVerticallyImage();

    unsigned int textureId() const;

private:
    // 紋理的的图片信息
    int m_width;
    int m_height;
    // 通道信息 
    int m_nrChannels;
    // 调用create之前暂存的 图像数据 
    unsigned char* m_data; //暂存 像素的数据

    // wrap 
    GLuint m_wrapS;
    GLuint m_wrapT;

    // filter 
    GLuint m_filterMin;
    GLuint m_filterMag;

    // texture ID  
    unsigned int m_textureId{0};

public:
    // 使用 stb_image 读取一个纹理 返回纹理的ID 
    static unsigned int textureFromFile(const std::string& path, const std::string& directory, bool gamma = false);
    // load picture as texture 
    static unsigned int loadTexture(const char* path);
};


#endif // !1
