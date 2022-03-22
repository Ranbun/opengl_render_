#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GL/GL.h>
#include "render_en.h"
#include "stb_image.h"

class Texture
{
public:
	explicit Texture(std::string& path);        // 读取文件 
	~Texture();                                  // 析构函数 
	void bindTexture() const;                    // bind
	void createTexture();                        //create
	// set wrap & filter
	void setWarpParameteri(const texture::WRAP wrap, unsigned int type);
	void setFilterParameteri(texture::FILETER filter, unsigned int type);

	void releaseTexture() const;                         // 解除当前纹理的绑定 
	void flipVerticallyImage();                          // 翻转图像
	unsigned int textureId() const;                      // 返回创建的纹理的ID

private:
	// 紋理的的图片信息
	int width_{};               // 宽度
	int height_{};              // 高度 
	int nr_channels_{};         // 通道信息

	bool init_;                 // 初始化当前纹理 -- 记录纹理是否被创建 

	// 调用create之前暂存的 图像数据 
	unsigned char* data_;      //暂存 像素的数据

	// wrap -- 环绕方式 
	GLuint wrap_s_;
	GLuint wrap_t_;

	// filter -- 过滤
	GLuint filter_min_;
	GLuint filter_mag_;

	// texture ID  
	unsigned int texture_id_{};

public:
	// 使用 stb_image 读取一个纹理 返回纹理的ID 
	static unsigned int textureFromFile(std::string path, std::string& directory,bool gamma = false);
	// load picture as texture 
	static  unsigned int loadTexture(char const* path);
};



#endif // !1
